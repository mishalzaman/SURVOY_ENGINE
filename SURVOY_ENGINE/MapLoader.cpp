#include "MapLoader.h"
#include <fstream>
#include <iostream>

MapLoader::MapLoader()
{
}

bool MapLoader::Load(const std::string mapDataPath)
{
	std::ifstream jsonFile(mapDataPath);

    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        return false;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;
    jsonFile.close();

    _mapW = jsonData["width"];
    _mapH = jsonData["height"];
    _tileW = jsonData["tilewidth"];
    _tileH = jsonData["tileheight"];

    if (jsonData.find("layers") != jsonData.end()) {
        // Iterate over the "layers" array
        for (const auto& layer : jsonData["layers"]) {
            // Access elements within each layer as needed

            if (layer["name"] == "Graphic") {
                for (const auto& element : layer["data"]) {
                    // Convert each JSON element to an integer and push it into _graphic
                    _graphic.push_back(element);
                }
            }
        }
    }
    else {
        std::cerr << "No 'layers' key found in JSON data." << std::endl;
    }

    return true;
}
