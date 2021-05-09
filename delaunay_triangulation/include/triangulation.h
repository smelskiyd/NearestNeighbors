//
// Created by daniilsmelskiy on 01.05.21.
//

#pragma once

#include <vector>

#include "types.h"

namespace Geometry {

class DelaunayTriangulation {
  public:
    explicit DelaunayTriangulation(std::vector<Point2D> points);

    const std::vector<Point2D>& GetPointsIndexation() const;
    const std::vector<Geometry::Point2D>& GetPointsById() const;

    std::vector<int> GetNeighbours(int v) const;
    std::vector<Geometry::Line2D> GetTriangulation() const;

  private:
    struct Edge {
        int id{};
        int from{}, to{};
        Edge* leftmost{};
        Edge* rightmost{nullptr};

        Edge() = default;
        Edge(int id, int from, int to);
    };

    struct Node {
        Edge* last_edge{nullptr};
        int id{0};
        int size{0};
    };

    void Initialize();

    /// Returns bottom convex-hull of points
    std::vector<Point2D> BuildTriangulation(int l, int r);

    Edge* GetLeftmost(int v, const Edge& edge);
    Edge* GetRightmost(int v, const Edge& edge);
    void InsertEdge(int id, int from, int to);
    int  AddEdge(int from, int to);
    Edge* FindEdge(int v, int id);
    void RemoveEdge(int v, int id);
    void RemoveEdge(Edge* edge);

    Edge* FindTriangleLeft(Edge* bottom_edge);
    Edge* FindTriangleRight(Edge* bottom_edge);

    Node* nodes_{nullptr};
    int n_;
    std::vector<Point2D> points_;
    std::vector<Point2D> points_by_id_;

    int edges_counter_{0};
};

}  // namespace Geometry