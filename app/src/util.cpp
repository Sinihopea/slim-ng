/*
 * SLiM - Simple Login Manager
 * Copyright (C) 2009 Eygene Ryabinkin <rea@codelabs.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "util.hpp"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

/*
 * Adds the given cookie to the specified Xauthority file.
 * Returns true on success, false on fault.
 */
bool Util::add_mcookie(const std::string &mcookie, const char *display,
	const std::string &xauth_cmd, const std::string &authfile)
{
	FILE *fp;
	std::string cmd = xauth_cmd + " -f " + authfile + " -q";
	fp = popen(cmd.c_str(), "w");

	if (!fp)
	{
		return false;
	}

	std::fprintf(fp, "remove %s\n", display);
	std::fprintf(fp, "add %s %s %s\n", display, ".", mcookie.c_str());
	std::fprintf(fp, "exit\n");
	pclose(fp);

	return true;
}

/*
 * Interface for random number generator.  Just now it uses ordinary
 * random/srandom routines and serves as a wrapper for them.
 */
void Util::srandom(unsigned long seed)
{
	::srandom(seed);
}

long Util::random(void)
{
	return ::random();
}

/*
 * Makes seed for the srandom() using "random" values obtained from
 * getpid(), time(nullptr) and others.
 */
long Util::makeseed(void)
{
	struct timespec ts;
	long pid = getpid();
	long tm = time(nullptr);

	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
	{
		ts.tv_sec = ts.tv_nsec = 0;
	}

	return pid + tm + (ts.tv_sec ^ ts.tv_nsec);
}
