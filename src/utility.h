/**
 * \file utility.h
 * \author Guy Rutenberg
 *
 * Various utility functions and templates
 */

#ifndef UTILITY_INC_LD9EHZZAK8
#define UTILITY_INC_LD9EHZZAK8

#include <sstream>
#include <string>

template <class T> inline std::string stringify(T x)
{
	std::ostringstream o;
	o << x;
	return o.str();
}

#endif
