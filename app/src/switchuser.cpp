/*
 * SLiM - Simple Login Manager
 * Copyright (C) 1997, 1998 Per Liden
 * Copyright (C) 2004-06 Simone Rota <sip@varlock.com>
 * Copyright (C) 2004-06 Johannes Winkelmann <jw@tks6.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "switchuser.hpp"

#include "util.hpp"

#include <cstdio>

SwitchUser::SwitchUser(struct passwd *pw, Cfg &c, const std::string &display, char **_env)
	: m_config_switchuser(c), Pw(pw), m_display_name(display), m_environment(_env)
{
}

void SwitchUser::Login(const char *cmd, const char *mcookie)
{
	SetUserId();
	SetClientAuth(mcookie);
	Execute(cmd);
}

void SwitchUser::SetUserId()
{
	if ((Pw == nullptr) || (initgroups(Pw->pw_name, Pw->pw_gid) != 0) || (setgid(Pw->pw_gid) != 0) ||
		(setuid(Pw->pw_uid) != 0))
	{
		logStream << APPNAME << ": could not switch user id" << std::endl;
		std::exit(ERR_EXIT);
	}
}

void SwitchUser::Execute(const char *cmd)
{
	if (-1 == chdir(Pw->pw_dir))
	{
		/** @todo Print strerror or something. */
	}

	execle(Pw->pw_shell, Pw->pw_shell, "-c", cmd, nullptr, m_environment);
	logStream << APPNAME << ": could not execute login command" << std::endl;
}

void SwitchUser::SetClientAuth(const char *mcookie)
{
	std::string home = std::string(Pw->pw_dir);
	std::string authfile = home + "/.Xauthority";
	remove(authfile.c_str());
	Util::add_mcookie(mcookie, ":0", m_config_switchuser.getOption("xauth_path"), authfile);
}
