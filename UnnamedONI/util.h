#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>

//#define CLAMP(i, a, b) i > a ? (i < b ? i : b) : a
//#define MIN(a, b) a < b ? a : b
//#define MAX(a, b) a > b ? a : b
//#define IN_RANGE(i, a, b) i >= a && i <= b ? true : false
//#define SIGN(i) i < 0 ? -1 : (i > 0 ? 1 : 0)
//#define STRICT_SIGN(i) i < 0 ? -1 : 1

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

//float CLAMP(float a, float b, float c);

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

//float SIGN(float i);

//std::vector<std::string> split(std::string str,std::string sep);

//bool IN_RANGE()

#endif