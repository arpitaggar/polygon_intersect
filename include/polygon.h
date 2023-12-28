// Class create a Polygon object using input coordinates.
#pragma once

#include "iostream"
#include "utility"
#include "vector"
#include "set"
#include "algorithm"
#include "iomanip"


class Polygon
{
    std::vector<std::pair<float, float>> coordinates;
    std::vector<float> x_coordinates, y_coordinates;

    // This creates a vector of line_segments in the polygon. Each segement consists of a
    // starting and ending coordiate (as std::pair) which in turn contains the x & y coordinates as another pair.

    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> line_segments;

public:
    Polygon(std::vector<std::pair<float, float>> input_coordinates)
    {
        this->coordinates = input_coordinates;

        for (size_t i = 0; i < this->coordinates.size(); i++) // coordinate : this->coordinates)
        {
            x_coordinates.push_back(this->coordinates[i].first);
            y_coordinates.push_back(this->coordinates[i].second);

            auto first_coord = this->coordinates[i];
            auto second_idx = i == this->coordinates.size() - 1 ? 0 : i + 1;
            auto second_coord = this->coordinates[second_idx];
            std::pair<std::pair<float, float>, std::pair<float, float>> coords = {first_coord, second_coord};
            this->line_segments.push_back(coords);
        }
    }

    std::vector<std::pair<float, float>> find_intersections(const Polygon &other_polygon)
    {
        std::vector<std::pair<float, float>> intersect_coordinates;

        for (auto p1_line : this->get_line_segments())
        {
            float l1_x1 = p1_line.first.first;
            float l1_y1 = p1_line.first.second;
            float l1_x2 = p1_line.second.first;
            float l1_y2 = p1_line.second.second;

            // std::cout<<"1-Line segment for ("<<l1_x1<<","<<l1_y1<<") &("<<l1_x2<<","<<l1_y2<<")\n";
            for (auto p2_line : other_polygon.get_line_segments())
            {

                float l2_x1 = p2_line.first.first;
                float l2_y1 = p2_line.first.second;
                float l2_x2 = p2_line.second.first;
                float l2_y2 = p2_line.second.second;

                float A1 = l1_y2 - l1_y1;
                float B1 = l1_x1 - l1_x2;
                float C1 = A1 * l1_x1 + B1 * l1_y1;

                float A2 = l2_y2 - l2_y1;
                float B2 = l2_x1 - l2_x2;
                float C2 = A2 * l2_x1 + B2 * l2_y1;

                auto det = A1 * B2 - A2 * B1;
                if (det == 0)
                { // std::cout<<"PARALLEL\n";
                    continue;
                }
                else
                {
                    // std::cout<<"2-Line segment for ("<<l2_x1<<","<<l2_y1<<") &("<<l2_x2<<","<<l2_y2<<")\n";

                    float x = (B2 * C1 - B1 * C2) / det;
                    float y = (A1 * C2 - A2 * C1) / det;

                    auto l1_x_bounds = std::max(l1_x1, l1_x2) >= x && x >= std::min(l1_x1, l1_x2);
                    auto l1_y_bounds = std::max(l1_y1, l1_y2) >= y && y >= std::min(l1_y1, l1_y2);
                    auto l2_x_bounds = std::max(l2_x1, l2_x2) >= x && x >= std::min(l2_x1, l2_x2);
                    auto l2_y_bounds = std::max(l2_y1, l2_y2) >= y && y >= std::min(l2_y1, l2_y2);

                    if (l1_x_bounds && l1_y_bounds && l2_x_bounds && l2_y_bounds)
                    {

                        intersect_coordinates.push_back({x, y});
                        std::cout << "Interection x: " << x << " y: " << y << std::endl;
                    }
                }
            }
        }

        return intersect_coordinates;
    }

    std::vector<std::pair<float, float>> re_order_coordinates(std::vector<std::pair<float, float>> coordinates)

    {   
        std::setprecision(5);
        std::vector<std::pair<float, float>> ordered_points;
        auto size_vector = coordinates.size();
        //ordered_points.resize(coordinates.size());

        ordered_points.push_back(coordinates[0]);

        // std::cout<<"\nSize:"<<size_vector;
        // std::cout<<"\nordered_points.size():"<<ordered_points.size();
        int counter = 0;
        while (ordered_points.size() < size_vector)
        {
            // for(auto a: coordinates)
            // std::cout<<"\n1-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;

            std::cout<<"\nPrinting array size:" <<ordered_points.size();
            auto point = ordered_points[counter];

            std::vector<float> lengths;

            std::map<float, std::pair<float, float>> map_length_coodinates;
            std::map<float, size_t> map_length_idx;
            //std::vector<size_t> idx;

            std::cout<<"\nPrint Point:"<<point.first<<", "<<point.second;

            for (size_t i = 1; i < coordinates.size(); i++)
            {
                float x_sqrd = std::pow(coordinates[i].first - point.first, 2);
                float y_sqrd = std::pow(coordinates[i].second - point.second, 2);

                float distance = std::sqrt(x_sqrd + y_sqrd);
                lengths.push_back(distance);
                //std::cout<<"\nHello "<<distance<<", "<<i;
                //std::cout<<" First:"<<coordinates[i].first<<" Second:"<<coordinates[i].second<<std::endl;


                map_length_coodinates.insert({distance, {coordinates[i].first, coordinates[i].second}});
                map_length_idx.insert({distance, i});
                // idx.push_back(i);
            }

            float min_length = *std::min_element(lengths.begin(), lengths.end());
            //std::cout<<"Min lenght"<<min_length;
            ordered_points.push_back({map_length_coodinates.at(min_length).first, map_length_coodinates.at(min_length).second});

            std::cout<<"Min length:"<<min_length<<", "<<"Coordinate: "<<map_length_coodinates.at(min_length).first<<map_length_coodinates.at(min_length).second<<" Idx: "<<map_length_idx.at(min_length)<<std::endl;
            coordinates.erase(coordinates.begin() + map_length_idx.at(min_length));
            counter+=1;

            //    for(auto a: ordered_points)
            //     std::cout<<"2-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;

            //         for(auto a: coordinates)
            //     std::cout<<"3-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;
        }

        return ordered_points;
    }

