#include "log.hpp"
#include <cstring>
#include <iostream>

bool
LogUnit::openLog (const char *filename)
{
	if (logFile.is_open ())
		{
			std::cerr
				<< APPNAME
				<< ": opening a new Log file, while another is already open"
				<< std::endl;
			logFile.close ();
		}

	// cerr is the default
	if (strcmp (filename, "/dev/stderr") == 0)
		return true;

	logFile.open (filename, std::ios_base::app);
	return !(logFile.fail ());
}

void
LogUnit::closeLog ()
{
	if (logFile.is_open ())
		logFile.close ();
}
