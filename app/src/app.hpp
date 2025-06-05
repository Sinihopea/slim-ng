/*
 *  SLiM - Simple Login Manager
 *  Copyright (C) 1997, 1998 Per Liden
 *  Copyright (C) 2004-06 Simone Rota <sip@varlock.com>
 *  Copyright (C) 2004-06 Johannes Winkelmann <jw@tks6.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef _APP_H_
#define _APP_H_

#include <X11/Xatom.h>
// #include <X11/Xlib.h>
// #include <errno.h>
// #include <iostream>
// #include <setjmp.h>
// #include <signal.h>
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <unistd.h>

#include "cfg.hpp"
#include "image.hpp"
#include "panel.hpp"

#ifdef USE_PAM
#include "pam.hpp"
#endif

#ifdef USE_CONSOLEKIT
#include "ck.hpp"
#endif

#define MCOOKIESIZE 32

class App
{
  public:
	explicit App (int argc, char **argv);
	~App () = default;

	void Run ();
	int GetServerPID ();
	void RestartServer ();
	void StopServer ();

	/* Lock functions */
	void GetLock ();
	void RemoveLock ();

	bool isServerStarted ();

  private:
	void Login ();
	void Reboot ();
	void Halt ();
	void Suspend ();
	void Console ();
	void Exit ();
	void KillAllClients (Bool top);
	// void ReadConfig ();
	void OpenLog ();
	void CloseLog ();
	void HideCursor ();
	void CreateServerAuth ();
	char *StrConcat (const char *str1, const char *str2);
	void UpdatePid ();
	bool AuthenticateUser (bool focuspass);
	std::string findValidRandomTheme (const std::string &set);
	static void replaceVariables (std::string &input, const std::string &var, const std::string &value);

	/* Server functions */
	int StartServer ();
	int ServerTimeout (int timeout, char *string);
	int WaitForServer ();

	/* Private data */
	Display *m_display;
	int m_screen;
	Window m_window_root;
	Panel *LoginPanel;
	int m_server_pid;
	const char *m_display_name;
	bool m_server_started;

#ifdef USE_PAM
	PAM::Authenticator pam;
#endif

#ifdef USE_CONSOLEKIT
	Ck::Session ck;
	bool consolekit_support_enabled;
#endif

	/* Options */
	void blankScreen ();
	void setBackground (const std::string &themedir);

	// char *DispName;
	Cfg m_config_app;
	// Pixmap BackgroundPixmap;
	Image *image;
	Atom BackgroundPixmapId;
	bool m_first_login;
	bool m_daemon_mode;
	bool m_force_no_daemon;

	/* For testing themes */
	char *m_test_theme;
	bool m_testing;
	std::string themeName;
	std::string mcookie;
};

#endif /* _APP_H_ */
