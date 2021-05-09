//
// Created by daniilsmelskiy on 02.05.21.
//

#include "types.h"

namespace {
    long double GetUnsignedDoubleArea(const Geometry::Point2D& lhs, const Geometry::Point2D& mid,
                                      const Geometry::Point2D& rhs) {
        return lhs.x * (mid.y - rhs.y) +
               mid.x * (rhs.y - lhs.y) +
               rhs.x * (lhs.y - mid.y);
    }

    long double Sqr(long double x) {
        return x * x;
    }

    Geometry::Point2D GetLinesIntersection(long double a1, long double b1, long double c1,
                                 long double a2, long double b2, long double c2) {
        Geometry::Point2D res;
        res.x = -1.f * (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
        res.y = -1.f * (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);
        return res;
    }
}  // namespace

namespace Geometry {
    std::ostream& operator << (std::ostream& output, Point2D point) {
        output << point.x << " " << point.y;
        return output;
    }

    bool IsClockwise(const Point2D& lhs, const Point2D& mid, const Point2D& rhs) {
        return GetUnsignedDoubleArea(lhs, mid, rhs) < 0;
    }

    bool IsCounterClockwise(const Point2D& lhs, const Point2D& mid, const Point2D& rhs) {
        return GetUnsignedDoubleArea(lhs, mid, rhs) > 0;
    }

    long double GetSquareDistance(const Geometry::Point2D& fir, const Geometry::Point2D& sec) {
        long double res = Sqr(fir.x - sec.x) + Sqr(fir.y - sec.y);
        return res;
    }

    Circle2D GetCircle(const Point2D& x, const Point2D& y, const Point2D& z) {
        Point2D c = GetLinesIntersection(-2ll * (x.x - y.x), -2ll * (x.y - y.y),
                                         Sqr(x.x) + Sqr(x.y) - Sqr(y.x) - Sqr(y.y),
                                         -2ll * (x.x - z.x), -2ll * (x.y - z.y),
                                         Sqr(x.x) + Sqr(x.y) - Sqr(z.x) - Sqr(z.y));
        Circle2D circle;
        circle.center = c;
        circle.r = GetSquareDistance(c, x);

        return circle;
    }

    long double GetAngle(const Point2D& lhs, const Point2D& mid, const Point2D& rhs) {
        auto get_length = [](const Point2D& p) {
            return sqrt(Sqr(p.x) + Sqr(p.y));
        };

        Point2D v1 = lhs - mid;
        Point2D v2 = rhs - mid;

        long double cos_value = (v1.x * v2.x + v1.y * v2.y) / (get_length(v1) * get_length(v2));

        return acosl(cos_value);
    }

    bool IsPointInsideCircle(const Circle2D& circle, const Point2D& point) {
        return GetSquareDistance(circle.center, point) <= (circle.r - kEpsilon);
    }
}