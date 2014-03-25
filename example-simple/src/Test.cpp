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

private:
	TileRenderer* renderer;
	Material* testMaterial;
	vector<MaterialInstance*> testMaterialInstalceList;

	Camera2dController* cameraController;
	Camera2d* camera;

	gl::GlslProg* gbufferShader;

	float fps;
};

void Test::prepareSettings(Settings *settings) {
	settings->setWindowSize(640, 480);
	settings->setFrameRate(60.0f);
	settings->setTitle("tile-deferred-2d");
	settings->enableConsoleWindow();
}

void Test::setup() {
	// Load gbuffer shader.
	gbufferShader = new gl::GlslProg(loadAsset("gbuffer.vert"), loadAsset("gbuffer.frag")); 
	//gbufferShader = new gl::GlslProg(loadAsset("gbuffer.vert"), loadAsset("gbuffer.frag")); 
	//console() << gbufferShader << endl;

	// Renderer.
	renderer = new TileRenderer(640, 480);

	// Material.
	testMaterial = new Material();
	testMaterial->loadFromXml("testmat.xml");

	// Camera.
	camera = new Camera2d(640, 480);
	cameraController = new Camera2dController(camera);

	// Test material instances
	MaterialInstance *_tmpInstance;
	#define MAX 1
	#define MSCALE 256.0f
	for (int ix = -MAX; ix < MAX * 2; ix++) {
		for (int iy = -MAX; iy < MAX * 2; iy++) {
			_tmpInstance = new MaterialInstance(testMaterial);
			_tmpInstance->setPosition((float)ix * MSCALE, (float)iy * MSCALE);
			testMaterialInstalceList.push_back(_tmpInstance);
		}
	}
}

void Test::update() {
	cameraController->updateStep(1.0f / 60.0f);
	fps = getAverageFps();
}

void Test::draw() {
	// Update camera.
	camera->updateMatrix();

	// Bind draw to buffer.
	renderer->gbuffer.bindFramebuffer();
		// Bind shader.
		gbufferShader->bind();

		gbufferShader->uniform("diffuseTex", 0);
		gbufferShader->uniform("normalTex", 1);
		gbufferShader->uniform("specularTex", 2);

		// Push camera matrix.
		gl::pushMatrices();
		gl::setMatrices(camera->getCameraOrtho());
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
		// Pop camera matrix.
		gl::popMatrices();

		// Bind shader.
		gbufferShader->unbind();
	// Unbind.
	renderer->gbuffer.unbindFramebuffer();

	// Draw buffer on screen.
	gl::draw(renderer->gbuffer.getTexture(1));

	// Draw text.
	gl::enableAlphaBlending();
	gl::drawString(
		camera->getStateString() + "\n" + 
		" fps: " + toString(fps) + "\n", 
		Vec2f(8.0f, 8.0f), Color::white(), Font("Calibri", 24.0f));
	gl::disableAlphaBlending();
}

void Test::keyDown(KeyEvent event) {
	cameraController->sendEventState(event, Camera2dController::EVENT_KEY_PRESS);
}
void Test::keyUp(KeyEvent event) {
	cameraController->sendEventState(event, Camera2dController::EVENT_KEY_RELEASE);
}

CINDER_APP_BASIC(Test, RendererGl)
