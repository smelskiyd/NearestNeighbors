//
// Created by daniilsmelskiy on 04.05.21.
//

#pragma once

#include <vector>
#include <memory>
#include <fstream>

#include "types.h"
#include "triangulation.h"

class NearestNeighbour {
  public:
    NearestNeighbour(std::vector<Geometry::Point2D> points);
    void PrintGraph(std::ofstream& output) const;

    std::vector<Geometry::Point2D> GetPoints() const;
    std::vector<Geometry::Line2D> GetTriangulation() const;
    std::vector<Geometry::Line2D> GetNearestNeighbours() const;

  private:
    const bool kCheckResults{true};

    void BuildTriangulation();
    void FindClosest();

    void RunResultsValidation() const;

    int n_;

    std::vector<Geometry::Point2D> points_;
    std::vector<Geometry::Point2D> points_by_id_;
    std::vector<int> closest_point_;

    std::unique_ptr<Geometry::DelaunayTriangulation> triangulation_;
};