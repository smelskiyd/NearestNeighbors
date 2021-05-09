//
// Created by daniilsmelskiy on 01.05.21.
//

#pragma once

#include <vector>

#include "types.h"

namespace Geometry {

std::vector<Point2D> BuildBottomConvexHull(std::vector<Point2D> points, bool is_sorted = false);

std::vector<Point2D> MergeBottomConvexHull(std::vector<Point2D> lhs, const std::vector<Point2D>& rhs);

}  // namespace Geometry