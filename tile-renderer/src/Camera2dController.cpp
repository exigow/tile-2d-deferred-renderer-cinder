#include "Camera2dController.h"
#include <cinder/app/Event.h>
#include <cinder/Vector.h>

using namespace ci;
using namespace ci::app;

Camera2dController::Camera2dController(Camera2d* cameraPtr) {
	for (int i = 0; i < 4; i++) {
		trigger[i] = false;
	}

	moveVec = Vec2f();
	preMoveVec = Vec2f();
	stepVec = Vec2f();

	camera = cameraPtr;
}

Camera2dController::~Camera2dController() {
}

void Camera2dController::sendEventState(KeyEvent event, EventType type) {
	bool _press;
	if (type == EVENT_KEY_PRESS) {
		_press = true;
	} else if (type == EVENT_KEY_RELEASE) {
		_press = false;
	}

	char _getChar = event.getChar();
	switch (_getChar) {
		case KeyEvent::KEY_w: {
			trigger[0] = _press;
			break;
		}
		case KeyEvent::KEY_s: {
			trigger[1] = _press;
			break;
		}
		case KeyEvent::KEY_a: {
			trigger[2] = _press;
			break;
		}
		case KeyEvent::KEY_d: {
			trigger[3] = _press;
			break;
		}
	}
}

void Camera2dController::updateStep(float deltaTime) {
	const float spd = 1.0f;
	const float spdsq2 = .7f;

	#define UP trigger[0]
	#define DOWN trigger[1]
	#define LEFT trigger[2]
	#define RIGHT trigger[3]

	preMoveVec.set(0.0f, 0.0f);
	if (UP) {
		preMoveVec.y = -1.0f;
	}
	if (DOWN) {
		preMoveVec.y = 1.0f;
	}
	if (LEFT) {
		preMoveVec.x = -1.0f;
	}
	if (RIGHT) {
		preMoveVec.x = 1.0f;
	}
	if (UP && RIGHT) {
		preMoveVec.x = .7f;
		preMoveVec.y = -.7f;
	}
	if (UP && LEFT) {
		preMoveVec.x = -.7f;
		preMoveVec.y = -.7f;
	}
	if (DOWN && RIGHT) {
		preMoveVec.x = .7f;
		preMoveVec.y = .7f;
	}
	if (DOWN && LEFT) {
		preMoveVec.x = -.7f;
		preMoveVec.y = .7f;
	}

	stepVec.x += preMoveVec.x;
	stepVec.y += preMoveVec.y;

	stepVec.x *= .875f;
	stepVec.y *= .875f;

	camera->addPosition(stepVec.x, stepVec.y);
}