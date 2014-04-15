#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include <cinder/Camera.h>
#include <cinder/gl/Texture.h>
#include <cinder/ImageIo.h>
#include <cinder/Utilities.h>
#include <cinder/Rand.h>
#include <cinder/gl/GlslProg.h>

#include <math.h>

#include "TileRendererInit.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace tiler;

class Test : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();

	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
	void mouseMove(MouseEvent event);
	void mouseWheel(MouseEvent event);

private:
	TileRenderer* renderer;
	Material* testMaterial;
	vector<MaterialInstance*> testMaterialInstalceList;

	Camera2dController* cameraController;
	Camera2d* camera;

	gl::GlslProg 
		*gbufferShader, 
		*pointLightShader,
		*defaultShader;

	Vec2f mousePos, mousePosNormal;
	Rectf *globalRect;

	float fps;
	float testLightSize;
};

void Test::prepareSettings(Settings *settings) {
	settings->setWindowSize(1280, 640);
	settings->setFrameRate(60.0f);
	settings->setTitle("tile-deferred-2d");
	settings->enableConsoleWindow();
}

void Test::setup() {
	globalRect = new Rectf();
	globalRect->set(0, 640, 1280, 0);


	testLightSize = 256;

	// Load shader.
	gbufferShader = new gl::GlslProg(loadAsset("gbuffer.vert"), loadAsset("gbuffer.frag")); 
	pointLightShader = new gl::GlslProg(loadAsset("pointLight.vert"), loadAsset("pointLight.frag")); 
	defaultShader = new gl::GlslProg(loadAsset("default.vert"), loadAsset("default.frag")); 
	// Renderer.
	renderer = new TileRenderer(1280, 640, 128);

	// Material.
	testMaterial = new Material();
	testMaterial->loadFromXml("testmat.xml");

	// Camera.
	camera = new Camera2d(1280, 640);
	cameraController = new Camera2dController(camera);

	// Test material instances
	MaterialInstance *_tmpInstance;
	#define MAX 1
	#define MSCALE 256.0f
	//for (int ix = -MAX; ix < MAX * 2; ix++) {
		//for (int iy = -MAX; iy < MAX * 2; iy++) {
			_tmpInstance = new MaterialInstance(testMaterial);
			//_tmpInstance->setPosition((float)ix * MSCALE, (float)iy * MSCALE);
			testMaterialInstalceList.push_back(_tmpInstance);
		//}
	//}
}

void Test::update() {
	cameraController->updateStep(1.0f / 60.0f);
	fps = getAverageFps();
}

