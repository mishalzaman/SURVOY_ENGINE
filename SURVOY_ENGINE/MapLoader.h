#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class MapLoader
{
public:
	MapLoader();

	bool Load(const std::string mapDataPath);

	int GetMapWidth() const { return _mapW; }
	int GetMapHeight() const { return _mapH; }
	int GetTileWidth() const { return _tileW; }
	int GetTileHeight() const { return _tileH; }
	const std::vector<int>& GetGraphic() const { return _graphic; }

private:
	int _mapW;
	int _mapH;
	int _tileW;
	int _tileH;

	std::vector<int> _graphic;
};

