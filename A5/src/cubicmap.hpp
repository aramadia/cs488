/*
 * cubicmap.h
 *
 *  Created on: Apr 7, 2013
 *      Author: aramadia
 */

#ifndef CUBICMAP_H_
#define CUBICMAP_H_

#include <vector>
#include "texture.hpp"

class CubicMap
{
public:
	CubicMap();
	virtual ~CubicMap();

	// 6 faces of a cube
	enum CubeFaces {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK
	};

	// Determine the colour given an infinite ray hitting the cube
	Colour hit(const Ray &ray) const;
private:
	std::vector<Texture> m_textures;

};

#endif /* CUBICMAP_H_ */
