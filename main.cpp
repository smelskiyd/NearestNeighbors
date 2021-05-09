#include <iostream>
#include <memory>
#include <algorithm>
#include <GL/freeglut.h>

#include "triangulation.h"
#include "nearest_point.h"
#include "visualization.h"

using namespace Geometry;

std::unique_ptr<NearestNeighbour> nearest_neighbour;
int width, height;

std::vector<Point2D> SelectUnique(std::vector<Point2D> points) {
    std::sort(points.begin(), points.end());

    int removed_counter = 0;

    std::vector<Point2D> unique_points;
    for (int i = 0; i < points.size(); ++i) {
        if (i == 0 || points[i] != points[i - 1]) {
            unique_points.push_back(points[i]);
        } else {
            ++removed_counter;
        }
    }

    std::cout << "Number of removed duplicated points: " << removed_counter << std::endl;

    return unique_points;
}

std::vector<Point2D> ReadInput(std::istream& input) {
    std::cout << "Print N: \n";
    int n;
    input >> n;

    std::vector<Point2D> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::cout << "Print point " << i << " coordinates: \n";

        long double x, y;
        input >> x >> y;
        points.emplace_back(x, y);
    }
    points = SelectUnique(points);

    std::cout << "Print plane width and height: \n";
    input >> width >> height;

    return points;
}

void display_background_lines() {
    Plane::SetLineWidth(3);
    Plane::SetColor(194, 194, 194);

    for (const Line2D& line : nearest_neighbour->GetTriangulation()) {
        Plane::AddLine(line);
    }
}

void display_foreground_lines() {
    Plane::SetLineWidth(4);
    Plane::SetColor(10, 191, 0);

    for (const Line2D& line : nearest_neighbour->GetNearestNeighbours()) {
        Plane::AddLine(line);
    }
}

void display_points() {
    Plane::SetPointSize(5);
    Plane::SetColor(0, 0, 0);

    for (const Point2D& point : nearest_neighbour->GetPoints()) {
        Plane::AddPoint(point);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    display_background_lines();
    display_foreground_lines();
    display_points();

    glutSwapBuffers();
}

void Visualize() {
    Plane::SetPlaneSize(width, height);

    Plane::InitPlane(0, nullptr);

    glutDisplayFunc(display);

    Plane::PutPlane();
}

int main(int argc, char* argv[]) {
    std::ifstream input("/home/daniilsmelskiy/working-directory/tmp_projects/NearestPoints/input_tests/test5");
    if (!input.is_open()) {
        exit(1);
    }

    const auto points = ReadInput(input);
    std::cout << "Input was read successfully" << std::endl;

    nearest_neighbour = std::make_unique<NearestNeighbour>(points);

    Visualize();
}
