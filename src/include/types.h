#pragma once

#ifdef DOUBLE_PRECISION
using real = double;
#define REAL_LITERAL(x) x
#elif SINGLE_PRECISION
using real = float;
#define REAL_LITERAL(x) x##f
#else
#error "Must define either SINGLE_PRECISION or DOUBLE_PRECISION"
#endif

using uint = unsigned int;
using byte = unsigned char;