//
// Created by daniilsmelskiy on 04.05.21.
//

#pragma once

#include "types.h"

using namespace Geometry;

class Plane {
  public:
    static void InitPlane(int argc, char* argv[]);
    static void PutPlane();

    static void AddLine(const Line2D& line);
    static void AddPoint(const Point2D& p);

    static void SetColor(int r, int g, int b);
    static void SetPointSize(int size);
    static void SetLineWidth(int width);

    static void SetPlaneSize(int width, int height);

  private:
    static constexpr int kDefaultPadding{20};
    static int plane_width;
    static int plane_height;
};