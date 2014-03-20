#pragma once

#include "Camera2d.h"
#include <cinder/app/AppBasic.h>
#include <cinder/app/Event.h>
#include <cinder/Vector.h>

using namespace ci;
using namespace ci::app;

class Camera2dController {
	//KeyEvent::KEY_w
public:
	Camera2dController(Camera2d* cameraPtr);
	~Camera2dController();

	Vec2f moveVec, preMoveVec, stepVec;

	//const int trigger_max = 3;
	bool trigger[3];
	Camera2d* camera;
	// {0 - up, 1 - down, 2 - left, 3- right}

	/*enum TriggerType {
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT
	};*/

	enum EventType {
		EVENT_KEY_PRESS,
		EVENT_KEY_RELEASE,
	};

	void sendEventState(KeyEvent event, EventType type);
	void updateStep(float deltaTime);
};

