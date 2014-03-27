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

private:
	TileRenderer* renderer;
	Material* testMaterial;
	vector<MaterialInstance*> testMaterialInstalceList;

	Camera2dController* cameraController;
	Camera2d* camera;

	gl::GlslProg 
		*gbufferShader, 
		*pointLightShader;

	Vec2f mousePos, mousePosNormal;
	Rectf *globalRect;

	float fps;
};

void Test::prepareSettings(Settings *settings) {
	settings->setWindowSize(640, 480);
	settings->setFrameRate(60.0f);
	settings->setTitle("tile-deferred-2d");
	settings->enableConsoleWindow();
}

void Test::setup() {
	globalRect = new Rectf();
	globalRect->set(0, 480, 640, 0);

	//gl::drawSolidRoundedRect(rect, 15.0);

	// Load shader.
	gbufferShader = new gl::GlslProg(loadAsset("gbuffer.vert"), loadAsset("gbuffer.frag")); 
	pointLightShader = new gl::GlslProg(loadAsset("pointLight.vert"), loadAsset("pointLight.frag")); 
	console() << " " << endl;

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
		pointLightShader->uniform("lightRadius", 256.0f);
		pointLightShader->uniform("bufferRatio", globalRect->getAspectRatio());
		pointLightShader->uniform("bufferSize", globalRect->getWidth());
		
		// Draw.
		gl::drawSolidRect(*globalRect);

	// Unbind shader.
	pointLightShader->unbind();

	// Draw text.
	gl::enableAlphaBlending();
	gl::drawString(
		"camera: " + camera->getStateString() + "\n" + 
		"fps: " + toString(fps) + "\n" +
		"mouse: " + toString(mousePos) + "\n" + 
		"mouseNormal: " + toString(mousePosNormal) + "\n", 
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
	mousePosNormal = Vec2f((float)mousePos.x / 640.0f, (float)mousePos.y / 480.0f);
}

CINDER_APP_BASIC(Test, RendererGl)
