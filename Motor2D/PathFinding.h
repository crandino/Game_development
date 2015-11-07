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

	DList<pathNode>    open_list;
	DList<pathNode>   close_list;

	DynArray<iPoint>  path_found;
	uchar*			    map_data;
	uint	       width, height;

public:

	PathFinding();
	~PathFinding();

	// Received all the info about the tiles and its walkability
	bool setMap(const uint &width, const uint &height, uchar *data);

	// CreatePath: Request to have a path from A to B
	int createPath(const iPoint& origin, const iPoint& destination);

		// GetLastPath: Returns order path step by step
	const DynArray<iPoint>* getLastPath() const;

	//Three utility methods :

	//CheckBoundaries: return true if pos is inside the map boundaries
	bool checkBoundaries(const iPoint& pos) const;

	// IsWalkable: returns true if the tile is walkable
	bool isWalkable(const iPoint& pos) const;

	//GetTileAt: return the walkability value of a tile
	uchar getTileAt(const iPoint& pos) const;

	//bool findPath(const iPoint &origin, const iPoint &destination);

	//bool start();
	//bool awake(pugi::xml_node &conf);
	//bool update(float dt);
	//bool cleanUp();
};

#endif // !__ PATHFINDING_H__
