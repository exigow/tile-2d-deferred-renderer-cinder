﻿#include "Camera2dController.h"
#include <cinder/app/Event.h>
#include <cinder/Vector.h>

using namespace ci;
using namespace ci::app;

Camera2dController::Camera2dController(Camera2d* cameraPtr) {
	for (int i = 0; i < 32; i++) {
		trigger[i] = false;
	}

	camera = cameraPtr;

	moveTargetVec = Vec2f(cameraPtr->getPosition());
	moveVec = Vec2f(moveTargetVec);
	zoomTarget = cameraPtr->getZoom();
	zoom = zoomTarget;
}

Camera2dController::~Camera2dController() {
}

void Camera2dController::sendEventState(KeyEvent event, EventMode mode) {
	bool _press;
	if (mode == EVENT_KEY_PRESS) {
		_press = true;
	} else if (mode == EVENT_KEY_RELEASE) {
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
		case KeyEvent::KEY_q: {
			trigger[4] = _press;
			break;
		}
		case KeyEvent::KEY_e: {
			trigger[5] = _press;
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
	#define ZOOMIN trigger[4]
	#define ZOOMOUT trigger[5]

	float _x = 0.0f, _y = 0.0f;
	if (UP) {
		_y = -1.0f;
	}
	if (DOWN) {
		_y = 1.0f;
	}
	if (LEFT) {
		_x = -1.0f;
	}
	if (RIGHT) {
		_x = 1.0f;
	}
	if (UP && RIGHT) {
		_x = .7f;
		_y = -.7f;
	}
	if (UP && LEFT) {
		_x = -.7f;
		_y = -.7f;
	}
	if (DOWN && RIGHT) {
		_x = .7f;
		_y = .7f;
	}
	if (DOWN && LEFT) {
		_x = -.7f;
		_y = .7f;
	}

	if (ZOOMOUT) {
		zoomTarget -= 0.0125f;
	}
	if (ZOOMIN) {
		zoomTarget += 0.0125f;
	}

	zoom += (zoomTarget - zoom) * .125f;

	moveTargetVec.x += _x * deltaTime * 192.0f;
	moveTargetVec.y += _y * deltaTime * 192.0f;

	moveVec.x += (moveTargetVec.x - moveVec.x) * .125f;
	moveVec.y += (moveTargetVec.y - moveVec.y) * .125f;

	camera->setPosition(moveVec.x, moveVec.y);
	camera->setZoom(zoom);
}