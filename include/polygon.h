// Class create a Polygon object using input coordinates.
#pragma once

#include "iostream"
#include "utility"
#include "vector"
#include "set"
#include "algorithm"

class Polygon
{
    std::vector<std::pair<float, float>> coordinates;
    std::set<float> x_coordinates, y_coordinates;

public:
    Polygon(std::vector<std::pair<float, float>> input_coordinates)
    {
        this->coordinates = input_coordinates;

        for (auto coordinate : this->coordinates)
        {
            x_coordinates.insert(coordinate.first);
            y_coordinates.insert(coordinate.second);
        }
    }

    Polygon operator-(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        new_coordinates.resize(std::max(this->coordinates.size(), obj.coordinates.size()));
    }

    void print_coordinates()
    {
        for (auto coordinate : this->coordinates)
        {
            std::cout << coordinate.first << " " << coordinate.second << std::endl;
        }
    }
};