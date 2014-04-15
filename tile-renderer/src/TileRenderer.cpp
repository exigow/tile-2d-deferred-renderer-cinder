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
	this->tileTableWidth = width / tileSize;
	this->tileTableHeight = height / tileSize;

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

			// Fill fbo (default color).
			_ptr->getBuffer().bindFramebuffer();
				gl::clear(ColorA(.125, 0.0f, .125, 1.0f));
			_ptr->getBuffer().unbindFramebuffer();
		}
	}

	// Debug console info.
	console() << "width: " << this->gbuffer.getWidth() << endl << "height: " << this->gbuffer.getHeight() << endl;
	console() << "tile size: " << this->getTileSize() << endl;
	console() << "tile max W: " << this->getTileWidthCount() << endl << "tile max H: " << this->getTileHeightCount() << endl;

	texelWidth = 1.0f / ((float)(this->tileTableWidth));
	texelHeight = 1.0f / ((float)(this->tileTableHeight));

	// Vertex.
	tileVerts[0] = 0.0f;
	tileVerts[1] = 0.0f;
	tileVerts[2] = 0.0f;

	tileVerts[3] = 1.0f; 
	tileVerts[4] = 0.0f; 
	tileVerts[5] = 0.0f;

	tileVerts[6] = 0.0f; 
	tileVerts[7] = 1.0f; 
	tileVerts[8] = 0.0f;

	tileVerts[9] = 1.0f;
	tileVerts[10] = 1.0f; 
	tileVerts[11] = 0.0f;

	// Indices.
	tileIndices[0] = 0;
	tileIndices[1] = 1;
	tileIndices[2] = 2;

	tileIndices[3] = 1;
	tileIndices[4] = 2;
	tileIndices[5] = 3;

	// UVs.
	setTileUV(0.0f, 0.0f, 1.0f, 1.0f);
}

void TileRenderer::clearBuffers() {
	gbuffer.bindFramebuffer();
		gl::clear(ColorA(.25f, 0.0f, .225, 1.0f));
	gbuffer.unbindFramebuffer();
}

TileRenderer::~TileRenderer() {
}

void TileRenderer::setTileUV(float startX, float startY, float endX, float endY) {
	tileUV[4] = startX;
	tileUV[5] = startY;

	tileUV[6] = endX;
	tileUV[7] = startY;

	tileUV[0] = startX;
	tileUV[1] = endY;

	tileUV[2] = endX;
	tileUV[3] = endY;
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











//