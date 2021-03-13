#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <WinInet.h>

#include "constants.h"

inline void to_upper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) -> unsigned char { return std::toupper(c); });
}
