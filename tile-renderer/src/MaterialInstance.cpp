#include "MaterialInstance.h"

#include <cinder/Vector.h>

MaterialInstance::MaterialInstance(Material *material) {
	materialPtr = material;
	position = new Vec2f();
}

MaterialInstance::~MaterialInstance() {
}

void MaterialInstance::setPosition(float x, float y) {
	position->set(x, y);
}

Material *(MaterialInstance::getMaterial()) {
	return materialPtr;
}

float MaterialInstance::getX() {
	return position->x;
}
float MaterialInstance::getY() {
	return position->y;
}
