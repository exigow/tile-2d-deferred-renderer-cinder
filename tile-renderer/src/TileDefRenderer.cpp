#include "TileDefRenderer.h"

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>

TileDefRenderer::TileDefRenderer(int width = 640, int height = 480) {
	gbufferFormat.setColorInternalFormat(GL_RGB8);
	gbufferFormat.setSamples(0);
	gbufferFormat.setCoverageSamples(0);
	gbufferFormat.enableColorBuffer(true, 3);

	gbuffer = gl::Fbo(width, height, gbufferFormat);
	gbuffer.getTexture(0).setFlipped(true);
	gbuffer.getTexture(1).setFlipped(true);
	gbuffer.getTexture(2).setFlipped(true);
}

TileDefRenderer::~TileDefRenderer() {
}