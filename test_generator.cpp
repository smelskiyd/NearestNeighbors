//
// Created by daniilsmelskiy on 05.05.21.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "types.h"

using namespace Geometry;

int n;
int width, height;
std::vector<Point2D> points;

void GeneratePoints() {
    unsigned int time_ui = static_cast<unsigned int>(time(nullptr));
    srand(time_ui);

    for (int i = 0; i < n; ++i) {
        int x = std::abs(rand()) % width;
        int y = std::abs(rand()) % height;

        points.emplace_back(x, y);
    }
}

int main(int argc, char* argv[]) {
    std::ofstream output("/home/daniilsmelskiy/working-directory/tmp_projects/NearestPoints/input_tests/test9");
    if (!output.is_open()) {
        exit(1);
    }

    std::cout << "Print N: ";
    std::cin >> n;

    std::cout << "Print width and height: ";
    std::cin >> width >> height;

    GeneratePoints();

    output << n << '\n';
    for (const auto& point : points) {
        output << point.x << " " << point.y << '\n';
    }
    output << width << " " << height << std::endl;

    output.close();
}