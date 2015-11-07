#include "PathFinding.h"
#include "Input.h"
#include "Maps.h"
#include "App.h"
#include "p2Log.h"

// Two interesting links about PathFinding
// Introduction: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Detailed: http://www.redblobgames.com/pathfinding/a-star/introduction.html

#define INVALID_WALKABILTY_CODE 255

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

bool PathFinding::checkBoundaries(const iPoint& pos) const
{
	if (pos.x >= 0 &&
		pos.x <= width &&
		pos.y >= 0 &&
		pos.y <= height)
		return true;
	return false;
}

bool PathFinding::isWalkable(const iPoint& pos) const
{
	if (checkBoundaries(pos))
		return (bool)map_data[pos.x * width + pos.y];
	return false;
}

uchar PathFinding::getTileAt(const iPoint& pos) const
{
	if (checkBoundaries(pos))
		return map_data[pos.x * width + pos.y];
	return INVALID_WALKABILTY_CODE;
}

//bool PathFinding::findPath(const iPoint &origin, const iPoint &destination)
//{
//	bool ret = false;
//
//	return ret;
//}
