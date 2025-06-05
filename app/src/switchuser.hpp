/* SLiM - Simple Login Manager
   Copyright (C) 1997, 1998 Per Liden
   Copyright (C) 2004-06 Simone Rota <sip@varlock.com>
   Copyright (C) 2004-06 Johannes Winkelmann <jw@tks6.net>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
*/

#ifndef _SWITCHUSER_H_
#define _SWITCHUSER_H_

#include "cfg.hpp"
#include "log.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <grp.h>
#include <iostream>
#include <paths.h>
#include <pwd.h>
#include <unistd.h>

class SwitchUser
{
  public:
	SwitchUser(struct passwd *pw, Cfg &c, const std::string &display, char **_env);
	~SwitchUser();
	void Login(const char *cmd, const char *mcookie);

  private:
	SwitchUser();
	void SetEnvironment();
	void SetUserId();
	void Execute(const char *cmd);
	void SetClientAuth(const char *mcookie);
	Cfg &m_config_switchuser;
	struct passwd *Pw;
	std::string m_display_name;
	char **m_environment;
};

#endif /* _SWITCHUSER_H_ */
