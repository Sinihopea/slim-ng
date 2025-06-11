/*
 * slimlock
 * Copyright (c) 2010-2012 Joel Burget <joelburget@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "cfg.hpp"
#include "panel.hpp"
#include "util.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/dpms.h>
#include <X11/keysym.h>
#include <algorithm>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/vt.h>
#include <pthread.h>
#include <security/pam_appl.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#undef APPNAME
#define APPNAME "slimlock"
#define SLIMLOCKCFG SYSCONFDIR "/slimlock.conf"

void setBackground(const std::string &themedir);
void HideCursor();
bool AuthenticateUser();
static int ConvCallback(int num_msgs, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
std::string findValidRandomTheme(const std::string &set);
void HandleSignal(int sig);
void *RaiseWindow(void *data);

/* I really didn't wanna put these globals here, but it's the only way... */
Display *dpy;
int scr;
Window win;
Window root;
Cfg m_config_slimlock;
Panel *loginPanel;
std::string themeName = "";
pam_handle_t *pam_handle;
struct pam_conv conv = {ConvCallback, nullptr};
CARD16 dpms_standby;
CARD16 dpms_suspend;
CARD16 dpms_off;
CARD16 dpms_level;
BOOL dpms_state;
BOOL using_dpms;
int term;

static void die(const char *errstr, ...)
{
	va_list ap;
	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if ((argc == 2) && !strcmp("-v", argv[1]))
	{
		die(APPNAME "-" VERSION ", © 2010-2012 Joel Burget\n");
	}
	else if (argc != 1)
	{
		die("usage: " APPNAME " [-v]\n");
	}

	void (*prev_fn)(int);

	/* restore DPMS settings should slimlock be killed in the line of duty */
	prev_fn = signal(SIGTERM, HandleSignal);

	if (prev_fn == SIG_IGN)
	{
		signal(SIGTERM, SIG_IGN);
	}

	/* create a lock file to solve mutliple instances problem /var/lock used to
	be the place to put this, now it's /run/lock ...i think */
	struct stat statbuf;
	int lock_file;

	/* try /run/lock first, since i believe it's preferred */
	if (!stat("/run/lock", &statbuf))
	{
		lock_file = open("/run/lock/" APPNAME ".lock", O_CREAT | O_RDWR, 0666);
	}
	else
	{
		lock_file = open("/var/lock/" APPNAME ".lock", O_CREAT | O_RDWR, 0666);
	}

	int rc = flock(lock_file, LOCK_EX | LOCK_NB);

	if (rc)
	{
		if (EWOULDBLOCK == errno)
		{
			die(APPNAME " already running\n");
		}
	}

	unsigned int cfg_passwd_timeout;

	/* Read user's current theme */
	m_config_slimlock.readConf(CFGFILE);
	m_config_slimlock.readConf(SLIMLOCKCFG);
	std::string themebase = "";
	std::string themefile = "";
	std::string themedir = "";
	themeName = "";
	themebase = m_config_slimlock.getOption("themes_dir") + "/";
	themeName = m_config_slimlock.getOption("current_theme");
	std::string::size_type pos;

	if ((pos = themeName.find(",")) != std::string::npos)
	{
		themeName = findValidRandomTheme(themeName);
	}

	bool loaded = false;

	while (!loaded)
	{
		themedir = themebase + themeName;
		themefile = themedir + THEMESFILE;
		if (!m_config_slimlock.readConf(themefile))
		{
			if (themeName == "default")
			{
				std::cerr << APPNAME << ": Failed to open default theme file " << themefile << std::endl;
				exit(ERR_EXIT);
			}
			else
			{
				std::cerr << APPNAME << ": Invalid theme in config: " << themeName << std::endl;
				themeName = "default";
			}
		}
		else
		{
			loaded = true;
		}
	}

	const char *display = getenv("DISPLAY");

	if (!display)
	{
		display = DISPLAY;
	}

	XInitThreads();

	if (!(dpy = XOpenDisplay(display)))
	{
		die(APPNAME ": cannot open display\n");
	}

	scr = DefaultScreen(dpy);
	XSetWindowAttributes wa;
	wa.override_redirect = 1;

	/* Create a full screen window */
	root = RootWindow(dpy, scr);
	win = XCreateWindow(dpy, root, 0, 0, DisplayWidth(dpy, scr), DisplayHeight(dpy, scr), 0, DefaultDepth(dpy, scr),
		CopyFromParent, DefaultVisual(dpy, scr), CWOverrideRedirect, &wa);
	XMapWindow(dpy, win);
	XFlush(dpy);

	for (int len = 1000; len; len--)
	{
		if (XGrabKeyboard(dpy, root, True, GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess)
		{
			break;
		}

		usleep(1000);
	}

	XSelectInput(dpy, win, ExposureMask | KeyPressMask);

	/* This hides the cursor if the user has that option enabled in their configuration */
	HideCursor();
	loginPanel = new Panel(dpy, scr, win, m_config_slimlock, themedir, Panel::Mode_Lock);
	int ret = pam_start(APPNAME, loginPanel->GetName().c_str(), &conv, &pam_handle);

	/* If we can't start PAM, just exit because slimlock won't work right */
	if (ret != PAM_SUCCESS)
	{
		die("PAM: %s\n", pam_strerror(pam_handle, ret));
	}

	/* disable tty switching */
	if (m_config_slimlock.getOption("tty_lock") == "1")
	{
		if ((term = open("/dev/console", O_RDWR)) == -1)
		{
			perror("error opening console");
		}

		if ((ioctl(term, VT_LOCKSWITCH)) == -1)
		{
			perror("error locking console");
		}
	}

	/* Set up DPMS */
	unsigned int cfg_dpms_standby, cfg_dpms_off;
	cfg_dpms_standby = Cfg::string2int(m_config_slimlock.getOption("dpms_standby_timeout").c_str());
	cfg_dpms_off = Cfg::string2int(m_config_slimlock.getOption("dpms_off_timeout").c_str());
	using_dpms = DPMSCapable(dpy) && (cfg_dpms_standby > 0);

	if (using_dpms)
	{
		DPMSGetTimeouts(dpy, &dpms_standby, &dpms_suspend, &dpms_off);
		DPMSSetTimeouts(dpy, cfg_dpms_standby, cfg_dpms_standby, cfg_dpms_off);
		DPMSInfo(dpy, &dpms_level, &dpms_state);

		if (!dpms_state)
		{
			DPMSEnable(dpy);
		}
	}

	/* Get password timeout */
	cfg_passwd_timeout = Cfg::string2int(m_config_slimlock.getOption("wrong_passwd_timeout").c_str());

	/* Let's just make sure it has a sane value */
	cfg_passwd_timeout = cfg_passwd_timeout > 60 ? 60 : cfg_passwd_timeout;

	pthread_t raise_thread;
	pthread_create(&raise_thread, nullptr, RaiseWindow, nullptr);

	/* Main loop */
	while (true)
	{
		loginPanel->ResetPasswd();

		/* AuthenticateUser returns true if authenticated */
		if (AuthenticateUser())
		{
			break;
		}

		loginPanel->WrongPassword(cfg_passwd_timeout);
	}

	/* kill thread before destroying the window that it's supposed to be raising */
	pthread_cancel(raise_thread);
	loginPanel->ClosePanel();

	delete loginPanel;

	/* Get DPMS stuff back to normal */
	if (using_dpms)
	{
		DPMSSetTimeouts(dpy, dpms_standby, dpms_suspend, dpms_off);

		/* turn off DPMS if it was off when we entered */
		if (!dpms_state)
		{
			DPMSDisable(dpy);
		}
	}

	XCloseDisplay(dpy);
	flock(lock_file, LOCK_UN);
	close(lock_file);

	if (m_config_slimlock.getOption("tty_lock") == "1")
	{
		if ((ioctl(term, VT_UNLOCKSWITCH)) == -1)
		{
			perror("error unlocking console");
		}
	}

	close(term);

	return 0;
}

void HideCursor()
{
	if (m_config_slimlock.getOption("hidecursor") == "true")
	{
		XColor black;
		char cursordata[1];
		Pixmap cursorpixmap;
		Cursor cursor;
		cursordata[0] = 0;
		cursorpixmap = XCreateBitmapFromData(dpy, win, cursordata, 1, 1);
		black.red = 0;
		black.green = 0;
		black.blue = 0;
		cursor = XCreatePixmapCursor(dpy, cursorpixmap, cursorpixmap, &black, &black, 0, 0);
		XFreePixmap(dpy, cursorpixmap);
		XDefineCursor(dpy, win, cursor);
	}
}

static int ConvCallback(int num_msgs, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
	loginPanel->EventHandler(Panel::Get_Passwd);

	/* PAM expects an array of responses, one for each message */
	if (num_msgs == 0 || (*resp = (pam_response *)calloc(num_msgs, sizeof(struct pam_message))) == nullptr)
	{
		return PAM_BUF_ERR;
	}

	for (int i = 0; i < num_msgs; i++)
	{
		if (msg[i]->msg_style != PAM_PROMPT_ECHO_OFF && msg[i]->msg_style != PAM_PROMPT_ECHO_ON)
		{
			continue;
		}

		/* return code is currently not used but should be set to zero */
		resp[i]->resp_retcode = 0;

		if ((resp[i]->resp = strdup(loginPanel->GetPasswd().c_str())) == nullptr)
		{
			free(*resp);

			return PAM_BUF_ERR;
		}
	}

	return PAM_SUCCESS;
}

bool AuthenticateUser() { return (pam_authenticate(pam_handle, 0) == PAM_SUCCESS); }

std::string findValidRandomTheme(const std::string &set)
{
	/* extract random theme from theme set; return empty string on error */
	std::string name = set;
	struct stat buf;

	if (name[name.length() - 1] == ',')
	{
		name.erase(name.length() - 1);
	}

	Util::srandom(Util::makeseed());
	std::vector<std::string> themes;
	std::string themefile;
	Cfg::split(themes, name, ',');

	do
	{
		int sel = Util::random() % themes.size();
		name = Cfg::Trim(themes[sel]);
		themefile = m_config_slimlock.getOption("themes_dir") + "/" + name + THEMESFILE;

		if (stat(themefile.c_str(), &buf) != 0)
		{
			themes.erase(find(themes.begin(), themes.end(), name));
			std::cerr << APPNAME << ": Invalid theme in config: " << name << std::endl;
			name = "";
		}
	}

	while (name == "" && themes.size());

	return name;
}

void HandleSignal(int sig)
{
	/* Get DPMS stuff back to normal */
	if (using_dpms)
	{
		DPMSSetTimeouts(dpy, dpms_standby, dpms_suspend, dpms_off);

		/* turn off DPMS if it was off when we entered */
		if (!dpms_state)
		{
			DPMSDisable(dpy);
		}
	}

	if ((ioctl(term, VT_UNLOCKSWITCH)) == -1)
	{
		perror("error unlocking console");
	}

	close(term);
	loginPanel->ClosePanel();

	delete loginPanel;

	die(APPNAME ": Caught signal; dying\n");
}

/** i think this should be in an event loop instead of this threaded thing */
void *RaiseWindow(void *data)
{
	while (1)
	{
		XRaiseWindow(dpy, win);
		XGrabKeyboard(dpy, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);
		sleep(1);
	}

	return (void *)0;
}
