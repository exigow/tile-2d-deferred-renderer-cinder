﻿#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/Vbo.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/Camera.h>

#include "Tile.h"

using namespace ci;
using namespace gl;

namespace tiler {
	class TileRenderer {
	public:
		TileRenderer(int width, int height, int tileSize);
		~TileRenderer();

		gl::Fbo gbuffer; // G-Buffer FBO.

		void captureStart(CameraOrtho cameraOrtho, GlslProg *gbufferShader);
		void captureEnd(GlslProg *gbufferShader);

		Tile getTile(int tileCount);
		Tile getTile(int tileWidth, int tileHeight);

		int getTileWidthCount();
		int getTileHeightCount();
		int getTileSize();

		std::string getStateString();

		// Tile mesh elements.
		GLfloat tileVerts[3 * 4];
		GLfloat tileUV[2 * 4];
		GLubyte tileIndices[4];

		float texelWidth, texelHeight;

		void setTileUV(float startX, float startY, float endX, float endY);

		void clearBuffers();

	private:
		gl::Fbo::Format gbufferFormat; // G-Buffer format.
		gl::Fbo::Format tileFormat;

		std::vector<Tile*> tiles; // Tile list.

		int tileTableWidth, tileTableHeight,
			tileSize;
	};
};