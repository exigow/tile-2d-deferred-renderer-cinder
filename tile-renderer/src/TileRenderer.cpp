#include "TileRenderer.h"

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/app/AppBasic.h>
#include <cinder/Utilities.h>
#include <cinder/Rand.h>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace tiler;

TileRenderer::TileRenderer(int width = 640, int height = 480, int tileSize = 32) {
	this->tileSize = tileSize;

	// Create fbo.
	gbufferFormat.setColorInternalFormat(GL_RGB8);
	gbufferFormat.setSamples(0);
	gbufferFormat.setCoverageSamples(0);
	gbufferFormat.enableColorBuffer(true, 3);
	gbuffer = gl::Fbo(width, height, gbufferFormat);

	tileFormat.setColorInternalFormat(GL_RGB8);
	tileFormat.setSamples(0);
	tileFormat.setCoverageSamples(0);

	Tile *_ptr;
	int ix, iy;
	for (ix = 0; ix < width; ix += tileSize) {
		for (iy = 0; iy < height; iy += tileSize) {
			_ptr = new Tile(tileSize, tileFormat);
			tiles.push_back(_ptr);

			_ptr->getBuffer().bindFramebuffer();
				gl::clear(ColorA(randFloat(), randFloat(), randFloat(), 1.0f));
			_ptr->getBuffer().unbindFramebuffer();
		}
	}

	tileTableWidth = width / tileSize;
	tileTableHeight = height / tileSize;

	tileRect = new Rectf();
	tileRect->set(0, (float)tileSize, (float)tileSize, 0);
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

Tile TileRenderer::getTile(int tileCount) {
	return *tiles[tileCount];
}

Tile TileRenderer::getTile(int tileWidth, int tileHeight) {
	return getTile(tileWidth + (tileHeight * getTileWidthCount()));
}

int TileRenderer::getTileWidthCount() {
	return tileTableWidth;
}

int TileRenderer::getTileHeightCount() {
	return tileTableHeight;
}

int TileRenderer::getTileSize() {
	return tileSize;
}

std::string TileRenderer::getStateString() {
	return 
		"size: " + toString(gbuffer.getWidth()) + ", " + toString(gbuffer.getHeight()) + "\n" +
		"tile size: " + toString(tileSize) + "\n" +
		"tiles count: " + toString(tileTableWidth) + ", " + toString(tileTableHeight) + "\n";
}

void TileRenderer::updateTilesFbos(GlslProg *indrawShader) {
	indrawShader->uniform("texture", 0);
	indrawShader->bind();
		for (int ix = 0; ix < getTileWidthCount(); ix++) {
			for (int iy = 0; iy < getTileHeightCount(); iy++) {
				getTile(ix + (iy * getTileWidthCount())).getBuffer().getTexture(0).bind(0);

				// Bind tile texture.
				//renderer->getTile(ix + (iy * renderer->getTileWidthCount())).getBuffer().getTexture(0).bind(0);

				// Compute position and size.
				/*float 
					_x = (float)ix * renderer->getTileSize(), 
					_y = (float)iy * renderer->getTileSize(),
					_s = (float)renderer->getTileSize();

				// Draw.
				/*gl::pushMatrices();
					gl::translate(_x, _y);
					gl::drawSolidRect(*(renderer->tileRect));
				gl::popMatrices();*/
			}
		}
	indrawShader->unbind();
}