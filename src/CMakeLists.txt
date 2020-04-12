add_subdirectory(utils)
add_subdirectory(geometry)

add_library(RTCore
            AABB.hpp
            BVHNode.hpp
            BVHNode.cpp
            Camera.hpp
            Frame.hpp
            HittableList.hpp
            HittableList.cpp
            LinearAlgebra.hpp
            Ray.hpp
            RayTracer.hpp
            RayTracer.cpp)