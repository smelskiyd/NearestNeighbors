//
// Created by daniilsmelskiy on 04.05.21.
//

#include "convex_hull.h"
#include "triangulation.h"

#include "test_runner.h"

using namespace Geometry;

void TestGeometry() {
    {
        Point2D x{0, 1};
        Point2D y{1, 0};
        Point2D z{4, 3};

        ASSERT(IsCounterClockwise(x, y, z));
    }

    {
        Point2D x{0, 1};
        Point2D y{2, 3};
        Point2D z{4, 3};

        ASSERT(IsClockwise(x, y, z));
    }

    {
        Point2D x{0, 2};
        Point2D y{2, 0};
        Point2D z{4, 2};

        Circle2D c = GetCircle(x, y, z);
        Circle2D correct_result{{2.f, 2.f}, 4.f};

        ASSERT_EQUAL(c.center, correct_result.center);
        ASSERT_EQUAL(c.r, correct_result.r);
    }

}

void TestConvexHull() {
    std::vector<Geometry::Point2D> points = {
            {0, 1},
            {1, 0},
            {1, 4},
            {2, 3},
            {4, 3},
            {6, 0}
    };

    std::vector<Geometry::Point2D> convex_hull = Geometry::BuildBottomConvexHull(points);

    const std::vector<Geometry::Point2D> correct_result = {
            {0, 1},
            {1, 0},
            {6, 0}
    };

    ASSERT_EQUAL(convex_hull, correct_result);
}

int main(int argc, char* argv[]) {
    TestRunner tr;
    RUN_TEST(tr, TestGeometry);
    RUN_TEST(tr, TestConvexHull);
}