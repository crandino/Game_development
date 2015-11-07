#include "PathFinding.h"
#include "Input.h"
#include "Maps.h"
#include "App.h"
#include "p2Log.h"

// Two interesting links about PathFinding
// Introduction: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Detailed: http://www.redblobgames.com/pathfinding/a-star/introduction.html

#define INVALID_WALKABILTY_CODE 255

pathNode::pathNode() : g(-1), h(-1), x(-1), y(-1), parent(NULL) { }
pathNode::pathNode(int g_score, int h_score, int posx, int posy, const pathNode *parent_node) : g(g_score), h(h_score), x(posx), y(posy), parent(parent) { }
pathNode::pathNode(const pathNode &node) : g(node.g), h(node.h), x(node.x), y(node.y), parent(node.parent)  { }

uint pathNode::findWalkableAdjacents(pathList& list_to_fill) const
{
	uint items_before = list_to_fill.list.count();
	iPoint new_pos;
	uint items_added;

	//Nodes-- > East, South, West, North;
	new_pos.set(x + 1, y);
	if (app->path->isWalkable(new_pos))
	{
		pathNode node_E(-1, -1, new_pos.x, new_pos.y, this);
		items_added++;
		list_to_fill.list.add(node_E);
	}

	new_pos.set(x, y + 1);
	if (app->path->isWalkable(new_pos))
	{
		pathNode node_S(-1, -1, new_pos.x, new_pos.y, this);
		items_added++;
		list_to_fill.list.add(node_S);
	}

	new_pos.set(x - 1, y);
	if (app->path->isWalkable(new_pos))
	{
		pathNode node_W(-1, -1, new_pos.x, new_pos.y, this);
		items_added++;
		list_to_fill.list.add(node_W);
	}

	new_pos.set(x, y - 1);
	if (app->path->isWalkable(new_pos))
	{
		pathNode node_N(-1, -1, new_pos.x, new_pos.y, this);
		items_added++;
		list_to_fill.list.add(node_N);
	}

	return items_added - items_before;
}

int pathNode::score() const
{
	return g + h;
}

int pathNode::calculateF(const iPoint& destination)
{
	g = parent->g + 1;
	iPoint pos(x, y);
	h = pos.distanceTo(destination);
	//abs((destination.x - x) + (destination.y - y));
	return g + h;
}

doubleNode<pathNode> *pathList::find(const iPoint& point) const
{
	doubleNode<pathNode> *item = list.getFirst();
	while (item != NULL)
	{
		if (item->data.x == point.x && item->data.y == point.y)
			return item;
		item = item->next;
	}
	return item;
}

doubleNode<pathNode> *pathList::getNodeLowestScore() const
{
	doubleNode<pathNode> *item = list.getFirst();
	doubleNode<pathNode> *item_lowest_score = NULL;
	int score;

	if (item != NULL)
		score = item->data.score();
	else
		return item_lowest_score;

	while (item != NULL)
	{
		if (item->data.score() < score)
		{
			score = item->data.score();
			item_lowest_score = item;
		}
		item = item->next;
	}

	return item_lowest_score;
}

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

//FindWalkableAdjacents: Fills a list of adjacent tiles that are walkable
uint pathNode::findWalkableAdjacents(pathList& list_to_fill) const
{
	iPoint origin(x, y);
	
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
