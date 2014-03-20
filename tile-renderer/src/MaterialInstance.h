#pragma once

#include <cinder/Vector.h>
#include "Material.h"

using namespace ci;

class MaterialInstance {
public:
	MaterialInstance(Material *material = nullptr);
	~MaterialInstance();

	void setPosition(float x, float y);

	float getX();
	float getY();

	Material *getMaterial();

private:
	Material *materialPtr;
	Vec2f *position;
};