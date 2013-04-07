/*
 * texture.hpp
 *
 *  Created on: Apr 7, 2013
 *      Author: aramadia
 */
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "image.hpp"

// Represents a texture
class Texture
{
public:
	Texture();
	virtual ~Texture();

	void loadFile(std::string filename);

	Colour read(double u, double v) const;

private:
	Image m_image;

};

#endif /* TEXTURE_H_ */