void Test::draw() {
	gl::clear();

	// Update camera.
	camera->updateMatrix();
	
	renderer->clearBuffers();

	// Start capture.
	renderer->captureStart(camera->getCameraOrtho(), gbufferShader);

		MaterialInstance* _tmpInstance;
		for (size_t i = 0; i < testMaterialInstalceList.size(); i++) {
			// Get instance pointer.
			_tmpInstance = testMaterialInstalceList[i];

			// Bind textures.
			_tmpInstance->getMaterial()->diffuse.bind(0);
			_tmpInstance->getMaterial()->normal.bind(1);
			_tmpInstance->getMaterial()->specular.bind(2);

			// Translate (pos/rotation).
			gl::pushMatrices();
				gl::translate(_tmpInstance->getX(), _tmpInstance->getY());
				gl::draw(_tmpInstance->getMaterial()->diffuse);
			gl::popMatrices();
		}

	// End capture.
	renderer->captureEnd(gbufferShader);

	/*
	// Bind buffers.
	renderer->gbuffer.getTexture(0).bind(0);
	renderer->gbuffer.getTexture(1).bind(1);
	renderer->gbuffer.getTexture(2).bind(2);

	// Bind shader.
	pointLightShader->bind();
		
		// Set texture uniforms.
		pointLightShader->uniform("diffuseTex", 0);
		pointLightShader->uniform("normalTex", 1);
		pointLightShader->uniform("specularTex", 2);

		// Set var unif. 
		pointLightShader->uniform("lightPos", Vec2f(mousePosNormal.x, 1.0f - mousePosNormal.y));
		pointLightShader->uniform("lightPropertySize", testLightSize / globalRect->getWidth());
		pointLightShader->uniform("bufferRatio", globalRect->getAspectRatio());
		
		// Draw.
		gl::drawSolidRect(*globalRect);

	// Unbind shader.
	pointLightShader->unbind();
	*/

	// Fill test tile.
	/*renderer->gbuffer.getTexture(1).bind(0);
		defaultShader->bind();
			for (int ix = 0; ix < renderer->getTileWidthCount(); ix++) {
				for (int iy = 0; iy < renderer->getTileHeightCount(); iy++) {
					renderer->getTile(ix, iy).getBuffer().bindFramebuffer();
					defaultShader->uniform("texture", 0);
					gl::pushMatrices();
						gl::translate(Vec2f(0.0f, (float)renderer->gbuffer.getHeight()) - Vec2f(0.0f, (float)renderer->getTileSize()));
						// TODO Tu trzeba updejtowac uv-coordy do komorek.
						gl::drawSolidRect(*(renderer->tileRect));
					gl::popMatrices();
				}
			}
		defaultShader->unbind();
	renderer->getTile(0).getBuffer().unbindFramebuffer();*/

	// Draw tiles.
	renderer->drawTileTable(defaultShader);

	// Testing vbo!
	renderer->gbuffer.getTexture(1).bind(0);
	defaultShader->bind();
		defaultShader->uniform("texture", 0);
		
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
				glVertexPointer(3, GL_FLOAT, 0, renderer->tileVerts);
				glTexCoordPointer(2, GL_FLOAT, 0, renderer->tileUV);

					float 
						texelWidth = 1.0f / ((float)(renderer->getTileWidthCount())),
						texelHeight = 1.0f / ((float)(renderer->getTileHeightCount()));
					console() << toString(texelWidth) << endl;
					console() << toString(texelHeight) << endl;

					for (int ix = 0; ix < renderer->getTileWidthCount(); ix++) {
						for (int iy = 0; iy < renderer->getTileHeightCount(); iy++) {
							gl::pushMatrices();
								gl::translate((float)(ix * renderer->getTileSize()), (float)(iy * renderer->getTileSize()));
								gl::scale((float)renderer->getTileSize(), (float)renderer->getTileSize(), 1);
								//renderer->setTileUV(randFloat(), randFloat(), randFloat(), randFloat());

								renderer->setTileUV(
									texelWidth * (float)ix, 
									texelHeight * (float)iy,
									texelWidth * (float)ix + texelWidth, 
									texelHeight * (float)iy + texelWidth);

								glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, renderer->tileIndices);
							gl::popMatrices();
						}
					}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	defaultShader->unbind();

	// Draw text.
	gl::enableAlphaBlending();
	gl::drawString(
		"camera: " + camera->getStateString() + "\n" + 
		"renderer: " + renderer->getStateString() + "\n" +
		"fps: " + toString(fps) + "\n" +
		"mouse: " + toString(mousePos) + "\n" + 
		"mouseNormal: " + toString(mousePosNormal) + "\n" +
		"testLightSize: " + toString(testLightSize) + "\n",
		Vec2f(8.0f, 8.0f), Color::white(), Font("Calibri", 24.0f));
	gl::disableAlphaBlending();
}

void Test::keyDown(KeyEvent event) {
	cameraController->sendEventState(event, Camera2dController::EVENT_KEY_PRESS);
}
void Test::keyUp(KeyEvent event) {
	cameraController->sendEventState(event, Camera2dController::EVENT_KEY_RELEASE);
}
void Test::mouseMove(MouseEvent event) {
    mousePos = event.getPos();
	mousePosNormal = Vec2f((float)mousePos.x / 1280.0f, (float)mousePos.y / 640.0f);
}
void Test::mouseWheel(MouseEvent event) {
	testLightSize += event.getWheelIncrement() * 32;
}

CINDER_APP_BASIC(Test, RendererGl)
