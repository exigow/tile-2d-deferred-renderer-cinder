#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include <cinder/Camera.h>
#include <cinder/MayaCamUI.h>
#include <cinder/gl/Texture.h>
#include <cinder/ImageIo.h>
#include <cinder/Utilities.h>
#include <cinder/Rand.h>
#include <cinder/gl/GlslProg.h>

#include <math.h>

#include "TileRenderer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Test : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);

private:
	TileDefRenderer* renderer;
	Material* testMaterial;
	vector<MaterialInstance*> testMaterialInstalceList;
	Camera2d* camera;
	gl::GlslProg* gbufferShader;

	bool moveTriggerUp, moveTriggerDown, moveTriggerLeft, moveTriggerRight;
};

void Test::prepareSettings(Settings *settings) {
	settings->setWindowSize(640, 480);
	settings->setFrameRate(60.0f);
	settings->setTitle("deferred-2d");
	settings->enableConsoleWindow();
}

void Test::setup() {
	moveTriggerUp = false;
	moveTriggerDown = false; 
	moveTriggerLeft = false;
	moveTriggerRight = false;

	// Load gbuffer shader.
	gbufferShader = new gl::GlslProg(loadAsset("gbuffer.vert"), loadAsset("gbuffer.frag")); 
	console() << gbufferShader << endl;

	// Renderer.
	renderer = new TileDefRenderer(640, 480);

	// Material.
	testMaterial = new Material();
	testMaterial->loadFromXml("testmat.xml");

	// Camera.
	camera = new Camera2d(640, 480);

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
	const float spd = 4.0f;
	if (moveTriggerUp) {
		camera->addPosition(0.0f, -spd);
	}
	if (moveTriggerDown) {
		camera->addPosition(0.0f, spd);
	}
	if (moveTriggerLeft) {
		camera->addPosition(-spd, 0.0f);
	}
	if (moveTriggerRight) {
		camera->addPosition(spd, 0.0f);
	}
}

void Test::draw() {
	// Clear background.
	gl::clear(Color(0.025f, 0.0f, 0.0f));

	// Camera update.
	//camera->setPosition((float)cos(getElapsedSeconds()) * 32.0f, (float)sin(getElapsedSeconds()) * 32.0f);
	//camera->setZoom(1.0f + (float)sin(getElapsedSeconds() * 1.25f) * 0.25f);
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
		gl::popMatrices();

		// Bind shader.
		gbufferShader->unbind();
	// Unbind.
	renderer->gbuffer.unbindFramebuffer();

	// Draw buffer on screen.
	gl::draw(renderer->gbuffer.getTexture(0));

	// Draw text.
	gl::enableAlphaBlending();
	gl::drawString(camera->getStateString(), Vec2f(8.0f, 8.0f), Color::white(), Font("Calibri", 16.0f));
	gl::disableAlphaBlending();
}


//bool moveTriggerUp, moveTriggerDown, moveTriggerLeft, moveTriggerRight;
void Test::keyDown(KeyEvent event) {
	char ch = event.getChar();
    if (ch == KeyEvent::KEY_w) {
		moveTriggerUp = true;
    }
	if (ch == KeyEvent::KEY_s) {
		moveTriggerDown = true;
    }
	if (ch == KeyEvent::KEY_a) {
		moveTriggerLeft = true;
    }
	if (ch == KeyEvent::KEY_d) {
		moveTriggerRight = true;
    }
}
void Test::keyUp(KeyEvent event) {
    char ch = event.getChar();
    if (ch == KeyEvent::KEY_w) {
		moveTriggerUp = false;
    }
	if (ch == KeyEvent::KEY_s) {
		moveTriggerDown = false;
    }
	if (ch == KeyEvent::KEY_a) {
		moveTriggerLeft = false;
    }
	if (ch == KeyEvent::KEY_d) {
		moveTriggerRight = false;
    }
}

CINDER_APP_BASIC(Test, RendererGl)
