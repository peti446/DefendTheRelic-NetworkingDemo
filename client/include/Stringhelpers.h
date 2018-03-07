#ifndef STRINGHELPERS_H_INCLUDED
#define STRINGHELPERS_H_INCLUDED


#include <sstream>

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

#include "StringHelpers.inl"

#endif // STRINGHELPERS_H_INCLUDED
