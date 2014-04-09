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

	// Tile rectangle.
	tileRect = new Rectf();
	tileRect->set(0, (float)tileSize, (float)tileSize, 0);

	// Debug console info.
	console() << "width: " << this->gbuffer.getWidth() << endl << "height: " << this->gbuffer.getHeight() << endl;
	console() << "tile size: " << this->getTileSize() << endl;
	console() << "tile max W: " << this->getTileWidthCount() << endl << "tile max H: " << this->getTileHeightCount() << endl;

	// Create tile vbo.
	gl::VboMesh::Layout vboTileLayout;
	vboTileLayout.setStaticIndices();
	vboTileLayout.setStaticPositions();
	vboTileLayout.setDynamicTexCoords2d();
	vboTile = gl::VboMesh::create(
	4, 4, vboTileLayout, GL_QUADS);

	// Build indices.
	vector<uint32_t> vboTileIndices;
	vboTileIndices.push_back(0);
	vboTileIndices.push_back(1);
	vboTileIndices.push_back(2);
	vboTileIndices.push_back(3);
	vboTile->bufferIndices(vboTileIndices);

	// Build positions.
	vector<Vec3f> vboTilePositions;
	vboTilePositions.push_back(Vec3f(0, 0, 0));
	vboTilePositions.push_back(Vec3f((float)tileSize, 0, 0));
	vboTilePositions.push_back(Vec3f((float)tileSize, (float)tileSize, 0));
	vboTilePositions.push_back(Vec3f(0, (float)tileSize, 0));
	vboTile->bufferPositions(vboTilePositions);

	// Build uv (DEFAULT DYNAMIC).
	gl::VboMesh::VertexIter vboIter = vboTile->mapVertexBuffer();
	vboIter.setTexCoord2d0(Vec2f(0.0f, 1.0f)); ++vboIter;
	vboIter.setTexCoord2d0(Vec2f(1.0f, 1.0f)); ++vboIter;
	vboIter.setTexCoord2d0(Vec2f(1.0f, 0.0f)); ++vboIter;
	vboIter.setTexCoord2d0(Vec2f(0.0f, 0.0f));

	/*int i = 0;
	tileVerts[i + 0] = 0.0f; tileVerts[i + 1] = 0.0f; tileVerts[i + 2] = 0.0f; i++;
	tileVerts[i + 0] = 0.0f; tileVerts[i + 1] = 0.0f; tileVerts[i + 2] = 0.0f; i++;
	tileVerts[i + 0] = 0.0f; tileVerts[i + 1] = 0.0f; tileVerts[i + 2] = 0.0f; i++;
	tileVerts[i + 0] = 0.0f; tileVerts[i + 1] = 0.0f; tileVerts[i + 2] = 0.0f;*/

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

void TileRenderer::drawTileTable(GlslProg *shader) {
	// Bind shader.
	shader->bind();
		shader->uniform("texture", 0);
		for (int ix = 0; ix < getTileWidthCount(); ix++) {
			for (int iy = 0; iy < getTileHeightCount(); iy++) {
				// Bind tile texture.
				getTile(ix, iy).getBuffer().getTexture(0).bind(0);

				// Compute position and size.
				float 
					_x = (float)ix * getTileSize(), 
					_y = (float)iy * getTileSize(),
					_s = (float)getTileSize();

				// Draw.
				gl::pushMatrices();
					gl::translate(_x, _y);
					gl::drawSolidRect(*(tileRect));
				gl::popMatrices();
			}
		}
	shader->unbind();
}