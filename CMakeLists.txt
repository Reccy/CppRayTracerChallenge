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
	"${CORE_LIBRARY_DIR}/math/constants.h"
	"${CORE_LIBRARY_DIR}/math/tuple.h"
	"${CORE_LIBRARY_DIR}/math/comparison.h"
	"${CORE_LIBRARY_DIR}/math/trig.h"
	"${CORE_LIBRARY_DIR}/math/vector.h"
	"${CORE_LIBRARY_DIR}/math/point.h"
	"${CORE_LIBRARY_DIR}/math/matrix.h"
	"${CORE_LIBRARY_DIR}/math/transform.h"
	"${CORE_LIBRARY_DIR}/math/ray.h"
	"${CORE_LIBRARY_DIR}/math/i_shape.h"
	"${CORE_LIBRARY_DIR}/math/shape.h"
	"${CORE_LIBRARY_DIR}/math/sphere.h"
	"${CORE_LIBRARY_DIR}/math/plane.h"
	"${CORE_LIBRARY_DIR}/math/cube.h"
	"${CORE_LIBRARY_DIR}/math/cylinder.h"
	"${CORE_LIBRARY_DIR}/math/cone.h"
	"${CORE_LIBRARY_DIR}/math/triangle.h"
	"${CORE_LIBRARY_DIR}/math/smooth_triangle.h"
	"${CORE_LIBRARY_DIR}/math/intersection.h"
	"${CORE_LIBRARY_DIR}/math/intersections.h"
	"${CORE_LIBRARY_DIR}/math/perlin_noise.h"
	"${CORE_LIBRARY_DIR}/math/bounding_box.h"
	"${CORE_LIBRARY_DIR}/graphics/color.h"
	"${CORE_LIBRARY_DIR}/graphics/image.h"
	"${CORE_LIBRARY_DIR}/graphics/canvas.h"
	"${CORE_LIBRARY_DIR}/renderer/point_light.h"
	"${CORE_LIBRARY_DIR}/renderer/material.h"
	"${CORE_LIBRARY_DIR}/renderer/lighting.h"
	"${CORE_LIBRARY_DIR}/renderer/world.h"
	"${CORE_LIBRARY_DIR}/renderer/computed_values.h"
	"${CORE_LIBRARY_DIR}/renderer/i_group.h"
	"${CORE_LIBRARY_DIR}/renderer/shape.h"
	"${CORE_LIBRARY_DIR}/renderer/group.h"
	"${CORE_LIBRARY_DIR}/renderer/csg.h"
	"${CORE_LIBRARY_DIR}/renderer/camera.h"
	"${CORE_LIBRARY_DIR}/renderer/render_job.h"
	"${CORE_LIBRARY_DIR}/renderer/pattern.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/solid_color.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/stripe.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/gradient.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/ring.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/checker.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/radial_gradient.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/perturbed.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/masked.h"
	"${CORE_LIBRARY_DIR}/renderer/patterns/test_pattern.h"
	"${CORE_LIBRARY_DIR}/serializer/base_image_serializer.h"
	"${CORE_LIBRARY_DIR}/serializer/portable_pixmap_image_serializer.h"
	"${CORE_LIBRARY_DIR}/serializer/portable_network_graphics_serializer.h"
	"${CORE_LIBRARY_DIR}/serializer/wavefront_obj_deserializer.h"
	"${CORE_LIBRARY_DIR}/encryption/crc.h"
	"${CORE_LIBRARY_DIR}/encryption/adler32.h"
	"${CORE_LIBRARY_DIR}/compression/huffman_coding.h"
	"${CORE_LIBRARY_DIR}/compression/deflate_block.h"
	"${CORE_LIBRARY_DIR}/compression/zlib.h"
	"${CORE_LIBRARY_DIR}/helpers/polymorphic_comparable.h"
	"${CORE_LIBRARY_DIR}/helpers/material_helper.h"
)

