#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <random>

struct Room {
	int rows;
	int columns;
	int startRow;
	int startColumn;
	int quadRow;
	int quadColumn;
};

class MapGenerator
{
public:
	enum Tiles {
		FLOOR,
		EARTH,
		WALL
	};
	
	MapGenerator();

	void Create(
		int mapWidth,
		int mapHeight
	);

	std::vector<int>& GetMap() { return _map; }

private:
	std::vector<int> _map;
	std::vector<std::unique_ptr<Room>> _rooms;
	int _mapColumns;
	int _mapRows;

	int _minRoomColumns;
	int _maxRoomColumns;
	int _minRoomRows;
	int _maxRoomRows;
	int _maxRooms;

	void _createRoomsRandom();
	void _createRoomsConverge();
	void _createRoom(int row, int column, int rows, int columns);

	const int MIN_ROOM_SIZE = 5;
	const int MAX_ROOM_SIZE = 12;

	std::random_device _rd;
	std::mt19937 _gen;

	// helpers
	int _getIndex(int row, int column);
	int _randomRange(int start, int end);
};

