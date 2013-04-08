/*
 * texture.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: aramadia
 */

#include "texture.hpp"

using namespace std;

Texture::Texture()
{

}

Texture::~Texture()
{
}



void Texture::loadFile(std::string filename) {
	if (!m_image.loadPng(filename))
	{
		cerr << "Failed to load texture " << filename << endl;
		exit(-1);
	}
}

// Map [u,v] of range [0,1] to [width, height]
// Does bilinear interpolation.  Also clamps
Colour Texture::read(double u, double v) const
{
	const int width = m_image.width();
	const int height = m_image.height();
//
//	u = abs(u);
//	v = abs(v);

	// clamp u,v to [0,1].  Interpolate between two points
	int umin = int(width * u);
	int umax = int(width * u) + 1;
	int vmin = int(height * v);
	int vmax = int(height *v) + 1;

	umin = min(max(umin, 0), width -1 );
	umax = min(max(umax, 0), width -1 );
	vmin = min(max(vmin, 0), height-1);
	vmax = min(max(vmax, 0), height - 1);

	// 4 samples on square
	Colour p0 = m_image(umin, vmin);
	Colour p1 = m_image(umin, vmax);
	Colour p2 = m_image(umax, vmin);
	Colour p3 = m_image(umax, vmax);

	double uD = width * u - umin;
	double vD = height * v - vmin;

	Colour finalColour = vD * (uD * p0 + (1.0 - uD) * p1) +	// top points
			//bottom points
			(1.0 - vD) * (uD * p2 + (1.0 - uD) * p3);

	//  do 4 point interpolation
	return finalColour;




}
