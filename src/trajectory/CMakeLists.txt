# Include path
#include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Dependency libraries
#find_package(OpenCV REQUIRED)

## Find LCM
find_package(LCM REQUIRED)

if(LCM_NOT_FOUND)
  message(STATUS "LCM is not found. Inter-process communication depends on LCM.")
endif(LCM_NOT_FOUND)

# Include files for LCM
set(LCM_GENERATED_HEADER_PATH ${CMAKE_BINARY_DIR}/include)
include_directories(${LCM_GENERATED_HEADER_PATH})



# Add libraries
set(TRAJECTORY_LIB_SRC
	#dubins_steer.cpp
	dubins_path.cpp
	#quad_polyopt.cpp
)
add_library(trajectory STATIC ${TRAJECTORY_LIB_SRC})
target_link_libraries(trajectory)
#add_library(transsys STATIC ${GRAPH_LIB_SRC})
#target_link_libraries(transsys spot bddx)

# Add executables
#add_executable(run_h2c h2c.cpp)
#target_link_libraries(build_graph graph ${OpenCV_LIBS})
#target_link_libraries(run_h2c h2c spot bddx)
