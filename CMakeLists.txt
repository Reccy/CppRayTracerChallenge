cmake_minimum_required(VERSION 3.15.0)

set(PROJECT_NAME CppRayTracerChallenge)
project("${PROJECT_NAME}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
endif()

#
# Compile Flags Setup
#
set(CUSTOM_COMPILE_FLAGS "")
list(APPEND CUSTOM_COMPILE_FLAGS "/W4")		# Highest warning level in MSVC
list(APPEND CUSTOM_COMPILE_FLAGS "/WX")		# Treat warnings as errors

#
# Core Library Setup
#
set(CORE_LIBRARY_DIR "src/core")
set(CORE_LIBRARY_NAME "${PROJECT_NAME}Core")

set(LIBRARY_HEADERS
	"${CORE_LIBRARY_DIR}/math/tuple.h"
	"${CORE_LIBRARY_DIR}/math/comparison.h"
	"${CORE_LIBRARY_DIR}/math/trig.h"
	"${CORE_LIBRARY_DIR}/math/vector.h"
	"${CORE_LIBRARY_DIR}/math/point.h"
	"${CORE_LIBRARY_DIR}/math/matrix.h"
	"${CORE_LIBRARY_DIR}/math/transform.h"
	"${CORE_LIBRARY_DIR}/math/ray.h"
	"${CORE_LIBRARY_DIR}/math/sphere.h"
	"${CORE_LIBRARY_DIR}/math/intersection.h"
	"${CORE_LIBRARY_DIR}/math/intersections.h"
	"${CORE_LIBRARY_DIR}/math/i_intersectable.h"
	"${CORE_LIBRARY_DIR}/graphics/color.h"
	"${CORE_LIBRARY_DIR}/graphics/image.h"
	"${CORE_LIBRARY_DIR}/graphics/canvas.h"
	"${CORE_LIBRARY_DIR}/renderer/point_light.h"
	"${CORE_LIBRARY_DIR}/serializer/base_image_serializer.h"
	"${CORE_LIBRARY_DIR}/serializer/portable_pixmap_image_serializer.h"
)

set(LIBRARY_SOURCES
	"${CORE_LIBRARY_DIR}/math/trig.cpp"
	"${CORE_LIBRARY_DIR}/math/vector.cpp"
	"${CORE_LIBRARY_DIR}/math/point.cpp"
	"${CORE_LIBRARY_DIR}/math/transform.cpp"
	"${CORE_LIBRARY_DIR}/math/ray.cpp"
	"${CORE_LIBRARY_DIR}/math/sphere.cpp"
	"${CORE_LIBRARY_DIR}/math/intersection.cpp"
	"${CORE_LIBRARY_DIR}/math/intersections.cpp"
	"${CORE_LIBRARY_DIR}/graphics/color.cpp"
	"${CORE_LIBRARY_DIR}/graphics/image.cpp"
	"${CORE_LIBRARY_DIR}/graphics/canvas.cpp"
	"${CORE_LIBRARY_DIR}/renderer/point_light.cpp"
	"${CORE_LIBRARY_DIR}/serializer/base_image_serializer.cpp"
	"${CORE_LIBRARY_DIR}/serializer/portable_pixmap_image_serializer.cpp"
)

add_library("${CORE_LIBRARY_NAME}" "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")
target_compile_options("${CORE_LIBRARY_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")

#
# Test Suite Setup
#
include(GoogleTest)
include("cmake/SetupGoogleTest.cmake")
set(TEST_SUITE_DIR "src/test")
set(TEST_RUNNER_EXECUTABLE_NAME "${PROJECT_NAME}TestRunner")
set(TEST_RUNNER_EXECUTABLE_SOURCES
	"${TEST_SUITE_DIR}/core/math/tuple_test.cpp"
	"${TEST_SUITE_DIR}/core/math/comparison_test.cpp"
	"${TEST_SUITE_DIR}/core/math/trig_test.cpp"
	"${TEST_SUITE_DIR}/core/math/vector_test.cpp"
	"${TEST_SUITE_DIR}/core/math/point_test.cpp"
	"${TEST_SUITE_DIR}/core/math/matrix_test.cpp"
	"${TEST_SUITE_DIR}/core/math/transform_test.cpp"
	"${TEST_SUITE_DIR}/core/math/ray_test.cpp"
	"${TEST_SUITE_DIR}/core/math/sphere_test.cpp"
	"${TEST_SUITE_DIR}/core/math/intersection_test.cpp"
	"${TEST_SUITE_DIR}/core/math/intersections_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/color_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/image_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/canvas_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/point_light_test.cpp"
	"${TEST_SUITE_DIR}/core/serializer/portable_pixmap_image_serializer_test.cpp"
)
list(APPEND TEST_INCLUDED_DIRS "${CORE_LIBRARY_NAME}")
add_executable("${TEST_RUNNER_EXECUTABLE_NAME}" "${TEST_RUNNER_EXECUTABLE_SOURCES}")
target_include_directories("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${CORE_LIBRARY_DIR}")
target_link_libraries("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE gtest_main gmock_main "${CORE_LIBRARY_NAME}")
target_compile_options("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")

enable_testing()
gtest_discover_tests("${TEST_RUNNER_EXECUTABLE_NAME}")

#
# Executable Setup
#
set(EXECUTABLE_DIR "src/main")
set(EXECUTABLE_NAME "${PROJECT_NAME}")

list(APPEND INCLUDED_LIBS "${CORE_LIBRARY_NAME}")

if(WIN32)
	add_executable("${EXECUTABLE_NAME}" "${EXECUTABLE_DIR}/win32_main.cpp")

	list(APPEND INCLUDED_LIBS "${WIN32_THIRD_PARTY_LIBS}")
else()
	message(FATAL_ERROR "WIN32 is currently the only supported build platform")
endif()

target_include_directories("${EXECUTABLE_NAME}" PRIVATE "${CORE_LIBRARY_DIR}")
target_link_libraries("${EXECUTABLE_NAME}" PRIVATE "${INCLUDED_LIBS}")
target_compile_options("${EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")
