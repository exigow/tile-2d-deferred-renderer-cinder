#include "Tile.h"

#include <cinder/gl/Fbo.h>

using namespace tiler;

Tile::Tile(int tileSize, gl::Fbo::Format format) {
	fbo = gl::Fbo(tileSize, tileSize, format);
}

Tile::~Tile() {
}

gl::Fbo Tile::getBuffer() {
	return fbo;
}

Vec2i Tile::getSize() {
	return fbo.getSize();
}