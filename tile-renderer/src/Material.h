#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>
#include <cinder/Xml.h>

using namespace ci;

class Material {
public:
	Material();
	~Material();

	static enum TextureType {
		TEX_DIFFUSE = 0,
		TEX_NORMAL = 1,
		TEX_SPECULAR = 2
	};

	void loadFromXml(std::string filename);

	gl::Texture 
		diffuse, 
		normal, 
		specular;
private:
	//
};