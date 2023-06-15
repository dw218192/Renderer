#pragma once

#ifdef DOUBLE_PRECISION
using real = double;
#elif SINGLE_PRECISION
using real = float;
#else
#error "Must define either SINGLE_PRECISION or DOUBLE_PRECISION"
#endif

using uint = unsigned int;
using byte = unsigned char;