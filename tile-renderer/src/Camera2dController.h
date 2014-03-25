#pragma once

#include "Camera2d.h"
#include <cinder/app/AppBasic.h>
#include <cinder/app/Event.h>
#include <cinder/Vector.h>

using namespace ci;
using namespace ci::app;

class Camera2dController {
public:
	Camera2dController(Camera2d* cameraPtr);
	~Camera2dController();

	Vec2f moveVec, moveTargetVec;
	float zoom, zoomTarget;
	
	bool trigger[32];

	Camera2d* camera;

	enum EventMode {
		EVENT_KEY_PRESS = 0,
		EVENT_KEY_RELEASE = 1,
	};

	void sendEventState(KeyEvent event, EventMode mode);
	void updateStep(float deltaTime);
};

