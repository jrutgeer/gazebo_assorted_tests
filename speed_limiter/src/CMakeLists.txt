cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

find_package(gz-cmake3 REQUIRED)

project(speed_limiter)

gz_find_package(gz-math7 REQUIRED)
set(GZ_SIM_VER ${gz-math7_VERSION_MAJOR})

add_executable(limiter_test speed_limiter.cpp)
target_link_libraries(limiter_test gz-math7)

install(TARGETS
    limiter_test
	DESTINATION bin/${PROJECT_NAME})
