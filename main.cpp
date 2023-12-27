
#include "iostream"
#include "utility"
#include "vector"
#include "set"
#include "algorithm"
#include "string"
#include "cmath"
#include "include/matplotlibcpp.h"
#include "include/polygon.h"

namespace plt = matplotlibcpp;

std::vector<float> prepare_points_for_lines(const std::vector<float> &points)
{
    auto temp = points;
    temp.push_back(temp[0]);
    return temp;
}

void plot_line_with_symbol(const Polygon &obj, const std::string &symbol, const std::string &line_style)
{
    // Plot the vertices
    plt::plot(obj.get_x_coordinates(), obj.get_y_coordinates(), symbol);

    // Draw the lines for a closed polygon.
    // Close the polygon the first vertex is inserted to the end of the vector.
    plt::plot(prepare_points_for_lines(obj.get_x_coordinates()), prepare_points_for_lines(obj.get_y_coordinates()), line_style);
}

void print_coordinate(const std::vector<std::pair<float, float>> &c)
{
    for (auto a : c)
    {
        std::cout << a.first << ", "<< a.second << std::endl;
    }
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

std::vector<std::pair<float, float>> re_order_coordinates(std::vector<std::pair<float, float>> coordinates)

{   
    std::vector<std::pair<float, float>> ordered_points;
    auto size_vector = coordinates.size();
    //ordered_points.resize(coordinates.size());

    ordered_points.push_back(coordinates[0]);

    
    // std::cout<<"\nSize:"<<size_vector;
    // std::cout<<"\nordered_points.size():"<<ordered_points.size();
    while (ordered_points.size()<size_vector)
    {
        // for(auto a: coordinates)
            // std::cout<<"\n1-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;
        
        // std::cout<<"\nPrinting array size:" <<ordered_points.size();
        auto point = ordered_points[ordered_points.size()];

        std::vector<float> lengths ;

        std::map<float, std::pair<float, float>> map_length_coodinates;
        std::map<float, size_t> map_length_idx;
        // std::vector<size_t> idx;


        for (size_t i = 1; i < coordinates.size(); i++)
        {   
            float x_sqrd = std::pow(coordinates[i].first - point.first, 2);
            float y_sqrd = std::pow(coordinates[i].second - point.second, 2);

            float distance = std::sqrt(x_sqrd+y_sqrd);
            lengths.push_back(distance);
            // std::cout<<"\nHello "<<distance<<", "<<i<<std::endl;
            map_length_coodinates.insert({distance, {coordinates[i].first, coordinates[i].second}});
            map_length_idx.insert({distance, i});
            // idx.push_back(i);

        }

        float min_length = *std::min_element(lengths.begin(), lengths.end());
        // std::cout<<"Min lenght"<<min_length;
        ordered_points.push_back({map_length_coodinates.at(min_length).first, map_length_coodinates.at(min_length).second} );

        //std::cout<<"Min length:"<<min_length<<", "<<"Coordinate: "<<map_length_coodinates.at(min_length).first<<map_length_coodinates.at(min_length).second<<" Idx: "<<map_length_idx.at(min_length)<<std::endl;
        coordinates.erase(coordinates.begin() + map_length_idx.at(min_length));

    //    for(auto a: ordered_points)
    //     std::cout<<"2-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;

    //         for(auto a: coordinates)
    //     std::cout<<"3-Coordinates First:"<<a.first<<", "<< "Second: " <<a.second<<std::endl;


    }

    return ordered_points;
}

int main()
{
    Polygon first_polygon({{0, 0}, {4, 0}, {4, 2}, {0, 2}});
    Polygon second_polygon({{3, 1}, {3, 3}, {6, 3}, {6, 1}});
    // Polygon second_polygon({{5, 1}, {5, 3}, {6, 3}, {6, 1}});

    // first_polygon.print_coordinates();
    // first_polygon.print_x_coordinates();
    // first_polygon.print_y_coordinates();
    // first_polygon.print_segments();

    // second_polygon.print_coordinates();
    // second_polygon.print_x_coordinates();
    // second_polygon.print_y_coordinates();
    // second_polygon.print_segments();

    plot_line_with_symbol(first_polygon, "x", "r-");
    plot_line_with_symbol(second_polygon, "x", "b-");

    auto intersection_coordinates = second_polygon.find_intersections(first_polygon);

    if (intersection_coordinates.empty())
    {
        std::cout << "No intersections found. Exiting." << std::endl;
        return 0;
    }

    auto overlap_coordinates1 = find_overlapping_coordinate(first_polygon, second_polygon);
    auto overlap_coordinates2 = find_overlapping_coordinate(second_polygon, first_polygon);

    intersection_coordinates.insert(intersection_coordinates.end(), overlap_coordinates1.begin(), overlap_coordinates1.end());
    intersection_coordinates.insert(intersection_coordinates.end(), overlap_coordinates2.begin(), overlap_coordinates2.end());

    // plt::plot({ic[0].first, ic[1].first}, {ic[0].second, ic[1].second}, "o");
    // print_coordinate(intersection_coordinates);

    // std::cout<<"Overllaping";
    // std::cout << "Before reordering:\n";
    // print_coordinate(intersection_coordinates);
    intersection_coordinates = re_order_coordinates(intersection_coordinates);

    // std::cout << "After reordering:\n";
    // print_coordinate(intersection_coordinates);

    Polygon comon(intersection_coordinates);
    plot_line_with_symbol(comon, "o", "c--");

    plt::show();
}