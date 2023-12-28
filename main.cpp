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

// Function to append the first coordinate to the end of the coordinated vector. This is done to created a closed polygon.
std::vector<float> prepare_points_for_lines(const std::vector<float> &points)
{
    auto temp = points;
    temp.push_back(temp[0]);
    return temp;
}

// Function to plot the vertices of each Polygon object passed.
// This function will also prepared vertices etc.
void plot_line_with_symbol(const Polygon &obj, const std::string &symbol, const std::string &line_style)
{
    // Plot the vertices
    plt::plot(obj.get_x_coordinates(), obj.get_y_coordinates(), symbol);

    // Draw the lines for a closed polygon.
    // Close the polygon the first vertex is inserted to the end of the vector.
    plt::plot(prepare_points_for_lines(obj.get_x_coordinates()), prepare_points_for_lines(obj.get_y_coordinates()), line_style);
}

// Function to call plot function for first, second and resulting polygon.
void plot_polygons(const Polygon &p1, const Polygon &p2, const Polygon &p3)
{
    plt::clf();
    plot_line_with_symbol(p1, "x", "r-");
    plot_line_with_symbol(p2, "x", "b-");
    plot_line_with_symbol(p3, "o", "c--");
    
}

// Wrapper function to get intersection of two polygons and export the plot.
void do_intersect(Polygon &first_polygon, Polygon &second_polygon)
{
    auto result_polygon = first_polygon.intersect(second_polygon);
    std::cout<<"\nResulting Polygon from Intersect Operation:"<<std::endl;
    result_polygon.print_coordinate(result_polygon.get_coordinates());
    plot_polygons(first_polygon, second_polygon, result_polygon);
    plt::savefig("result_intersect.png");
}

// Wrapper function to get difference of two polygons and export the plot.
void do_difference(Polygon &first_polygon, Polygon &second_polygon)
{
    auto result_polygon = first_polygon - second_polygon;

    std::cout<<"\nResulting Polygon from Difference Operation:"<<std::endl;
    result_polygon.print_coordinate(result_polygon.get_coordinates());

    plot_polygons(first_polygon, second_polygon, result_polygon);

    plt::savefig("result_difference.png");
}

// Wrapper function to get union of two polygons and export the plot.
void do_union(Polygon &first_polygon, Polygon &second_polygon)
{
    auto result_polygon = first_polygon + second_polygon;
    std::cout<<"\nResulting Polygon from Union Operation:"<<std::endl;
    result_polygon.print_coordinate(result_polygon.get_coordinates());
    plot_polygons(first_polygon, second_polygon, result_polygon);
    plt::savefig("result_union.png");
}

int main()
{
    // Create first polygon object.
    Polygon first_polygon({{0, 0}, {4, 0}, {4, 2}, {0, 2}});
    // Create second polygon object.
    Polygon second_polygon({{3, 1}, {3, 3}, {6, 3}, {6, 1}}); //Polygon with intersection
    //Polygon second_polygon({{5, 1}, {5, 3}, {6, 3}, {6, 1}}); // Polygon without intersection

    // Do the three operations.
    do_union(first_polygon, second_polygon);
    do_intersect(first_polygon, second_polygon);
    do_difference(first_polygon, second_polygon);

    plt::close();
}