#include "Maps.h"
#include "p2Log.h"
#include "p2Defs.h"

#include "App.h"
#include "FileSystem.h"
#include "Textures.h"
#include "Render.h"

// TMX format from Tilled : http://doc.mapeditor.org/reference/tmx-map-format/

Maps::Maps() : Module()
{
	name.create("map");
	map_loaded = false;
}

Maps::~Maps()
{ }

bool Maps::awake(pugi::xml_node &node)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(node.child("folder").child_value());

	return ret;
}

void Maps::draw()
{
	if (map_loaded == false)
		return;

	// TODO 5(old): Prepare the loop to draw all tilesets + Blit
	MapLayer* layer = data.layers.getFirst()->data; // for now we just use the first layer and tileset
	TileSet* tileset = data.tilesets.getFirst()->data;
	SDL_Rect rect;

	switch (data.type)
	{
		case(MAPTYPE_ORTHOGONAL) :
		{
			uint k = 0;
			for (int i = 0; i < data.height; i++)
			{
				for (int j = 0; j < data.width; j++)
				{
					rect = tileset->getTileRect(layer->data[k++]);
					app->render->Blit(tileset->texture, j * tileset->tile_width, i * tileset->tile_height, &rect);
				}
			}
			break;
		}

		case(MAPTYPE_ISOMETRIC) :
		{
			uint k = 0;
			for (int i = 0; i < data.height; i++)
			{
				for (int j = 0; j < data.width; j++)
				{
					rect = tileset->getTileRect(layer->data[k++]);
					app->render->Blit(tileset->texture, (j-i) * (tileset->tile_width * 0.5f), (j+i) * (tileset->tile_height * 0.5f - tileset->offset_y), &rect);
				}
			}
			break;
		}
	}
		
	// TODO 10(old): Complete the draw function
}

iPoint Maps::mapToWorld(int x, int y) const
{
	iPoint ret(0, 0);
	// TODO 8(old): Create a method that translates x,y coordinates from map positions to world positions

	// TODO 1: Add isometric map to world coordinates
	return ret;
}

iPoint Maps::worldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	switch (data.type)
	{
	case(MAPTYPE_ORTHOGONAL) :
		// TODO 2: Add orthographic world to map coordinates
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case(MAPTYPE_ISOMETRIC) :
		// TODO 3: Add the case for isometric maps to WorldToMap
		ret.x = ((x / data.tile_width * 0.5f) + (y / data.tile_height * 0.5f )) / 2;
		ret.y = ((y / data.tile_height * 0.5f) - (x / data.tile_width * 0.5f)) / 2;
		break;
	}

	return ret;
}

SDL_Rect TileSet::getTileRect(int id) const
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	// TODO 7(old): Create a method that receives a tile id and returns it's Rect
	// CRZ al ataque!

	rect.w = tile_width;
	rect.h = tile_height;

	int index_X = (id - 1) % num_tiles_width;
	rect.x = index_X * tile_width + index_X * spacing + margin;
	int index_Y = (id - 1) / num_tiles_width;
	rect.y = index_Y * tile_height + index_Y * spacing + margin;

	return rect;
}

bool Maps::cleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	doubleNode<TileSet*>* item;
	item = data.tilesets.getFirst();

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	doubleNode<MapLayer*>* item2;
	item2 = data.layers.getFirst();

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

bool Maps::load(const char *file_name)
{
	bool ret = false;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char *buffer;
	uint size = app->fs->load(tmp.GetString(), &buffer);
	pugi::xml_parse_result result = map_file.load_buffer(buffer, size);

	RELEASE(buffer);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load General Info of Map
	ret = loadMap();

	// Load all tilesets info
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tilset"))
	{
		TileSet *set = new TileSet();

		if (ret != false)
			ret = loadTilesetDetails(tileset, set);

		if (ret != false)
			ret = loadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// Load layer info
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = loadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);
	}

	// Displaying information
	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		doubleNode<TileSet*>* item = data.tilesets.getFirst();
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		doubleNode<MapLayer*>* item_layer = data.layers.getFirst();
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool Maps::loadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");
	
	if (map == NULL)
	{
		ret = false;
		LOG("Error parsing map xml file: Cannot find 'map' tag");
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());
		
		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool Maps::loadTilesetDetails(pugi::xml_node &tileset_node, TileSet *set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool Maps::loadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool Maps::loadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}