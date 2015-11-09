#ifndef __PATHFINDING_H__
#define __PATHFINDING_H_

#include "Module.h"
#include "DList.h"
#include "DynArray.h"
#include "Point2d.h"

struct pathList;

struct pathNode
{
	iPoint pos;  // Tile info
	int g, h;    // Score, cost
	const pathNode *parent;

	pathNode();
	pathNode(int score_g, int score_h, iPoint pos, const pathNode *parent_node);
	pathNode(const pathNode& node);

	// FindWalkableAdjacents: Fills a list of adjacent tiles that are walkable
	uint findWalkableAdjacents(pathList& list_to_fill) const;
	
	// Score: Basically returns g + h
	int score() const;
	
	// CalculateF : Recalculates F based on distance to destination
	// Estimation("Manhattan distance")
	int calculateF(const iPoint& destination);
};

struct pathList
{
	//It contains a linked list of PathNode(not PathNode*)
	DList<pathNode> list;

	//Find: Returns the node item if a certain node is in this list already(or NULL)
	doubleNode<pathNode> *find(const iPoint& point) const;

	//GetNodeLowestScore: Returns the Pathnode with lowest score in this list or NULL if empty
	doubleNode<pathNode> *getNodeLowestScore() const;
};

class PathFinding : public Module
{

private:

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
	const DynArray<iPoint> *getLastPath() const;

	//Three utility methods :

	//CheckBoundaries: return true if pos is inside the map boundaries
	bool checkBoundaries(const iPoint& pos) const;

	// IsWalkable: returns true if the tile is walkable
	bool isWalkable(const iPoint& pos) const;

	//GetTileAt: return the walkability value of a tile
	uchar getTileAt(const iPoint& pos) const;

	//bool start();
	//bool awake(pugi::xml_node &conf);
	//bool update(float dt);
	//bool cleanUp();
};

#endif // !__ PATHFINDING_H__
