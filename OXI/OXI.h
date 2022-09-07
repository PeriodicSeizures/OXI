#pragma once

#include <vector>
#include "fix.h"

using byte_t = uint8_t;
using FFix = Fix<8>;
using DFix = Fix<12>;

void OXI_loadFile(const char* name, std::vector<byte_t>& out);