#pragma once

#include <cinder/Vector.h>
#include <cinder/Camera.h>
#include <string>

using namespace ci;

class Camera2d {
public:
	Camera2d(float width, float height);
	~Camera2d(void);

	// Get camera.
	CameraOrtho getCameraOrtho();

	// Size set/get.
	void setSize(float width, float height);
	Vec2f getSize();

	// Position set/get.
	void setPosition(float x, float y);
	Vec2f getPosition();
	void addPosition(float x, float y);

	// Zoom set/get.
	void setZoom(float zoom);
	float getZoom();

	// Update matrix.
	void updateMatrix();

	// Get string of camera status (position, zoom, etc.).
	std::string getStateString();

private:
	CameraOrtho *cameraOrtho;

	Vec2f position, size;
	float zoom;
};