set(LIBRARY_SOURCES
	"${CORE_LIBRARY_DIR}/math/trig.cpp"
	"${CORE_LIBRARY_DIR}/math/vector.cpp"
	"${CORE_LIBRARY_DIR}/math/point.cpp"
	"${CORE_LIBRARY_DIR}/math/transform.cpp"
	"${CORE_LIBRARY_DIR}/math/ray.cpp"
	"${CORE_LIBRARY_DIR}/math/shape.cpp"
	"${CORE_LIBRARY_DIR}/math/sphere.cpp"
	"${CORE_LIBRARY_DIR}/math/plane.cpp"
	"${CORE_LIBRARY_DIR}/math/cube.cpp"
	"${CORE_LIBRARY_DIR}/math/cylinder.cpp"
	"${CORE_LIBRARY_DIR}/math/cone.cpp"
	"${CORE_LIBRARY_DIR}/math/triangle.cpp"
	"${CORE_LIBRARY_DIR}/math/smooth_triangle.cpp"
	"${CORE_LIBRARY_DIR}/math/intersection.cpp"
	"${CORE_LIBRARY_DIR}/math/intersections.cpp"
	"${CORE_LIBRARY_DIR}/math/perlin_noise.cpp"
	"${CORE_LIBRARY_DIR}/math/bounding_box.cpp"
	"${CORE_LIBRARY_DIR}/graphics/color.cpp"
	"${CORE_LIBRARY_DIR}/graphics/image.cpp"
	"${CORE_LIBRARY_DIR}/graphics/canvas.cpp"
	"${CORE_LIBRARY_DIR}/renderer/point_light.cpp"
	"${CORE_LIBRARY_DIR}/renderer/material.cpp"
	"${CORE_LIBRARY_DIR}/renderer/lighting.cpp"
	"${CORE_LIBRARY_DIR}/renderer/world.cpp"
	"${CORE_LIBRARY_DIR}/renderer/computed_values.cpp"
	"${CORE_LIBRARY_DIR}/renderer/shape.cpp"
	"${CORE_LIBRARY_DIR}/renderer/group.cpp"
	"${CORE_LIBRARY_DIR}/renderer/csg.cpp"
	"${CORE_LIBRARY_DIR}/renderer/camera.cpp"
	"${CORE_LIBRARY_DIR}/renderer/render_job.cpp"
	"${CORE_LIBRARY_DIR}/renderer/pattern.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/solid_color.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/stripe.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/gradient.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/ring.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/checker.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/radial_gradient.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/perturbed.cpp"
	"${CORE_LIBRARY_DIR}/renderer/patterns/test_pattern.cpp"
	"${CORE_LIBRARY_DIR}/serializer/base_image_serializer.cpp"
	"${CORE_LIBRARY_DIR}/serializer/portable_pixmap_image_serializer.cpp"
	"${CORE_LIBRARY_DIR}/serializer/portable_network_graphics_serializer.cpp"
	"${CORE_LIBRARY_DIR}/serializer/wavefront_obj_deserializer.cpp"
	"${CORE_LIBRARY_DIR}/encryption/crc.cpp"
	"${CORE_LIBRARY_DIR}/encryption/adler32.cpp"
	"${CORE_LIBRARY_DIR}/compression/huffman_coding.cpp"
	"${CORE_LIBRARY_DIR}/compression/deflate_block.cpp"
	"${CORE_LIBRARY_DIR}/compression/zlib.cpp"
	"${CORE_LIBRARY_DIR}/helpers/material_helper.cpp"
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
	"${TEST_SUITE_DIR}/core/math/plane_test.cpp"
	"${TEST_SUITE_DIR}/core/math/cube_test.cpp"
	"${TEST_SUITE_DIR}/core/math/cylinder_test.cpp"
	"${TEST_SUITE_DIR}/core/math/cone_test.cpp"
	"${TEST_SUITE_DIR}/core/math/triangle_test.cpp"
	"${TEST_SUITE_DIR}/core/math/smooth_triangle_test.cpp"
	"${TEST_SUITE_DIR}/core/math/intersection_test.cpp"
	"${TEST_SUITE_DIR}/core/math/intersections_test.cpp"
	"${TEST_SUITE_DIR}/core/math/perlin_noise_test.cpp"
	"${TEST_SUITE_DIR}/core/math/bounding_box_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/color_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/image_test.cpp"
	"${TEST_SUITE_DIR}/core/graphics/canvas_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/point_light_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/material_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/shape_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/group_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/csg_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/lighting_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/world_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/computed_values_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/camera_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/render_job_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/pattern_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/stripe_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/gradient_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/ring_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/checker_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/radial_gradient_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/perturbed_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/masked_test.cpp"
	"${TEST_SUITE_DIR}/core/renderer/patterns/solid_color_test.cpp"
	"${TEST_SUITE_DIR}/core/serializer/portable_pixmap_image_serializer_test.cpp"
	"${TEST_SUITE_DIR}/core/serializer/portable_network_graphics_serializer_test.cpp"
	"${TEST_SUITE_DIR}/core/serializer/wavefront_obj_deserializer_test.cpp"
	"${TEST_SUITE_DIR}/core/encryption/crc_test.cpp"
	"${TEST_SUITE_DIR}/core/encryption/adler32_test.cpp"
	"${TEST_SUITE_DIR}/core/compression/huffman_coding_test.cpp"
	"${TEST_SUITE_DIR}/core/compression/deflate_block_test.cpp"
	"${TEST_SUITE_DIR}/core/compression/zlib_test.cpp"
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

#
# GLFW
#

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory("src/lib/glfw")
target_link_libraries("${EXECUTABLE_NAME}" PRIVATE glfw)

add_subdirectory("src/lib/glad")
target_link_libraries("${EXECUTABLE_NAME}" PRIVATE glad)

#
# Concurrency Library
#

set(TF_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TF_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TF_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(TF_BUILD_CUDA OFF CACHE BOOL "" FORCE)

add_subdirectory("src/lib/taskflow")
target_link_libraries("${CORE_LIBRARY_NAME}" PRIVATE Taskflow)
