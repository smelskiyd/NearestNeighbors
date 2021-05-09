//
// Created by daniilsmelskiy on 04.05.21.
//

#include "nearest_point.h"

NearestNeighbour::NearestNeighbour(std::vector<Geometry::Point2D> points) :
        n_(points.size()), points_(std::move(points)) {
    BuildTriangulation();
    FindClosest();
}

void NearestNeighbour::BuildTriangulation() {
    triangulation_ = std::make_unique<Geometry::DelaunayTriangulation>(points_);

    points_ = triangulation_->GetPointsIndexation();

    points_by_id_ = triangulation_->GetPointsById();

    if (n_ <= 500 && kCheckResults) {
        RunResultsValidation();
    }
}

void NearestNeighbour::FindClosest() {
    closest_point_.resize(n_);

    for (int i = 0; i < n_; ++i) {
        long double min_distance = std::numeric_limits<long double>::max();
        int closest = i;

        for (int j : triangulation_->GetNeighbours(i)) {
            long double current_distance = Geometry::GetSquareDistance(points_by_id_[i], points_by_id_[j]);
            if (current_distance <= min_distance + Geometry::kEpsilon) {
                min_distance = current_distance;
                closest = j;
            }
        }

        closest_point_[i] = closest;
    }
}

void NearestNeighbour::PrintGraph(std::ofstream& output) const {
    for (const auto point : points_) {
        output << "Point(" << point.x << ", " << point.y << ") id: " << point.id << '\n';
    }

    for (int i = 0; i < n_; ++i) {
        output << "Point " << i << " neighbours: ";

        bool first = true;
        for (const int j : triangulation_->GetNeighbours(i)) {
            if (!first) {
                output << ", ";
            }
            first = false;
            output << j;
        }

        output << '\n';
    }
}

std::vector<Geometry::Point2D> NearestNeighbour::GetPoints() const {
    return points_;
}

std::vector<Geometry::Line2D> NearestNeighbour::GetTriangulation() const {
    return triangulation_->GetTriangulation();
}

std::vector<Geometry::Line2D> NearestNeighbour::GetNearestNeighbours() const {
    std::vector<Geometry::Line2D> result;

    for (int i = 0; i < n_; ++i) {
        result.emplace_back(points_by_id_[i], points_by_id_[closest_point_[i]]);
    }

    return result;
}

void NearestNeighbour::RunResultsValidation() const {
    for (int i = 0; i < n_; ++i) {
        auto neighbours = triangulation_->GetNeighbours(i);

        for (int j = 0; j < neighbours.size(); ++j) {
            int cur = neighbours[j];
            int next = neighbours[(j + 1) % neighbours.size()];

            if (i < cur && i < next && cur != next) {
                if (!Geometry::IsClockwise(points_by_id_[cur], points_by_id_[i], points_by_id_[next])) {
                    continue;
                }
                Geometry::Circle2D circle = Geometry::GetCircle(points_by_id_[i], points_by_id_[cur],
                                                                points_by_id_[next]);

                for (int k = 0; k < n_; ++k) {
                    if (i != k && cur != k && next != k) {
                        if (Geometry::IsPointInsideCircle(circle, points_by_id_[k])) {
                            std::cout << "Triangulation is not Delaunay" << std::endl;
                            std::cout << "Circle from points ("     << points_by_id_[i] << "), "
                                      << "(" << points_by_id_[cur]  << "), "
                                      << "(" << points_by_id_[next] << ") contains point "
                                      << "(" << points_by_id_[k]    << ")" << std::endl;
                            exit(2);
                        }
                    }
                }
            }
        }
    }
}