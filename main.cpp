#include <iostream>
#include <memory>
#include <algorithm>
#include <cstring>
#include <GL/freeglut.h>

#include "json.h"
#include "nearest_point.h"
#include "visualization.h"

using namespace Geometry;

const std::string kOutputJsonPath{"/home/daniilsmelskiy/working-directory/tmp_projects/NearestPoints/json_output/"};

std::unique_ptr<NearestNeighbour> nearest_neighbour;
int width, height;

// Some input test example: /home/daniilsmelskiy/working-directory/tmp_projects/NearestPoints/input_tests/test5

namespace {
    std::string GetFileName(const std::string& file_path) {
        int pos = 0;
        for (int i = 0; i < file_path.size(); ++i) {
            if (file_path[i] == '/') {
                pos = i + 1;
            }
        }

        std::string result;
        for (int i = pos; i < file_path.size(); ++i) {
            result += file_path[i];
        }

        return result;
    }
}  // namespace

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

    std::cout << "Input was read successfully" << std::endl;

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

void DumpResults(const std::string& file_name, const std::vector<Point2D>& points) {
    auto convert_point_to_json = [](const Point2D& point) {
        std::map<std::string, Json::Node> point_json;

        point_json.insert({"x", Json::Node(static_cast<double>(point.x))});
        point_json.insert({"y", Json::Node(static_cast<double>(point.y))});
        point_json.insert({"id", point.id});

        return point_json;
    };

    std::map<std::string, Json::Node> root;
    {
        std::vector<Json::Node> points_json;
        points_json.reserve(points.size());

        for (const auto& point : points) {
            points_json.emplace_back(convert_point_to_json(point));
        }

        root.insert({"points", points_json});
    }
    {
        std::vector<Json::Node> triangulation_lines;

        for (const auto& line : nearest_neighbour->GetTriangulation()) {
            std::map<std::string, Json::Node> line_json;

            line_json.insert({"first", convert_point_to_json(line.fir)});
            line_json.insert({"second", convert_point_to_json(line.sec)});

            triangulation_lines.emplace_back(line_json);
        }

        root.insert({"triangulation", triangulation_lines});
    }
    {
        std::vector<Json::Node> nearest_neighbor_lines;

        for (const auto& line : nearest_neighbour->GetNearestNeighbours()) {
            std::map<std::string, Json::Node> line_json;

            line_json.insert({"first", convert_point_to_json(line.fir)});
            line_json.insert({"second", convert_point_to_json(line.sec)});

            nearest_neighbor_lines.emplace_back(line_json);
        }

        root.insert({"nearest_neighbor", nearest_neighbor_lines});
    }

    std::string output_path = kOutputJsonPath + file_name;
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) {
        printf("Failed to open output file.\n");
        exit(1);
    } else {
        printf("Json results successfully saved into %s\n", output_path.c_str());
    }

    output_file << root;
    output_file.close();
}

int main(int argc, char* argv[]) {
    printf("Would you like to read a data from file?\n");
    bool read_from_file = false;
    while (true) {
        printf("Please, print 'yes', or 'no':\n");
        std::string answer;
        std::cin >> answer;

        if (answer == "yes") {
            read_from_file = true;
            break;
        } else if (answer == "no") {
            read_from_file = false;
            break;
        }
    }

    std::string input_file_path;

    std::vector<Point2D> points;
    if (read_from_file) {
        std::ifstream input;
        while (true) {
            printf("Please, print path to the input file: ");
            std::cin >> input_file_path;

            input.open(input_file_path);
            if (!input.is_open()) {
                printf("Failed to open input file.\n");
            } else {
                printf("Input file is successfully opened.\n");
                break;
            }
        }

        points = ReadInput(input);
    } else {
        points = ReadInput(std::cin);
    }

    nearest_neighbour = std::make_unique<NearestNeighbour>(points);

    std::string output_file_name;
    if (read_from_file) {
        output_file_name = GetFileName(input_file_path);
    } else {
        printf("Please, write output file name:\n");
        std::cin >> output_file_name;
    }

    DumpResults(output_file_name, points);
    Visualize();
}
