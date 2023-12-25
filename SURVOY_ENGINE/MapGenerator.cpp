#include "MapGenerator.h"
#include <cmath>
#include <iostream>
#include <random>

MapGenerator::MapGenerator() : _rd(), _gen(_rd()), _mapColumns(0), _mapRows(0), _maxRooms(0)
{
}

void MapGenerator::Create(const int mapWidth, const int mapHeight)
{
	_mapColumns = mapWidth;
	_mapRows = mapHeight;

	_map.clear();
	_map.resize(mapWidth * mapHeight, Tiles::EARTH); // Initialize map with earth tiles

    _createRoomsConverge();
}

void MapGenerator::_createRoomsRandom() {
    _maxRooms = std::floor((_mapRows * _mapColumns) / (MAX_ROOM_SIZE * MAX_ROOM_SIZE));
    std::cout << "_maxRooms: " << _maxRooms << std::endl;

    for (int roomC = 0; roomC < _maxRooms; roomC++) {
        bool foundNonIntersect = false;
        int attemptCounter = 0;

        while (!foundNonIntersect && attemptCounter < 100) {
            int startRow = _randomRange(0, _mapRows - MAX_ROOM_SIZE);
            int startColumn = _randomRange(0, _mapColumns - MAX_ROOM_SIZE);

            int roomColumns = _randomRange(MIN_ROOM_SIZE, std::min(MAX_ROOM_SIZE, _mapColumns - startColumn));
            int roomRows = _randomRange(MIN_ROOM_SIZE, std::min(MAX_ROOM_SIZE, _mapRows - startRow));

            bool intersects = false;
            for (int i = 0; i < roomRows && !intersects; i++) {
                for (int j = 0; j < roomColumns && !intersects; j++) {
                    int index = _getIndex(startRow + i, startColumn + j);

                    if (_map[index] == Tiles::FLOOR || _map[index] == Tiles::WALL) {
                        intersects = true;
                    }
                }
            }

            if (!intersects) {
                _createRoom(startRow, startColumn, roomRows, roomColumns);
                foundNonIntersect = true;
            }
            else {
                ++attemptCounter;
            }
        }
    }
}

void MapGenerator::_createRoomsConverge() {
    _maxRooms = std::floor((_mapRows * _mapColumns) / (MIN_ROOM_SIZE * MIN_ROOM_SIZE));
    std::cout << "_maxRooms: " << _maxRooms << std::endl;

    // Create a central room
    int centralRow = _mapRows / 2 - MAX_ROOM_SIZE / 2;
    int centralColumn = _mapColumns / 2 - MAX_ROOM_SIZE / 2;
    _createRoom(centralRow, centralColumn, MAX_ROOM_SIZE, MAX_ROOM_SIZE);

    for (int roomC = 1; roomC < _maxRooms; roomC++) { // Start from 1 as central room is already created
        bool foundNonIntersect = false;
        int attemptCounter = 0;

        while (!foundNonIntersect && attemptCounter < 200) {
            // Generate rooms near the center
            int startRow = _randomRange(centralRow - MAX_ROOM_SIZE, centralRow + MAX_ROOM_SIZE);
            int startColumn = _randomRange(centralColumn - MAX_ROOM_SIZE, centralColumn + MAX_ROOM_SIZE);

            startRow = std::max(0, std::min(startRow, _mapRows - MAX_ROOM_SIZE));
            startColumn = std::max(0, std::min(startColumn, _mapColumns - MAX_ROOM_SIZE));

            int roomColumns = _randomRange(MIN_ROOM_SIZE, std::min(MAX_ROOM_SIZE, _mapColumns - startColumn));
            int roomRows = _randomRange(MIN_ROOM_SIZE, std::min(MAX_ROOM_SIZE, _mapRows - startRow));

            bool intersects = false;
            for (int i = 0; i < roomRows && !intersects; i++) {
                for (int j = 0; j < roomColumns && !intersects; j++) {
                    int index = _getIndex(startRow + i, startColumn + j);

                    if (_map[index] == Tiles::FLOOR || _map[index] == Tiles::WALL) {
                        intersects = true;
                    }
                }
            }

            if (!intersects) {
                _createRoom(startRow, startColumn, roomRows, roomColumns);
                foundNonIntersect = true;
            }
            else {
                ++attemptCounter;
            }
        }
    }
}


void MapGenerator::_createRoom(int startRow, int startColumn, int rows, int columns)
{
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < columns; ++col) {
			int index = _getIndex(startRow + row, startColumn + col);

            if (_map[index] == Tiles::WALL || _map[index] == Tiles::FLOOR) {
                std::cout << "what" << std::endl;
            }

			if (row == 0 || row == rows - 1 || col == 0 || col == columns - 1) {
				_map[index] = Tiles::WALL;
			}
			else {
				_map[index] = Tiles::FLOOR;
			}
		}
	}
}

int MapGenerator::_getIndex(int row, int column)
{
	return row * _mapColumns + column;
}

int MapGenerator::_randomRange(int start, int end)
{
	std::uniform_int_distribution<int> distribution(start, end - 1); // -1 to include the end in range
	return distribution(_gen);
}
