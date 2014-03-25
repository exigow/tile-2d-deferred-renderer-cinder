#pragma once

#include <cinder/gl/Fbo.h>

using namespace ci;

namespace tiler {
	class Tile {
	public:
		Tile();
		~Tile();
	private:
		gl::Fbo::Format fbo;
	};
}