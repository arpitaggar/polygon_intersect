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
    std::vector<float> x_coordinates, y_coordinates;

public:
    Polygon(std::vector<std::pair<float, float>> input_coordinates)
    {
        this->coordinates = input_coordinates;

        for (auto coordinate : this->coordinates)
        {
            x_coordinates.push_back(coordinate.first);
            y_coordinates.push_back(coordinate.second);
        }
    }

    Polygon operator-(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        new_coordinates.resize(std::max(this->coordinates.size(), obj.coordinates.size()));
        return Polygon(new_coordinates);
    }

    void print_coordinates()
    {
        for (auto coordinate : this->coordinates)
        {
            std::cout << coordinate.first << " " << coordinate.second << std::endl;
        }
    }
    std::vector<std::pair<float, float>> get_coordinates() const
    {
        return coordinates;
    }

    std::vector< float> get_x_coordinates() const
    {
        return this->x_coordinates;
    }
    std::vector< float> get_y_coordinates() const
    {
        return this->y_coordinates;
    }
};