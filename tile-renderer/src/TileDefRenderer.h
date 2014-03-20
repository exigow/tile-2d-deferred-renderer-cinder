#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>

using namespace ci;

class TileDefRenderer {
public:
	TileDefRenderer(int width, int height);
	~TileDefRenderer();

	Vec2i getFBOSize();

	gl::Fbo gbuffer;

private:
	gl::Fbo::Format gbufferFormat;
};