#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>

#include "Tile.h"

using namespace ci;

namespace tiler {
	class TileRenderer {
	public:
		TileRenderer(int width, int height);
		~TileRenderer();

		gl::Fbo gbuffer; // G-Buffer FBO.

	private:
		gl::Fbo::Format gbufferFormat; // G-Buffer format.
		std::vector<Tile> tiles; // Tile list.
	};
};