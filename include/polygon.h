#pragma once

#include "iostream"
#include "utility"
#include "vector"
#include "set"
#include "algorithm"
#include "iomanip"
#include "cassert"

// Class create a Polygon object using input coordinates.
// This class also support other polygon union, intersect, subtract operations.
class Polygon
{   
    // Vector of pair of x,y coordinate representing the vertices of a polygon.
    std::vector<std::pair<float, float>> coordinates;

    // Vectors to hold only x values and y values separately.
    std::vector<float> x_coordinates, y_coordinates;

    // This is a vector of line_segments in the polygon. Each segement consists of a
    // starting and ending coordiate (as std::pair) which in turn contains the x & y coordinates as another pair.
    std::vector<std::pair<std::pair<float, float>, std::pair<float, float>>> line_segments;

public:

    // Constructor
    Polygon(std::vector<std::pair<float, float>> input_coordinates)
    {
        this->coordinates = input_coordinates;

        for (size_t i = 0; i < this->coordinates.size(); i++)
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

    // Function to find intersection points of polygone sides.
    std::vector<std::pair<float, float>> find_intersections(const Polygon &other_polygon)
    {
        std::vector<std::pair<float, float>> intersect_coordinates;

        for (auto p1_line : this->get_line_segments())
        {
            float l1_x1 = p1_line.first.first;
            float l1_y1 = p1_line.first.second;
            float l1_x2 = p1_line.second.first;
            float l1_y2 = p1_line.second.second;

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
                { 
                    continue;
                }
                else
                {

                    float x = (B2 * C1 - B1 * C2) / det;
                    float y = (A1 * C2 - A2 * C1) / det;

                    auto l1_x_bounds = std::max(l1_x1, l1_x2) >= x && x >= std::min(l1_x1, l1_x2);
                    auto l1_y_bounds = std::max(l1_y1, l1_y2) >= y && y >= std::min(l1_y1, l1_y2);
                    auto l2_x_bounds = std::max(l2_x1, l2_x2) >= x && x >= std::min(l2_x1, l2_x2);
                    auto l2_y_bounds = std::max(l2_y1, l2_y2) >= y && y >= std::min(l2_y1, l2_y2);

                    if (l1_x_bounds && l1_y_bounds && l2_x_bounds && l2_y_bounds)
                    {

                        intersect_coordinates.push_back({x, y});
                    }
                }
            }
        }

        return intersect_coordinates;
    }

    
    // Function to compute centroid.
    std::pair<float, float> compute_centroid(const std::vector<std::pair<float, float>> &coordinates)
    {

        float avg_x = 0;
        float avg_y = 0;
        for (auto coordinate : coordinates)
        {
            avg_x += coordinate.first;
            avg_y += coordinate.second;
        }
        avg_x /= coordinates.size();
        avg_y /= coordinates.size();
        return {avg_x, avg_y};

    }

    /*
    Function to re-order vertices in anti-clockwise order to create a closed polygon.
    Using this idea https://blogs.sas.com/content/iml/2021/11/17/order-vertices-convex-polygon.html#:~:text=Order%20vertices%20of%20a%20convex%20polygon&text=You%20can%20use%20the%20centroid,vertices%20of%20the%20convex%20polygon.
    */
    std::vector<std::pair<float, float>> re_order_coordinates(const std::vector<std::pair<float, float>> &coordinates)

    {
        std::setprecision(5);
        auto centroid = this->compute_centroid(coordinates);

        std::vector<std::pair<float, float>> ordered_points;
        ordered_points.reserve(coordinates.size());

        std::multimap<float, std::pair<float, float>> map_angle_coodinates;

        for (auto coordinate : coordinates)
        {
            auto angle = std::atan2(coordinate.second - centroid.second, coordinate.first - centroid.first);
            map_angle_coodinates.insert({angle, coordinate});
        }

        for (auto it = map_angle_coodinates.begin(); it != map_angle_coodinates.end(); it++)
        {
            ordered_points.push_back(it->second);
        }


        return ordered_points;
    }

    // Function to find any vertices of one polygon inside the other polygon.
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

