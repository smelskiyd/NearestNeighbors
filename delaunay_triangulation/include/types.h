//
// Created by daniilsmelskiy on 01.05.21.
//

#pragma once

#include <cmath>
#include <iostream>

namespace Geometry {

const long double kEpsilon = 1e-9;

struct Point2D {
    long double x{0}, y{0};
    int id{0};

    Point2D() = default;
    Point2D(long double x, long double y) : x(x), y(y) {}

    Point2D operator - (const Point2D& rhs) const {
        Point2D res;
        res.x = x - rhs.x;
        res.y = y - rhs.y;
        return res;
    }

    bool operator < (const Point2D& rhs) const {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }

    bool operator == (const Point2D& rhs) const {
        return (std::abs(x - rhs.x) <= kEpsilon &&
                std::abs(y - rhs.y) <= kEpsilon);
    }

    bool operator != (const Point2D& rhs) const {
        return !(*this == rhs);
    }
};

struct Line2D {
    Point2D fir, sec;

    Line2D(Point2D fir, Point2D sec) : fir(fir), sec(sec) {}
};

std::ostream& operator << (std::ostream& output, Point2D point);

struct Circle2D {
    Point2D center{};

    /// Squared radius
    long double r{0};
};

bool IsClockwise(const Point2D& lhs, const Point2D& mid, const Point2D& rhs);
bool IsCounterClockwise(const Point2D& lhs, const Point2D& mid, const Point2D& rhs);

long double GetSquareDistance(const Geometry::Point2D& fir, const Geometry::Point2D& sec);

long double GetAngle(const Point2D& lhs, const Point2D& mid, const Point2D& rhs);

Circle2D GetCircle(const Point2D& x, const Point2D& y, const Point2D& z);
bool IsPointInsideCircle(const Circle2D& circle, const Point2D& point);

}