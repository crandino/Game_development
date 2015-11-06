#include "PathFinding.h"
#include "Input.h"
#include "Maps.h"
#include "App.h"
#include "p2Log.h"

PathFinding::PathFinding()
{
	name.create("path_finding");
}

PathFinding::~PathFinding()
{ }

bool PathFinding::setMap(const uint &width, const uint &height, uchar *data)
{
	this->width = width;
	this->height = height;

	if (data != NULL)
	{
		RELEASE_ARRAY(map_data);
		map_data = new uchar[width * height];
		memcpy(map_data, data, width*height);
	}

	return true;
}

//bool PathFinding::findPath(const iPoint &origin, const iPoint &destination)
//{
//	bool ret = false;
//
//	return ret;
//}