    // Function to print the coordinates of a polygon.
    void print_coordinate(const std::vector<std::pair<float, float>> &c)
    {
        for (auto a : c)
        {
            std::cout << a.first << ", " << a.second << std::endl;
        }
    }

    // Function to remove redundant coordinates. Called incase of difference or union operations.
    void remove_redundant_coordinates(const std::vector<std::pair<float, float>> &redundant_coordinates, std::vector<std::pair<float, float>> &target_coordinates)
    {
        //std::cout << "Size Before " << target_coordinates.size() << redundant_coordinates.size() << std::endl;
        for (auto coord : redundant_coordinates)
        {
            auto it = std::find(target_coordinates.begin(), target_coordinates.end(), coord);

            if (it != target_coordinates.end())
            {
                target_coordinates.erase(it);
            }
        }
        //std::cout << "Size After " << target_coordinates.size() << redundant_coordinates.size() << std::endl;
    }

    // Function to return a polygon formed by intersection of two polygons.
    Polygon intersect(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;

        // get intersection points
        auto intersection_coordinates = this->find_intersections(obj);
        auto additional_coordinates1 = this->find_overlapping_coordinate(*this, obj);
        auto additional_coordinates2 = this->find_overlapping_coordinate(obj, *this);

        // Append the vector of coordinates.
        intersection_coordinates.insert(intersection_coordinates.end(), additional_coordinates1.begin(), additional_coordinates1.end());
        intersection_coordinates.insert(intersection_coordinates.end(), additional_coordinates2.begin(), additional_coordinates2.end());
        //print_coordinate(intersection_coordinates);

        // Reorder vertices.
        intersection_coordinates = this->re_order_coordinates(intersection_coordinates);

        assert(intersection_coordinates.empty()==false && "Intersection not found."); 


        return Polygon(intersection_coordinates);
    }

    // Operator overloading the subtract (-) operator to subtract a polygon from the target polgon.
    Polygon operator-(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        
        new_coordinates.insert(new_coordinates.end(), this->coordinates.begin(), this->coordinates.end());

        // get intersection points
        auto intersection_coordinates = this->find_intersections(obj);

        // Get points of target polygon in other polygon subtracted from it.
        auto redundant_coordinates = this->find_overlapping_coordinate(*this, obj);

        // Get points of subtracting polygon in target polygon.
        auto additional_coordinates = this->find_overlapping_coordinate(obj, *this);

        this->remove_redundant_coordinates(redundant_coordinates, new_coordinates);

        new_coordinates.insert(new_coordinates.end(), intersection_coordinates.begin(), intersection_coordinates.end());
        new_coordinates.insert(new_coordinates.end(), additional_coordinates.begin(), additional_coordinates.end());

        //print_coordinate(new_coordinates);
        new_coordinates = this->re_order_coordinates(new_coordinates);

        //std::cout << "AFTER REORDER\n";
        //print_coordinate(new_coordinates);


        return Polygon(new_coordinates);
    }

    // Operator overloading the add/union (+) operator to add a polygon to the target polgon.

    Polygon operator+(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        new_coordinates.insert(new_coordinates.end(), this->coordinates.begin(), this->coordinates.end());
        new_coordinates.insert(new_coordinates.end(), obj.coordinates.begin(), obj.coordinates.end());

        // get intersection points
        auto intersection_coordinates = this->find_intersections(obj);

        // Get and remove overlapping points of polygons in each other.
        this->remove_redundant_coordinates(this->find_overlapping_coordinate(*this, obj), new_coordinates);
        this->remove_redundant_coordinates(this->find_overlapping_coordinate(obj, *this), new_coordinates);

        new_coordinates.insert(new_coordinates.end(), intersection_coordinates.begin(), intersection_coordinates.end());
        // new_coordinates.insert(new_coordinates.end(), additional_coordinates.begin(), additional_coordinates.end());

        //print_coordinate(new_coordinates);
        new_coordinates = this->re_order_coordinates( new_coordinates);

        //std::cout << "AFTER REORDER\n";
        //print_coordinate(new_coordinates);

        assert(intersection_coordinates.empty()==false && "Union not possible since intersection not found.") ; 


        return Polygon(new_coordinates);
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