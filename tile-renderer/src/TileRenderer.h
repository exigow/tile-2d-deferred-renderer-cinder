#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/Camera.h>

#include "Tile.h"

using namespace ci;
using namespace gl;

namespace tiler {
	class TileRenderer {
	public:
		TileRenderer(int width, int height);
		~TileRenderer();

		gl::Fbo gbuffer; // G-Buffer FBO.

		void captureStart(CameraOrtho cameraOrtho, GlslProg *gbufferShader);
		void captureEnd(GlslProg *gbufferShader);

	private:
		gl::Fbo::Format gbufferFormat; // G-Buffer format.
		std::vector<Tile> tiles; // Tile list.
	};
};