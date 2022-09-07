#include "OXI.h"
#include "lodepng.h"
#include <stdexcept>

void OXI_loadFile(const char* name, std::vector<byte_t> &out) {

#ifdef _TINSPIRE
	if (lodepng::load_file(out, std::string(name) + ".tns"))
#else
	if (lodepng::load_file(out, name))
#endif
		throw std::runtime_error(std::string("Unable to open ") + name);
}