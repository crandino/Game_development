#ifndef __MAPS_H__
#define __MAPS_H__

#include "Module.h"
#include "DList.h"
#include "Point2d.h"
#include "SDL\include\SDL.h"

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct TileSet
{
	SDL_Rect getTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture			*texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct MapLayer
{
	p2SString		name;
	int				width;
	int				height;
	uint			*data;

	MapLayer() : data(NULL) {}
	~MapLayer() { RELEASE(data); }

	inline uint get(int x, int y) const
	{
		return 0;
	}
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	DList<TileSet*>		tilesets;
	DList<MapLayer*>	layers;
};

class Maps : public Module
{
private:

	pugi::xml_document		map_file;
	p2SString				folder;
	bool					map_loaded;

	bool loadMap();
	bool loadTilesetDetails(pugi::xml_node &tileset_node, TileSet *set);
	bool loadTilesetImage(pugi::xml_node &tileset_node, TileSet *set);
	bool loadLayer(pugi::xml_node &node, MapLayer *layer);
	
public:

	MapData	data;

	Maps();
	~Maps();

	bool awake(pugi::xml_node&);
	void draw();
	bool cleanUp();

	bool load(const char *path);

	iPoint mapToWorld(int x, int y) const;
	iPoint worldToMap(int x, int y) const;
	
};

#endif //!__MAPS_H__