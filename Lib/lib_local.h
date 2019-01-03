/**
 * Trick to keep the contents of the String class outside of the
 * symbol table (ar index) of the compiled static library
 *
 * Basically, the contents of `Strings.cpp` is made local and static
 * to the current compile unit.
 *
 * This change was prompted due to usage of the same String class in
 * other static libraries.
 *
 * It's not possible to hide symbols at the linking stage; in `ar`,
 * adding symbols to the index is an all-or-nothing proposition.
 *
 * Note that any cpp-files added here ('Strings.cpp'), should NOT be
 * compiled separately in the build.
 */
#ifndef LIB_LOCAL_H
#define LIB_LOCAL_H

// includes required by Strings.cpp
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>


_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wunused-function\"") \

namespace {
#include "Support/Strings.cpp"
}

_Pragma("GCC diagnostic pop") \

#endif // LIB_LOCAL_H
