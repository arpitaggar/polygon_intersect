
#include "iostream"
#include "utility"
#include "vector"
#include "set"
#include "algorithm"
#include "string"
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
    plt::plot(obj.get_x_coordinates(),obj.get_y_coordinates(), symbol);

    // Draw the lines for a closed polygon.
    // Close the polygon the first vertex is inserted to the end of the vector.
    plt::plot(prepare_points_for_lines(obj.get_x_coordinates()),prepare_points_for_lines(obj.get_y_coordinates()), line_style);
}

int main()
{
    Polygon first_polygon({{0, 0}, {4, 0}, {4, 2}, {0, 2}});
    Polygon second_polygon({{3, 1}, {3, 3}, {6, 3}, {6, 1}});
   
    plot_line_with_symbol(first_polygon,"x","r-");
    plot_line_with_symbol(second_polygon,"x","b-");

    plt::show();
}