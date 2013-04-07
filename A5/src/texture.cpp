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

	Colour cmin = m_image(umin, umin);
	Colour cmax = m_image(umax, vmax);

	double uD = width * u - umin;
	double vD = height * v - vmin;

	// TODO, do 4 point interpolation
	return cmin;




}
