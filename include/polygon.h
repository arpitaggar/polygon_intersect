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

    // This creates a vector of line_segments in the polygon. Each segement consists of a
    // starting and ending coordiate (as std::pair) which in turn contains the x & y coordinates as another pair.

    std::vector<std::pair<std::pair<float, float>,std::pair<float, float>> > line_segments;

public:
    Polygon(std::vector<std::pair<float, float>> input_coordinates)
    {
        this->coordinates = input_coordinates;

        for (size_t i=0; i<this->coordinates.size(); i++)  // coordinate : this->coordinates)
        {
            x_coordinates.push_back(this->coordinates[i].first);
            y_coordinates.push_back(this->coordinates[i].second);

            auto first_coord = this->coordinates[i];
            auto second_idx = i == this->coordinates.size()-1 ? 0 : i+1;
            auto second_coord = this->coordinates[second_idx];
            std::pair<std::pair<float, float>,std::pair<float, float>> coords= {first_coord, second_coord};
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


        //std::cout<<"1-Line segment for ("<<l1_x1<<","<<l1_y1<<") &("<<l1_x2<<","<<l1_y2<<")\n";
        for(auto p2_line : other_polygon.get_line_segments())
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


            auto det = A1*B2 - A2*B1;
            if (det==0)
            {   //std::cout<<"PARALLEL\n";
                continue;
            }
            else{
                    //std::cout<<"2-Line segment for ("<<l2_x1<<","<<l2_y1<<") &("<<l2_x2<<","<<l2_y2<<")\n";

                    float x = (B2*C1 - B1*C2)/det;
                    float y = (A1*C2 - A2*C1)/det;

                    auto l1_x_bounds = std::max(l1_x1,l1_x2)>=x && x>=std::min(l1_x1,l1_x2);
                    auto l1_y_bounds = std::max(l1_y1,l1_y2)>=y && y>=std::min(l1_y1,l1_y2);
                    auto l2_x_bounds = std::max(l2_x1,l2_x2)>=x && x>=std::min(l2_x1,l2_x2);
                    auto l2_y_bounds = std::max(l2_y1,l2_y2)>=y && y>=std::min(l2_y1,l2_y2);
                    
                    if (l1_x_bounds && l1_y_bounds && l2_x_bounds && l2_y_bounds)
                    {   

                        intersect_coordinates.push_back({x,y});
                        std::cout<<"Interection x: "<<x<<" y: "<<y<<std::endl;
            }
                }
        }
    }

    return intersect_coordinates;
}

    Polygon operator-(Polygon const &obj)
    {
        std::vector<std::pair<float, float>> new_coordinates;
        new_coordinates.resize(std::max(this->coordinates.size(), obj.coordinates.size()));
        return Polygon(new_coordinates);
    }



    void print_coordinates()
    {   
        std::cout<<"Printing (x,y) coordinate pairs"<<std::endl;
        for (auto coordinate : this->coordinates)
        {
            std::cout << coordinate.first << ", " << coordinate.second << std::endl;
        }
    }

    void print_x_coordinates()
    {
        std::cout<<"Printing X coordinates"<<std::endl;

        for (auto coordinate : this->x_coordinates)
        {
            std::cout << coordinate << std::endl;
        }
    }
        
    void print_y_coordinates()
    {
        std::cout<<"Printing Y coordinates"<<std::endl;

        for (auto coordinate : this->y_coordinates)
        {
            std::cout << coordinate << std::endl;
        }
    }

    void print_segments()
    {
        std::cout<<"Printing segments"<<std::endl;

        for (auto segment : this->line_segments)
        {   
            for (auto coordinate : this->coordinates)
        {
            std::cout << coordinate.first << ", " << coordinate.second << std::endl;
        }
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

    std::vector<std::pair<std::pair<float, float>,std::pair<float, float>> > get_line_segments() const
    {
        return this->line_segments;
    }
};