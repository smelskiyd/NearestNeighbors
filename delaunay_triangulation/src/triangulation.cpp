//
// Created by daniilsmelskiy on 01.05.21.
//

#include <algorithm>
#include <cassert>

#include "triangulation.h"
#include "convex_hull.h"

using namespace Geometry;

DelaunayTriangulation::Edge::Edge(int id, int from, int to) : id(id), from(from), to(to) {
    leftmost = this;
    rightmost = this;
}

DelaunayTriangulation::DelaunayTriangulation(std::vector<Point2D> points) :
        n_(points.size()), points_(std::move(points)) {
    Initialize();

    BuildTriangulation(0, n_);
}

const std::vector<Point2D>& DelaunayTriangulation::GetPointsIndexation() const {
    return points_;
}

const std::vector<Point2D>& DelaunayTriangulation::GetPointsById() const {
    return points_by_id_;
}

std::vector<int> DelaunayTriangulation::GetNeighbours(int v) const {
    auto last_edge = nodes_[v].last_edge;
    if (last_edge == nullptr) {
        return {};
    }

    std::vector<int> result;
    int first_edge_id = last_edge->id;
    while (true) {
        int current_id = last_edge->id;
        if (current_id == first_edge_id && !result.empty()) {
            break;
        }
        result.push_back(last_edge->to);

        last_edge = last_edge->leftmost;
    }

    return result;
}

void DelaunayTriangulation::Initialize() {
    std::sort(points_.begin(), points_.end());

    nodes_ = new Node[n_];
    for (int i = 0; i < n_; ++i) {
        nodes_[i] = Node{nullptr, i, 0};
        points_[i].id = i;
    }

    points_by_id_.resize(n_);
    for (const auto& point: points_) {
        points_by_id_[point.id] = point;
    }
}

std::vector<Point2D> DelaunayTriangulation::BuildTriangulation(int l, int r) {
    if (l >= r) {
        return {};
    }

    if (r - l <= 2) {
        if (r - l == 2) {
            AddEdge(points_[l].id, points_[l + 1].id);
        }

        std::vector<Point2D> result;
        for (int i = l; i < r; ++i) {
            result.push_back(points_[i]);
        }

        return result;
    }

    int mid = (l + r) / 2;
    const auto& convex_hull = MergeBottomConvexHull(BuildTriangulation(l, mid),
                                                    BuildTriangulation(mid, r));
    int pos{-1};
    for (int i = 1; i < convex_hull.size(); ++i) {
        if (convex_hull[i - 1].id < mid && convex_hull[i].id >= mid) {
            /// Edge between two parts
            pos = i - 1;
            break;
        }
    }
    assert(pos != -1);

    int base_left = convex_hull[pos].id;
    int base_right = convex_hull[pos + 1].id;
    int last_edge_id = AddEdge(base_left, base_right);

    while (true) {
        Edge* base_from_left = FindEdge(base_left, last_edge_id);
        Edge* left_candidate = FindTriangleLeft(base_from_left);

        Edge* base_from_right = FindEdge(base_right, last_edge_id);
        Edge* right_candidate = FindTriangleRight(base_from_right);

        if (left_candidate == nullptr) {
            if (right_candidate == nullptr) {
                break;
            } else {
                base_right = right_candidate->to;
                last_edge_id = AddEdge(base_left, base_right);
            }
        } else {
            if (right_candidate == nullptr) {
                base_left = left_candidate->to;
                last_edge_id = AddEdge(base_left, base_right);
            } else {
                Circle2D left_circle = GetCircle(points_[base_right], points_[base_left], points_[left_candidate->to]);
                if (!IsPointInsideCircle(left_circle, points_[right_candidate->to])) {
                    base_left = left_candidate->to;
                    last_edge_id = AddEdge(base_left, base_right);
                } else {
                    base_right = right_candidate->to;
                    last_edge_id = AddEdge(base_left, base_right);
                }
            }
        }
    }

    return convex_hull;
}

DelaunayTriangulation::Edge* DelaunayTriangulation::GetLeftmost(int v, const Edge& edge) {
    Edge* last_edge = nodes_[v].last_edge;
    if (last_edge == nullptr) {
        return nullptr;
    }

    while (true) {
        if (IsCounterClockwise(points_[edge.to], points_[edge.from], points_[last_edge->to])) {
            Edge* next_edge = last_edge->leftmost;

            if (IsCounterClockwise(points_[edge.to], points_[edge.from], points_[next_edge->to])) {
                // Still counter-clockwise, so we have to make sure that it's not some edge behind us
                // Angle must be less than current angle
                long double current_angle = GetAngle(points_[edge.to], points_[edge.from], points_[last_edge->to]);
                long double next_angle = GetAngle(points_[edge.to], points_[edge.from], points_[next_edge->to]);

                if (next_angle >= current_angle) {
                    last_edge = next_edge;
                    break;
                }
            } else {
                last_edge = next_edge;
                break;
            }

            last_edge = next_edge;
        } else {
            Edge* next_edge = last_edge->rightmost;

            if (IsClockwise(points_[edge.to], points_[edge.from], points_[next_edge->to])) {
                // Still clockwise, so we have to make sure that it's not some edge behind us
                // Angle must be less than current angle
                long double current_angle = GetAngle(points_[edge.to], points_[edge.from], points_[last_edge->to]);
                long double next_angle = GetAngle(points_[edge.to], points_[edge.from], points_[next_edge->to]);

                if (next_angle >= current_angle) {
                    break;
                }
            } else {
                break;
            }

            last_edge = next_edge;
        }
    }

    return last_edge;
}

