//
// Created by daniilsmelskiy on 01.05.21.
//

#include "convex_hull.h"

#include <algorithm>

using namespace Geometry;

std::vector<Point2D> Geometry::BuildBottomConvexHull(std::vector<Point2D> points, bool is_sorted) {
    if (!is_sorted) {
        std::sort(points.begin(), points.end());
    }

    int n = points.size();
    if (n <= 2) {
        return points;
    }

    std::vector<Point2D> convex_hull{points[0], points[1]};
    for (const auto& current_point: points) {
        while (convex_hull.size() > 1) {
            int current_size = convex_hull.size();

            if (IsClockwise(convex_hull[current_size - 2], convex_hull[current_size - 1],
                                    current_point)) {
                convex_hull.pop_back();
            } else {
                break;
            }
        }

        convex_hull.push_back(current_point);
    }

    return convex_hull;
}

std::vector<Point2D> Geometry::MergeBottomConvexHull(std::vector<Point2D> lhs, const std::vector<Point2D> &rhs) {
    if (lhs.empty()) {
        return rhs;
    }

    for (const Point2D& current_point: rhs) {
        while (lhs.size() > 1) {
            int current_size = lhs.size();

            if (IsClockwise(lhs[current_size - 2], lhs[current_size - 1], current_point)) {
                lhs.pop_back();
            } else {
                break;
            }
        }

        lhs.push_back(current_point);
    }

    return lhs;
}