#include "TileRenderer.h"

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>

using namespace tiler;

TileRenderer::TileRenderer(int width = 640, int height = 480) {
	gbufferFormat.setColorInternalFormat(GL_RGB8);
	gbufferFormat.setSamples(0);
	gbufferFormat.setCoverageSamples(0);
	gbufferFormat.enableColorBuffer(true, 3);

	gbuffer = gl::Fbo(width, height, gbufferFormat);
	//gbuffer.getTexture(0).setFlipped(true);
	//gbuffer.getTexture(1).setFlipped(true);
	//gbuffer.getTexture(2).setFlipped(true);
}

TileRenderer::~TileRenderer() {
}

void TileRenderer::captureStart(CameraOrtho cameraOrtho, ci::gl::GlslProg *gbufferShader) {
	// Bind draw to buffer.
	gbuffer.bindFramebuffer();

	// Bind shader.
	gbufferShader->bind();

	// Set uniform textures.
	gbufferShader->uniform("diffuseTex", 0);
	gbufferShader->uniform("normalTex", 1);
	gbufferShader->uniform("specularTex", 2);

	// Push camera matrix.
	gl::pushMatrices();
	gl::setMatrices(cameraOrtho);
}

void TileRenderer::captureEnd(ci::gl::GlslProg *gbufferShader) {
	// Pop camera matrix.
	gl::popMatrices();

	// Bind shader.
	gbufferShader->unbind();

	// Unbind.
	gbuffer.unbindFramebuffer();
}

