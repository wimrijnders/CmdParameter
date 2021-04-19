#ifndef _CMDPARAMETER_SUPPORT_BASICS_H
#define _CMDPARAMETER_SUPPORT_BASICS_H
#include <string>

// Following aken from project V3DLib
#include "debug.h"

inline std::string &operator<<(std::string &a, char const *str)        { a += str; return a; }
inline std::string &operator<<(std::string &a, std::string const &str) { a += str; return a; }


inline std::string &operator<<(std::string &a, int val)      { a += std::to_string(val); return a; }
inline std::string &operator<<(std::string &a, long val)     { a += std::to_string(val); return a; }
inline std::string &operator<<(std::string &a, uint32_t val) { a += std::to_string(val); return a; }
inline std::string &operator<<(std::string &a, uint64_t val) { a += std::to_string(val); return a; }
inline std::string &operator<<(std::string &a, float val)    { a += std::to_string(val); return a; }


#endif  // _CMDPARAMETER_SUPPORT_BASICS_H
