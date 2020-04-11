add_subdirectory(utils)

add_library(RTCore
            Camera.hpp
            Frame.hpp
            HittableList.hpp
            HittableList.cpp
            LinearAlgebra.hpp
            Ray.hpp
            RayTracer.hpp
            RayTracer.cpp)