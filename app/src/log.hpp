#ifndef _LOG_H_
#define _LOG_H_

#ifdef USE_CONSOLEKIT
#include "ck.hpp"
#endif
#ifdef USE_PAM
#include "pam.hpp"
#endif
#include "const.hpp"
#include <fstream>
#include <iostream>

static class LogUnit
{
	std::ofstream logFile;
	inline std::ostream &
	getStream ()
	{
		return logFile.is_open () ? logFile : std::cerr;
	}

public:
	bool openLog (const char * filename);
	void closeLog ();

	~LogUnit () { closeLog (); }

	template <typename Type>
	LogUnit &
	operator<< (const Type & text)
	{
		getStream () << text;
		getStream ().flush ();
		return *this;
	}

	LogUnit &
	operator<< (std::ostream & (*fp) (std::ostream &))
	{
		getStream () << fp;
		getStream ().flush ();
		return *this;
	}

	LogUnit &
	operator<< (std::ios_base & (*fp) (std::ios_base &))
	{
		getStream () << fp;
		getStream ().flush ();
		return *this;
	}
} logStream;

#endif /* _LOG_H_ */
