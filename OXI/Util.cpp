#include "Util.h"
#include "lodepng.h"
#include <stdexcept>
#include <memory>
#include <unordered_set>

void DD_loadFile(const char* name, std::vector<byte_t> &out) {

#ifdef _TINSPIRE
	if (lodepng::load_file(out, std::string(name) + ".tns"))
#else
	if (lodepng::load_file(out, name))
#endif
		throw std::runtime_error(std::string("Unable to open ") + name);
}


// transparency/alpha is not supported
std::unique_ptr<Texture> DD_loadTexture(const char* name) {
	std::vector<byte_t> buffer;

	DD_loadFile(name, buffer);

	std::vector<unsigned char> pixels;
	unsigned int w, h;
	lodepng::State state;
	if (lodepng::decode(pixels, w, h, state, buffer))
		throw std::runtime_error("Failed to decode texture");

	if (w > UCHAR_MAX || h > UCHAR_MAX)
		throw std::runtime_error("Texture dimesions are too large");



	auto tex = std::unique_ptr<Texture>(new Texture{
		static_cast<byte_t>(w),
		static_cast<byte_t>(h),
		std::unique_ptr<color_t>(new color_t[static_cast<uint64_t>(w * h)]),
		false,
		0 });

	// transparency detection
	{
		std::unordered_set<color_t> colors;
		for (int i = 0; i < pixels.size() / 4; i++) {

			// if transparent
			if (pixels[i * 4 + 3] == 0) {
				tex->m_useTransparency = true;
			}
			else {
				uint64_t rgb888 = (pixels[i * 4] << 16) | (pixels[i * 4 + 1] << 8) | (pixels[i * 4 + 2]);
				auto rgb565 = COLOR_888_565(rgb888);

				colors.insert(rgb565);
			}
		}

		// iterate unused for transparent candidate
		if (tex->m_useTransparency) {
			for (color_t c = 0; c < std::numeric_limits<color_t>::max(); c++) {
				if (colors.find(c) == colors.end()) {
					tex->m_transparent = c;
					break;
				}
			}
		}
	}



	for (int i = 0; i < pixels.size() / 4; i++) {
		// adds used colors for later transparent decision
		if (pixels[i * 4 + 3] == 0) {
			tex->m_pixels.get()[i] = tex->m_transparent;
		}
		else {
			uint64_t rgb888 = (pixels[i * 4] << 16) | (pixels[i * 4 + 1] << 8) | (pixels[i * 4 + 2]);
			auto rgb565 = COLOR_888_565(rgb888);

			tex->m_pixels.get()[i] = rgb565;
		}
	}

	return tex;

	//return std::unique_ptr<Texture>(
//		new Texture{static_cast<byte_t>(w), static_cast<byte_t>(h), std::move(px), false, 0});

}