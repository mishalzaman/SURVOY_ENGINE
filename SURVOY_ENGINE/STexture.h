#pragma once

#include <string>

namespace ENGINE {
	struct STexture {
		unsigned int id;
		std::string type;
		std::string path;
		int width;
		int height;
		int channel;
	};
}
