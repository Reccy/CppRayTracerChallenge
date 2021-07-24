#include <gtest/gtest.h>
#include "renderer/camera.h"
#include "renderer/world.h"
#include "math/transform.h"
#include "math/ray.h"
#include "math/vector.h"
#include "math/trig.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Camera, create_default_transformation_matrix)
{
	auto transform = Renderer::Camera::viewMatrix({ 0, 0, 0 }, { 0, 0, -1 }, Math::Vector::up());
	EXPECT_EQ(transform, Math::Transform().matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_looking_in_positive_z_direction)
{
	auto transform = Renderer::Camera::viewMatrix({ 0, 0, 0, }, { 0, 0, 1 }, Math::Vector::up());
	EXPECT_EQ(transform, Math::Transform().scale(-1, 1, -1).matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_transforms_the_world)
{
	auto transform = Renderer::Camera::viewMatrix({ 0,0,8 }, { 0,0,0 }, { 0,1,0 });
	EXPECT_EQ(transform, Math::Transform().translate(0, 0, -8).matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_arbitrary_translation)
{
	auto transform = Renderer::Camera::viewMatrix({ 1,3,2 }, { 4, -2,8 }, { 1,1,0 });
	auto expectedResult = Math::Matrix<double, 4, 4>({
		-0.507093f, 0.507093f, 0.676123f, -2.36643f,
		0.767716f, 0.606092f, 0.121218f, -2.82843f,
		-0.358569f, 0.597614f, -0.717137f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	});

	EXPECT_EQ(transform, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, creating_a_camera)
{
	Renderer::Camera camera = Renderer::Camera(160, 120, 90);

	EXPECT_EQ(camera.hSize(), 160);
	EXPECT_EQ(camera.vSize(), 120);
	EXPECT_EQ(camera.fieldOfView(), 90);
	EXPECT_EQ(camera.transformMatrix(), Math::Transform().matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, horizontal_camera_pixel_size)
{
	Renderer::Camera camera = Renderer::Camera(200, 125, 90);
	EXPECT_TRUE(Math::Comparison::equal<double>(camera.pixelSize(), 0.01));
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, vertical_camera_pixel_size)
{
	Renderer::Camera camera = Renderer::Camera(125, 200, 90);
	EXPECT_TRUE(Math::Comparison::equal<double>(camera.pixelSize(), 0.01));
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_through_center_of_camera)
{
	Renderer::Camera camera = Renderer::Camera(201, 101, 90);
	Math::Ray ray = camera.rayForPixel(100, 50);

	EXPECT_EQ(ray.origin(), Math::Point(0, 0, 0));
	EXPECT_EQ(ray.direction(), Math::Vector(0, 0, -1));
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_through_corner_of_canvas)
{
	Renderer::Camera camera = Renderer::Camera(201, 101, 90);
	Math::Ray ray = camera.rayForPixel(0, 0);

	EXPECT_EQ(ray.origin(), Math::Point(0, 0, 0));
	EXPECT_EQ(ray.direction(), Math::Vector(0.66519, 0.33259, -0.66851));
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_when_camera_is_transformed)
{
	Renderer::Camera camera = Renderer::Camera(201, 101, 90);
	Math::Transform transform = Math::Transform()
		.translate(0, -2, 5)
		.rotate(0, Math::Trig::radians_to_degrees(Math::Trig::PI / 4), 0);
	camera.transform(transform);

	Math::Ray ray = camera.rayForPixel(100, 50);

	EXPECT_EQ(ray.origin(), Math::Point(0, 2, -5));
	EXPECT_EQ(ray.direction(), Math::Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, render_world_with_camera)
{
	Renderer::World world = Renderer::World::defaultWorld();
	Renderer::Camera camera = Renderer::Camera(11, 11, 90);

	auto cameraTransform = Renderer::Camera::viewMatrix({ 0, 0, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
	camera.transform(cameraTransform);

	auto image = camera.render(world);

	EXPECT_EQ(image.readPixel(5, 5), Graphics::Color(0.38066f, 0.47583f, 0.2855f));
}