DelaunayTriangulation::Edge* DelaunayTriangulation::GetRightmost(int v, const Edge& edge) {
    Edge* leftmost = GetLeftmost(v, edge);

    return leftmost ? leftmost->rightmost : nullptr;
}

void DelaunayTriangulation::InsertEdge(int id, int from, int to) {
    Edge* new_edge = new Edge(id, from, to);

    if (nodes_[from].last_edge == nullptr) {
        ++nodes_[from].size;
        nodes_[from].last_edge = new_edge;
        return;
    }

    Edge* leftmost = GetLeftmost(from, *new_edge);
    new_edge->leftmost = leftmost;
    new_edge->rightmost = leftmost->rightmost;

    leftmost->rightmost->leftmost = new_edge;
    leftmost->rightmost = new_edge;

    ++nodes_[from].size;
}

int DelaunayTriangulation::AddEdge(int from, int to) {
    InsertEdge(edges_counter_, from, to);
    InsertEdge(edges_counter_, to, from);

    return edges_counter_++;
}

DelaunayTriangulation::Edge* DelaunayTriangulation::FindEdge(int v, int id) {
    Edge* edge = nodes_[v].last_edge;
    while (edge->id != id) {
        edge = edge->rightmost;
    }

    return edge;
}

void DelaunayTriangulation::RemoveEdge(int v, int id) {
    Edge* edge = FindEdge(v, id);

    if (nodes_[v].size == 1) {
        nodes_[v].size = 0;
        nodes_[v].last_edge = nullptr;

        delete edge;
        return;
    }

    --nodes_[v].size;
    edge->leftmost->rightmost = edge->rightmost;
    edge->rightmost->leftmost = edge->leftmost;

    nodes_[v].last_edge = edge->leftmost;

    delete edge;
}

void DelaunayTriangulation::RemoveEdge(Edge* edge) {
    int from = edge->from;
    int to   = edge->to;
    int id   = edge->id;

    RemoveEdge(from, id);
    RemoveEdge(to,   id);
}

DelaunayTriangulation::Edge* DelaunayTriangulation::FindTriangleLeft(Edge *bottom_edge) {
    Edge* last_edge = bottom_edge->leftmost;

    while (last_edge != bottom_edge) {
        /// If current angle isn't less than PI
        if (!IsClockwise(points_[bottom_edge->to], points_[bottom_edge->from], points_[last_edge->to])) {
            return nullptr;
        }

        Edge* next_edge = last_edge->leftmost;
        if (next_edge == bottom_edge) {
            break;
        }

        Circle2D circle = GetCircle(points_[bottom_edge->to], points_[bottom_edge->from], points_[last_edge->to]);

        if (IsPointInsideCircle(circle, points_[next_edge->to])) {
            RemoveEdge(last_edge);

            last_edge = next_edge;
        } else {
            break;
        }
    }

    return last_edge == bottom_edge ? nullptr : last_edge;
}

DelaunayTriangulation::Edge* DelaunayTriangulation::FindTriangleRight(Edge *bottom_edge) {
    Edge* last_edge = bottom_edge->rightmost;

    while (last_edge != bottom_edge) {
        /// If current angle isn't less than PI
        if (!IsCounterClockwise(points_[bottom_edge->to], points_[bottom_edge->from], points_[last_edge->to])) {
            return nullptr;
        }

        Edge* next_edge = last_edge->rightmost;
        if (next_edge == bottom_edge) {
            break;
        }

        Circle2D circle = GetCircle(points_[bottom_edge->to], points_[bottom_edge->from], points_[last_edge->to]);
        if (IsPointInsideCircle(circle, points_[next_edge->to])) {
            RemoveEdge(last_edge);

            last_edge = next_edge;
        } else {
            break;
        }
    }

    return last_edge == bottom_edge ? nullptr : last_edge;
}

std::vector<Geometry::Line2D> DelaunayTriangulation::GetTriangulation() const {
    std::vector<Geometry::Line2D> result;

    for (int i = 0; i < n_; ++i) {
        for (int j : GetNeighbours(i)) {
            result.emplace_back(points_by_id_[i], points_by_id_[j]);
        }
    }

    return result;
}
