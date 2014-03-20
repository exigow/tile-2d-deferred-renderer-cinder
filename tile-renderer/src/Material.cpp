#include "Material.h"

#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>
#include <cinder/Xml.h>
#include <cinder/Utilities.h>
#include <cinder/ImageIo.h>
#include <cinder/app/App.h>

using namespace ci;
using namespace ci::app;
using namespace std;

Material::Material() {
}

gl::Texture loadTextureFromMaterial(XmlTree *xmlSource, Material::TextureType type) {
	std::string str;
	switch(type) {
		case Material::TEX_DIFFUSE: {
			str = "diffuse";
			break;
		}
		case Material::TEX_NORMAL: {
			str = "normal";
			break;
		}
		case Material::TEX_SPECULAR: {
			str = "specular";
			break;
		}
	}
	return gl::Texture(loadImage(loadAsset(xmlSource->getChild("material/" + str).getValue())));
}

void Material::loadFromXml(std::string filename) {
	XmlTree *xml = new XmlTree(loadAsset(filename));
	this->diffuse = loadTextureFromMaterial(xml, TEX_DIFFUSE);
	this->normal = loadTextureFromMaterial(xml, TEX_NORMAL);
	this->specular = loadTextureFromMaterial(xml, TEX_SPECULAR);
	delete xml;


}

Material::~Material() {
}
