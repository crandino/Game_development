#ifndef __PATHFINDING_H__
#define __PATHFINDING_H_

#include "Module.h"
#include "DList.h"
#include "DynArray.h"
#include "Point2d.h"

struct pathNode
{
	int x, y;    // Tile info
	int f, g, h; // Score, cost, estimation ("Manhattan distance")
	pathNode *parent;
};

class PathFinding : public Module
{

private:

	DList<pathNode>   open_list;
	DList<pathNode>   close_list;

	DynArray<iPoint> path_found;
	uchar*			   map_data;
	uint	      width, height;

public:

	PathFinding();
	~PathFinding();

	bool setMap(const uint &width, const uint &height, uchar *data);
	//bool findPath(const iPoint &origin, const iPoint &destination);

	//bool start();
	//bool awake(pugi::xml_node &conf);
	//bool update(float dt);
	//bool cleanUp();
};

#endif // !__ PATHFINDING_H__
