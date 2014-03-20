#include "Camera2d.h"

#include <cinder/Vector.h>
#include <string>
#include <cinder/Utilities.h>

Camera2d::Camera2d(float width, float height) {
	cameraOrtho = new CameraOrtho(0, 0, 128, 128, 0, 128);
	position = Vec2f::zero();
	size = Vec2f(width, height);
	zoom = 1;
}

Camera2d::~Camera2d(void) {
	delete cameraOrtho;
}

CameraOrtho Camera2d::getCameraOrtho() {
	return *cameraOrtho;
}

void Camera2d::setSize(float width, float height) {
	size.set(width, height);
}

void Camera2d::setPosition(float x, float y) {
	position.set(x, y);
}

void Camera2d::updateMatrix() {
	cameraOrtho->setOrtho(
		position.x - (size.x / 2.0f) * zoom, 
		position.x + (size.x / 2.0f) * zoom, 
		position.y + (size.y / 2.0f) * zoom, 
		position.y - (size.y / 2.0f) * zoom, 
		0, 128);
}

std::string Camera2d::getStateString() {
	return 
		"position (center): " + toString(position) + "\n" + 
		"zoom: " + toString(zoom) + "\n" +
		"size: " + toString(size) + "\n";
}

Vec2f Camera2d::getSize() {
	return size;
}

Vec2f Camera2d::getPosition() {
	return position;
}

void Camera2d::setZoom(float zoom) {
	this->zoom = zoom;
}

float Camera2d::getZoom() {
	return zoom;
}

void Camera2d::addPosition(float x, float y) {
	this->position.x += x;
	this->position.y += y;
}