    std::vector<std::pair<float, float>> find_overlapping_coordinate(const Polygon &p1, const Polygon &other_polygon)
    {
        std::vector<std::pair<float, float>> overlap_coordinates;

        for (auto p1_coord : p1.get_coordinates())
        {
            auto x = p1_coord.first;
            auto y = p1_coord.second;

            auto other_x_coord = other_polygon.get_x_coordinates();
            auto other_y_coord = other_polygon.get_y_coordinates();

            auto x_min = *std::min_element(other_x_coord.begin(), other_x_coord.end());
            auto x_max = *std::max_element(other_x_coord.begin(), other_x_coord.end());
            auto y_min = *std::min_element(other_y_coord.begin(), other_y_coord.end());
            auto y_max = *std::max_element(other_y_coord.begin(), other_y_coord.end());

            if ((x >= x_min && x <= x_max) && (y >= y_min && y <= y_max))
            {
                overlap_coordinates.push_back({x, y});
            }
        }
        return overlap_coordinates;
    }
    void print_coordinate(const std::vector<std::pair<float, float>> &c)
{
    for (auto a : c)
    {
        std::cout << a.first << ", "<< a.second << std::endl;
    }
}

    void remove_redundant_coordinates(const std::vector<std::pair<float, float>> &redundant_coordinates, std::vector<std::pair<float, float>> &target_coordinates)
    {
        std::cout << "Size Before " << target_coordinates.size() << redundant_coordinates.size() << std::endl;
        for (auto coord : redundant_coordinates)
        {
            auto it = std::find(target_coordinates.begin(), target_coordinates.end(), coord);

            if (it != target_coordinates.end())
            {
                target_coordinates.erase(it);
            }
        }
        std::cout << "Size After " << target_coordinates.size()<<redundant_coordinates.size()<< std::endl;
    }

    Polygon operator-(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        new_coordinates.insert(new_coordinates.end(), this->coordinates.begin(), this->coordinates.end());

        // get intersection points
        auto intersection_coordinates = this->find_intersections(obj);
        auto redundant_coordinates = this->find_overlapping_coordinate(*this, obj);
        auto additional_coordinates = this->find_overlapping_coordinate(obj, *this);

        this->remove_redundant_coordinates(redundant_coordinates, new_coordinates);

        new_coordinates.insert(new_coordinates.end(), intersection_coordinates.begin(), intersection_coordinates.end());
        new_coordinates.insert(new_coordinates.end(), additional_coordinates.begin(), additional_coordinates.end());

        print_coordinate(new_coordinates);
        new_coordinates = this->re_order_coordinates(new_coordinates);


        std::cout<<"AFTER REORDER\n";
        print_coordinate(new_coordinates);

        return Polygon(new_coordinates);
    }

    void print_coordinates(void)
    {
        std::cout << "Printing (x,y) coordinate pairs" << std::endl;
        for (auto coordinate : this->coordinates)
        {
            std::cout << coordinate.first << ", " << coordinate.second << std::endl;
        }
    }

    void print_x_coordinates(void)
    {
        std::cout << "Printing X coordinates" << std::endl;

        for (auto coordinate : this->x_coordinates)
        {
            std::cout << coordinate << std::endl;
        }
    }

    void print_y_coordinates(void)
    {
        std::cout << "Printing Y coordinates" << std::endl;

        for (auto coordinate : this->y_coordinates)
        {
            std::cout << coordinate << std::endl;
        }
    }

    void print_segments(void)
    {
        std::cout << "Printing segments" << std::endl;

        for (auto segment : this->line_segments)
        {
            for (auto coordinate : this->coordinates)
            {
                std::cout << coordinate.first << ", " << coordinate.second << std::endl;
            }
        }
    }

    std::vector<std::pair<float, float>> get_coordinates(void) const
    {
        return coordinates;
    }

    std::vector<float> get_x_coordinates(void) const
    {
        return this->x_coordinates;
    }
    std::vector<float> get_y_coordinates(void) const
    {
        return this->y_coordinates;
    }

    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> get_line_segments(void) const
    {
        return this->line_segments;
    }
};