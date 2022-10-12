#include <gtest/gtest.h>
#include <RML.h>
#include "renderer/camera.h"
#include "renderer/world.h"
#include "math/ray.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	TEST(CppRayTracerChallenge_Core_Renderer_Camera, create_default_transformation_matrix)
	{
		auto transform = Camera::viewMatrix({ 0, 0, 0 }, { 0, 0, -1 }, RML::Vector::up());
		EXPECT_EQ(transform, Math::Transform().matrix());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_looking_in_positive_z_direction)
	{
		auto transform = Camera::viewMatrix({ 0, 0, 0, }, { 0, 0, 1 }, RML::Vector::up());
		EXPECT_EQ(transform, Math::Transform().scale(-1, 1, -1).matrix());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_transforms_the_world)
	{
		auto transform = Camera::viewMatrix({ 0,0,8 }, { 0,0,0 }, { 0,1,0 });
		EXPECT_EQ(transform, Math::Transform().translate(0, 0, -8).matrix());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_arbitrary_translation)
	{
		auto transform = Camera::viewMatrix({ 1,3,2 }, { 4, -2,8 }, { 1,1,0 });
		auto expectedResult = RML::Matrix<double, 4, 4>({
			-0.514496f, 0.514496f, 0.685994f, -2.40098f,
			0.778924f, 0.61494f, 0.122988f, -2.86972f,
			-0.358569f, 0.597614f, -0.717137f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			});

		EXPECT_EQ(transform, expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, creating_a_camera)
	{
		Camera camera = Camera(160, 120, 90);

		EXPECT_EQ(camera.hSize(), 160);
		EXPECT_EQ(camera.vSize(), 120);
		EXPECT_EQ(camera.fieldOfView(), 90);
		EXPECT_EQ(camera.transformMatrix(), Math::Transform().matrix());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, horizontal_camera_pixel_size)
	{
		Camera camera = Camera(200, 125, 90);
		EXPECT_TRUE(Math::Comparison::equal<double>(camera.pixelSize(), 0.01));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, vertical_camera_pixel_size)
	{
		Camera camera = Camera(125, 200, 90);
		EXPECT_TRUE(Math::Comparison::equal<double>(camera.pixelSize(), 0.01));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_through_center_of_camera)
	{
		Camera camera = Camera(201, 101, 90);
		Math::Ray ray = camera.rayForPixel(100, 50);

		EXPECT_EQ(ray.origin(), RML::Point(0, 0, 0));
		EXPECT_EQ(ray.direction(), RML::Vector(0, 0, -1));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_through_corner_of_canvas)
	{
		Camera camera = Camera(201, 101, 90);
		Math::Ray ray = camera.rayForPixel(0, 0);

		EXPECT_EQ(ray.origin(), RML::Point(0, 0, 0));
		EXPECT_EQ(ray.direction(), RML::Vector(0.66519, 0.33259, -0.66851));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, construct_ray_when_camera_is_transformed)
	{
		Camera camera = Camera(201, 101, 90);
		Math::Transform transform = Math::Transform()
			.translate(0, -2, 5)
			.rotate(0, RML::Trig::radians_to_degrees(RML::Trig::PI / 4), 0);
		camera.transform(transform);

		Math::Ray ray = camera.rayForPixel(100, 50);

		EXPECT_EQ(ray.origin(), RML::Point(0, 2, -5));
		EXPECT_EQ(ray.direction(), RML::Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Camera, render_world_with_camera)
	{
		Renderer::World world = Renderer::World::defaultWorld();
		Camera camera = Camera(11, 11, 90);

		auto cameraTransform = Camera::viewMatrix({ 0, 0, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
		camera.transform(cameraTransform);

		auto image = camera.render(world);

		EXPECT_EQ(image.readPixel(5, 5), Graphics::Color(0.38066f, 0.47583f, 0.2855f));
	}
}
