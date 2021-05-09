include_directories(include)

set(SOURCES
        include/types.h                 src/types.cpp
        include/convex_hull.h           src/convex_hull.cpp
        include/triangulation.h         src/triangulation.cpp
        include/nearest_point.h         src/nearest_point.cpp
        include/visualization.h         src/visualization.cpp)

add_library(geometry STATIC ${SOURCES})

target_include_directories(geometry PUBLIC include)