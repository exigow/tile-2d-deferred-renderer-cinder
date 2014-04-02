#pragma once

#include <cinder/gl/Fbo.h>

using namespace ci;

namespace tiler {
	class Tile {
	public:
		Tile(int tileSize, gl::Fbo::Format format);
		~Tile();
		gl::Fbo getBuffer();
		Vec2i getSize();
		
	private:
		gl::Fbo fbo;
	};
}