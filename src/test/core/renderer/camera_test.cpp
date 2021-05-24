#include <gtest/gtest.h>
#include "renderer/camera.h"
#include "math/transform.h"
#include "math/vector.h"
#include "math/trig.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Camera, create_default_transformation_matrix)
{
	Math::Matrix<double> transform = Renderer::Camera::viewMatrix({ 0, 0, 0 }, { 0, 0, -1 }, Math::Vector::up());
	EXPECT_EQ(transform, Math::Transform().matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_looking_in_positive_z_direction)
{
	Math::Matrix<double> transform = Renderer::Camera::viewMatrix({ 0, 0, 0, }, { 0, 0, 1 }, Math::Vector::up());
	EXPECT_EQ(transform, Math::Transform().scale(-1, 1, -1).matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_transforms_the_world)
{
	Math::Matrix<double> transform = Renderer::Camera::viewMatrix({ 0,0,8 }, { 0,0,0 }, { 0,1,0 });
	EXPECT_EQ(transform, Math::Transform().translate(0, 0, -8).matrix());
}

TEST(CppRayTracerChallenge_Core_Renderer_Camera, view_matrix_arbitrary_translation)
{
	Math::Matrix<double> transform = Renderer::Camera::viewMatrix({ 1,3,2 }, { 4, -2,8 }, { 1,1,0 });
	Math::Matrix<double> expectedResult = Math::Matrix<double>(4, 4, std::vector<double> {
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
