#pragma once

#include "../../lib/json/json.hpp"
#include <iostream>
#include <fstream>
#include <string>

inline nlohmann::json load_json(const std::string &filename)
{
    nlohmann::json jsonData;

    try
    {
        std::fstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        file >> jsonData;
        file.close();
        std::cout << "JSON data loaded successfully from " << filename << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return jsonData;
}