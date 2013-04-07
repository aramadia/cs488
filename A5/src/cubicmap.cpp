/*
 * cubicmap.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: aramadia
 */

#include "debug.hpp"
#include "cubicmap.hpp"

using namespace std;



CubicMap::CubicMap()
{

	string folderPath = "data/";
	string names[] =  {"top", "bottom", "left", "right", "front", "back" };
	string ext = ".png";

	for (int i = 0; i < 6; i++)
	{
		string file = folderPath + names[i] + ext;
		STATUS_MSG("Loading texture" << file);
		Texture texture;
		texture.loadFile(file);
		m_textures.push_back(texture);
	}

}

CubicMap::~CubicMap()
{
	// TODO Auto-generated destructor stub
}


Colour CubicMap::hit(Ray ray) const
{
	// fix the coordinate system
	//ray.dir[1] *= -1;

	Colour outputColour;

	double ax = abs(ray.dir[0]);
	double ay = abs(ray.dir[1]);
	double az = abs(ray.dir[2]);

	CubeFaces face;
	double u,v;

	if (ax >= ay && ax >= az)
	{
		if (ray.dir[0] > 0)
		{
			// right,  z/x, y/x + 1
			face = RIGHT;
			u = 1.0 - (ray.dir[2]/ray.dir[0] + 1.0) * 0.5;
			v = (ray.dir[1]/ray.dir[0] + 1.0) * 0.5;
		}
		else
		{
			// left, z/x,
			face = LEFT;
			u = 1.0 - (ray.dir[2]/ray.dir[0] + 1.0) * 0.5;
			v = 1.0 - (ray.dir[1]/ray.dir[0] + 1.0) * 0.5;
		}
	}
	else if (ay >= ax && ay >= az)
	{
		if (ray.dir[1] > 0)
		{
			// top
			face = TOP;
			u = (ray.dir[0]/ray.dir[1] + 1.0) * 0.5;
			v = 1.0 - (ray.dir[2]/ray.dir[1] + 1.0) * 0.5;
		}
		else
		{
			//bottom
			face = BOTTOM;
			u = 1.0 - (ray.dir[0]/ray.dir[1] + 1.0) * 0.5;
			v = (ray.dir[2]/ray.dir[1] + 1.0) * 0.5;
		}
	}
	else /*if (az >= ax && az >= ay)*/
	{
		if (ray.dir[2] > 0)
		{
			// front
			face = FRONT;
			u = (ray.dir[0]/ray.dir[2] + 1.0) * 0.5;
			v = (ray.dir[1]/ray.dir[2] + 1.0) * 0.5;
		}
		else
		{
			// back
			face = BACK;
			u = (ray.dir[0]/ray.dir[2] + 1.0) * 0.5;
			v = 1.0 - (ray.dir[1]/ray.dir[2] + 1.0) * 0.5;
		}
	}

	// transform colour
	const Texture &t =  m_textures[face];
	u = 1.0 - u;
	v = 1.0 - v;
	outputColour = t.read(u, v);

	// fix gamma, exposure

	return outputColour;


}
