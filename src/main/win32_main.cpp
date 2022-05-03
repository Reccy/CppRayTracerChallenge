#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>
#include <chrono>
#include <thread>

#undef DIFFERENCE

#include <glad.h>
#include <GLFW/glfw3.h>

#include <ryml_std.hpp>
#include <ryml.hpp>

#include "math/vector.h"
#include "math/point.h"
#include "math/transform.h"
#include "math/trig.h"
#include "math/sphere.h"
#include "math/plane.h"
#include "math/cube.h"
#include "math/cylinder.h"
#include "math/cone.h"
#include "math/intersections.h"
#include "math/ray.h"
#include "math/transform.h"
#include "graphics/canvas.h"
#include "graphics/color.h"
#include "renderer/world.h"
#include "renderer/material.h"
#include "renderer/point_light.h"
#include "renderer/lighting.h"
#include "renderer/shape.h"
#include "renderer/group.h"
#include "renderer/csg.h"
#include "renderer/camera.h"
#include "renderer/patterns/solid_color.h"
#include "renderer/patterns/stripe.h"
#include "renderer/patterns/ring.h"
#include "renderer/patterns/gradient.h"
#include "renderer/patterns/radial_gradient.h"
#include "renderer/patterns/checker.h"
#include "renderer/patterns/perturbed.h"
#include "renderer/patterns/masked.h"
#include "serializer/base_image_serializer.h"
#include "serializer/portable_pixmap_image_serializer.h"
#include "serializer/portable_network_graphics_serializer.h"
#include "serializer/wavefront_obj_deserializer.h"
#include "serializer/camera_deserializer.h"
#include "serializer/world_deserializer.h"
#include "helpers/material_helper.h"

using namespace CppRayTracerChallenge::Core;
using namespace CppRayTracerChallenge::Core::Serializer;

using namespace Renderer;
using namespace Renderer::Patterns;

using Math::Trig::PI;

using Graphics::Color;
using Graphics::Image;
using Graphics::Canvas;

constexpr double RENDER_QUALITY = 1;

std::shared_ptr<Renderer::World> world = nullptr;
std::shared_ptr<Renderer::Camera> camera = nullptr;

void log(std::string message)
{
	std::cout << message << std::endl;
}
/*
class WorldA
{
public:
	static World build()
	{
		auto floor = buildFloor();
		auto middleSphere = buildMiddleSphere();
		auto rightSphere = buildRightSphere();
		auto leftSphere = buildLeftSphere();
		auto cubeA = buildCubeA();
		auto cubeB = buildCubeB();
		auto cylinderA = buildCylinderA();
		auto coneA = buildConeA();
		PointLight light = buildLight();
		
		World world = World();

		world.defaultRemainingCalls = 32;

		Group bvhGroup = Group();
		bvhGroup.addChild(middleSphere);
		bvhGroup.addChild(rightSphere);
		bvhGroup.addChild(leftSphere);
		bvhGroup.addChild(cubeA);
		bvhGroup.addChild(cubeB);
		bvhGroup.addChild(cylinderA);
		bvhGroup.addChild(coneA);

		bvhGroup.divide(3);

		world.addGroup(bvhGroup);
		world.addLight(light);
		world.addObject(*floor);

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = 0;
		double camPosY = 1.5;
		double camPosZ = -10;

		double camLookX = 0;
		double camLookY = 1;
		double camLookZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 70;
	}
private:
	static std::shared_ptr<Renderer::Pattern> buildFloorPattern()
	{
		std::shared_ptr<Pattern> a = std::make_shared<Stripe>(Color(0.5f, 0, 0), Color(0.2f, 0, 0));
		a->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> b = std::make_shared<Stripe>(Color(0, 0, 0.5f), Color(0, 0, 0.2f));
		b->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> masked = std::make_shared<Masked<Checker>>(a, b);
		masked->transform(Math::Transform().rotate(0, 23, 0).translate(0, 0.01f, 0));

		std::shared_ptr<Pattern> pattern = std::make_shared<Perturbed>(masked);
		return pattern;
	}

	static std::shared_ptr<Renderer::Shape> buildFloor()
	{
		auto shape = std::make_shared<Plane>();
		Renderer::Shape floor = Renderer::Shape(shape);
		Transform floorTransform = Transform()
			.scale(10, 0.01, 10);
		Material bgMaterial = Material();
		bgMaterial.pattern = buildFloorPattern();
		bgMaterial.specular = 0.1f;
		bgMaterial.reflective = 0.95f;
		floor.material(bgMaterial);
		floor.transform(floorTransform);
		return std::make_shared<Renderer::Shape>(floor);
	}

	static std::shared_ptr<Renderer::Shape> buildMiddleSphere()
	{
		auto shape = std::make_shared<Sphere>();
		Renderer::Shape sphere = Renderer::Shape(shape);
		Math::Transform transform = Math::Transform()
			.translate(-0.5, 1, 0.5);
		Renderer::Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(0.1f, 1.0f, 0.5f));
		material.diffuse = 0.7f;
		material.specular = 0.3f;
		material.reflective = 0.5f;
		sphere.material(material);
		sphere.transform(transform);

		return std::make_shared<Renderer::Shape>(sphere);
	}

	static std::shared_ptr<Renderer::Shape> buildRightSphere()
	{
		auto shape = std::make_shared<Sphere>(Sphere());
		Renderer::Shape sphere = Renderer::Shape(shape);
		Transform transform = Transform()
			.scale(0.5, 0.5, 0.5)
			.translate(1.5, 0.5, -0.5);
		Material material = Material();
		material.pattern = std::make_shared<Stripe>(Color(0.5f, 1.0f, 0.1f), Color(0.7f, 0.02f, 0.6f));
		material.pattern->transform(Transform().rotate(66, 283, 1).scale(2, 1.3, 0.2));
		material.diffuse = 0.7f;
		material.specular = 0.3f;
		material.reflective = 0.2f;
		sphere.material(material);
		sphere.transform(transform);

		return std::make_shared<Renderer::Shape>(sphere);
	}

	static std::shared_ptr<Renderer::Shape> buildLeftSphere()
	{
		auto shape = std::make_shared<Sphere>(Sphere());
		Renderer::Shape sphere = Renderer::Shape(shape);
		Transform transform = Transform()
			.scale(0.33, 0.33, 0.33)
			.translate(-1.5, 0.33, -0.75);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(0.95f, 0.94f, 1.0f));
		material.pattern->transform(Transform().rotate(33, 24, 93).scale(0.2, 1.3, 0.2));
		material.diffuse = 0.2f;
		material.specular = 0.15f;
		material.reflective = 0.9f;
		material.refractiveIndex = 1.34f;
		material.transparency = 0.65f;
		sphere.material(material);
		sphere.transform(transform);

		return std::make_shared<Renderer::Shape>(sphere);
	}

	static std::shared_ptr<Renderer::Shape> buildCubeA()
	{
		auto shape = std::make_shared<Cube>();
		Renderer::Shape cube = Renderer::Shape(shape);
		Transform transform = Transform()
			.translate(5.3, 7, 1.5)
			.rotate(45, 0, 23)
			.scale(0.8, 0.8, 0.8);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(0.23f, 0.56f, 0.9f));
		material.diffuse = 0.5f;
		material.specular = 0.2f;
		material.reflective = 0.2f;
		cube.material(material);
		cube.transform(transform);

		return std::make_shared<Renderer::Shape>(cube);
	}

	static std::shared_ptr<Renderer::Shape> buildCubeB()
	{
		auto shape = std::make_shared<Cube>();
		Renderer::Shape cube = Renderer::Shape(shape);
		Transform transform = Transform()
			.translate(-3.3, 0, 8.5)
			.scale(2, 5, 2.5)
			.rotate(0, 0, 0);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(0.65f, 0.21f, 0.23f));
		cube.material(material);
		cube.transform(transform);

		return std::make_shared<Renderer::Shape>(cube);
	}

	static std::shared_ptr<Renderer::Shape> buildCylinderA()
	{
		auto shape = std::make_shared<Cylinder>(1, 3, true);
		Renderer::Shape cylinder = Renderer::Shape(shape);
		Transform transform = Transform()
			.translate(0, 1, 2)
			.rotate(20, 20, 20);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(1, 0, 1));
		cylinder.material(material);
		cylinder.transform(transform);

		return std::make_shared<Renderer::Shape>(cylinder);
	}

	static std::shared_ptr<Renderer::Shape> buildConeA()
	{
		auto shape = std::make_shared<Cone>(-1, 1, true);
		Renderer::Shape cone = Renderer::Shape(shape);
		Transform transform = Transform()
			.rotate(40, 30, 20)
			.translate(-4.5, 2.5, 3);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(1, 1, 0));
		cone.material(material);
		cone.transform(transform);

		return std::make_shared<Renderer::Shape>(cone);
	}

	static PointLight buildLight()
	{
		return PointLight({ -10, 10, -10 }, Color(1, 1, 1));
	}
};

class WorldB
{
public:
	static World build()
	{
		World world = World();

		world.addObject(buildFloor());
		world.addObject(buildWaterPlane());
		world.addObject(buildBackWall());
		world.addObject(buildMarble());
		world.addLight(buildLight(0, 4, 5));

		world.defaultRemainingCalls = 8;

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = 0;
		double camPosY = 3;
		double camPosZ = -8;

		double camLookX = 0;
		double camLookY = 3;
		double camLookZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 50;
	}
private:
	static Renderer::Shape buildFloor()
	{
		auto floorMat = Material();
		floorMat.pattern = std::make_shared<Checker>(Graphics::Color::black(), Graphics::Color::white());
		floorMat.shininess = 0.98f;
		floorMat.reflective = 0.74f;
		floorMat.specular = 0.3f;

		auto floorShape = std::make_shared<Math::Plane>();
		
		Renderer::Shape floor = Renderer::Shape(floorShape, floorMat);
		floor.transform(Math::Transform().translate(0, 0, 0));
		return floor;
	}

	static Renderer::Shape buildWaterPlane()
	{
		auto floorMat = Helpers::MaterialHelper::glassSphere().material();
		auto floorShape = std::make_shared<Math::Plane>();
		floorMat.pattern = std::make_shared<SolidColor>(Graphics::Color(0.1f, 0.1f, 0.1f));
		floorMat.reflective = 1.2f;
		floorMat.diffuse = 0.001f;

		Renderer::Shape floor = Renderer::Shape(floorShape, floorMat);
		floor.transform(Math::Transform().translate(0, 0.5f + Math::EPSILON * 2, 0));
		return floor;
	}

	static Renderer::Shape buildBackWall()
	{
		auto stripePattern = std::make_shared<Stripe>(Graphics::Color(25.0f/255.0f, 158.0f/255.0f, 170.0f/255.0f), Graphics::Color(166.0f/255.0f, 228.0f/255.0f, 234.0f/255.0f));

		auto wallMat = Material();
		wallMat.pattern = stripePattern;
		wallMat.reflective = 0.05f;
		wallMat.refractiveIndex = 1.23f;
		wallMat.diffuse = 0.84f;

		auto wallShape = std::make_shared<Math::Plane>();

		Renderer::Shape wall = Renderer::Shape(wallShape, wallMat);
		wall.transform(Math::Transform().rotate(82, 3, 180).translate(0,0,10));

		return wall;
	}

	static Renderer::Shape buildMarble()
	{
		auto ballMat = Helpers::MaterialHelper::glassSphere().material();
		ballMat.diffuse = 0.0005f;
		auto ballShape = std::make_shared<Math::Sphere>();

		Renderer::Shape ball = Renderer::Shape(ballShape, ballMat);
		ball.transform(Math::Transform().scale(2, 2, 2).translate(0, 4, 5));

		return ball;
	}

	static PointLight buildLight(double x, double y, double z)
	{
		return PointLight({ x, y, z }, Color(0.98f, 0.95f, 0.94f));
	}
};

class WorldC
{
public:
	static const int HEX_WIDTH = 10;
	static const int SPACING = 4;

	static World build()
	{
		World world = World();

		Group root = Group();

		for (int y = 0; y < HEX_WIDTH; ++y)
		{
			auto row = std::make_shared<Group>();

			for (int x = 0; x < HEX_WIDTH; ++x)
			{
				for (int z = 0; z < HEX_WIDTH; ++z)
				{
					auto hex = buildHexagon();
					hex->transform(Transform()
						.rotate(0, 25, 30)
						.translate(x * SPACING, y * SPACING, z * SPACING));
					row->addChild(hex);
				}
			}

			root.addChild(row);
		}

		//root.transform(root.transform().translate(0, 1, 0));

		world.addGroup(root);
		world.addObject(*buildBackdrop());
		world.addLight(buildLight(0, 40, -10));

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		constexpr double X_OFFSET = -20;
		constexpr double Y_OFFSET = 15;
		constexpr double Z_OFFSET = 0;

		constexpr double X_LOOK_OFFSET = 0;
		constexpr double Y_LOOK_OFFSET = -5;
		constexpr double Z_LOOK_OFFSET = 0;

		double camPosX = X_OFFSET + ((HEX_WIDTH * SPACING) / 2);
		double camPosY = Y_OFFSET + ((HEX_WIDTH * SPACING) / 2);
		double camPosZ = Z_OFFSET + (-((HEX_WIDTH * SPACING) / 2));

		double camLookX = X_LOOK_OFFSET + ((HEX_WIDTH * SPACING) / 2);
		double camLookY = Y_LOOK_OFFSET + ((HEX_WIDTH * SPACING) / 2);
		double camLookZ = Z_LOOK_OFFSET + ((HEX_WIDTH * SPACING) / 2);

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 120;
	}
private:
	static std::shared_ptr<Renderer::Shape> buildBackdrop()
	{
		auto plane = std::make_shared<Plane>();
		auto shape = std::make_shared<Renderer::Shape>(plane);

		shape->transform(Transform().rotate(0, 90, 0).translate(0, -1, 0));

		auto mat = Material();
		mat.pattern = std::make_shared<SolidColor>(Color(0.23f, 0.1f, 1.0f));

		shape->material(mat);

		return shape;
	}

	static std::shared_ptr<Group> buildHexagon()
	{
		auto result = std::make_shared<Group>();

		for (int i = 0; i < 6; ++i)
		{
			auto side = buildHexagonSide();
			side->transform(Transform()
				.rotate(0, Math::Trig::radians_to_degrees(i * PI / 3.0), 0));
			result->addChild(side);
		}

		return result;
	}

	static std::shared_ptr<Group> buildHexagonSide()
	{
		auto result = std::make_shared<Group>();

		result->addChild(buildHexagonEdge());
		result->addChild(buildHexagonCorner());

		return result;
	}

	static std::shared_ptr<Renderer::Shape> buildHexagonEdge()
	{
		auto cylinder = std::make_shared<Cylinder>(0, 1);
		auto result = std::make_shared<Renderer::Shape>(cylinder);

		result->transform(Transform()
			.scale(0.25, 1, 0.25)
			.rotate(0, 0, Math::Trig::radians_to_degrees(-PI / 2))
			.rotate(0, Math::Trig::radians_to_degrees(-PI / 6), 0)
			.translate(0, 0, -1));

		return result;
	}

	static std::shared_ptr<Renderer::Shape> buildHexagonCorner()
	{
		auto sphere = std::make_shared<Sphere>();
		auto result = std::make_shared<Renderer::Shape>(sphere);

		result->transform(Transform()
			.scale(0.25, 0.25, 0.25)
			.translate(0, 0, -1));

		return result;
	}

	static PointLight buildLight(double x, double y, double z)
	{
		return PointLight({ x, y, z }, Color(0.98f, 0.95f, 0.94f));
	}
};

class WorldD
{
public:
	static World build()
	{
		auto floor = buildFloor();
		PointLight light = buildLight();

		World world = World();

		world.defaultRemainingCalls = 32;

		Group bvhGroup = Group();
		
		auto model = buildModel();
		bvhGroup.addChild(model);

		world.addGroup(bvhGroup);
		world.addLight(light);
		world.addObject(*floor);

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = 0;
		double camPosY = 0.5;
		double camPosZ = -1;

		double camLookX = 0;
		double camLookY = 0;
		double camLookZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 90;
	}
private:
	static std::shared_ptr<Renderer::Pattern> buildFloorPattern()
	{
		std::shared_ptr<Pattern> a = std::make_shared<Stripe>(Color(0.5f, 0.5f, 0), Color(0.2f, 0.5, 0.4f));
		a->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> b = std::make_shared<Stripe>(Color(0.5f, 0.5f, 0.f), Color(0.5f, 0.3f, 0.5f));
		b->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> masked = std::make_shared<Masked<Checker>>(a, b);
		masked->transform(Math::Transform().rotate(0, 65, 0).translate(0, 0.01f, 0));

		std::shared_ptr<Pattern> pattern = std::make_shared<Perturbed>(masked);
		return pattern;
	}

	static std::shared_ptr<Renderer::Shape> buildFloor()
	{
		auto shape = std::make_shared<Plane>();
		Renderer::Shape floor = Renderer::Shape(shape);
		Transform floorTransform = Transform()
			.scale(2, 1, 10);
		Material bgMaterial = Material();
		bgMaterial.pattern = buildFloorPattern();
		bgMaterial.specular = 0.1f;
		bgMaterial.reflective = 0.95f;
		floor.material(bgMaterial);
		floor.transform(floorTransform);
		return std::make_shared<Renderer::Shape>(floor);
	}

	static std::shared_ptr<Renderer::Group> buildModel()
	{
		log("Loading model.obj");

		std::ifstream file("model.obj", std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (size < 0)
		{
			std::cerr << "FATAL: Cannot read model file";
			exit(0);
		}

		std::vector<char> buffer(size);
		if (!file.read(buffer.data(), size))
		{
			std::cerr << "FATAL: Cannot read model file";
			exit(0);
		}

		WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

		serializer.deserialize(buffer);

		log("===== Load results =====");
		std::stringstream ss1;
		ss1 << "> Model Instructions Ignored: " << serializer.ignoredLines();
		log(ss1.str());

		std::stringstream ss2;
		ss2 << "> Model Size: " << serializer.buffer().size();
		log(ss2.str());
		log("========================\n");

		log("== Subdividing Model ==");
		auto subresult = std::make_shared<Renderer::Group>(serializer.defaultGroup());
		subresult->divide(30);
		log("Done");
		log("=======================\n");

		auto result = std::make_shared<Renderer::Group>();
		result->addChild(subresult);

		result->transform(result->transform().scale(0.1, 0.1, 0.1));

		return result;
	}

	static PointLight buildLight()
	{
		return PointLight({ 10, 10, -10 }, Color(1, 1, 1));
	}
};

class WorldE
{
public:
	static World build()
	{
		auto floor = buildFloor();
		auto csgObj = buildCSG();
		auto csgObj2 = buildCSG();
		auto room = buildRoom();
		PointLight light = buildLight();

		World world = World();

		world.defaultRemainingCalls = 32;
		
		room->transform(room->transform().scale(0.5, 1, 1));

		csgObj->transform(csgObj->transform().rotate(-30, 0, 0));
		csgObj2->transform(csgObj2->transform().rotate(-30, 20, 40).scale(0.2, 0.2, 0.2).translate(0, 2, 1));

		auto mat = Helpers::MaterialHelper::glassSphere().material();
		mat.diffuse = 0.1f;
		mat.transparency = 0.9f;

		csgObj2->left()->material(mat);
		csgObj2->right()->material(mat);

		world.addCSG(*csgObj);
		world.addCSG(*csgObj2);
		world.addLight(light);
		world.addObject(*room);
		world.addObject(*floor);

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = 1.2;
		double camPosY = 9;
		double camPosZ = -7;

		double camLookX = 1.5;
		double camLookY = 2;
		double camLookZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 60;
	}
private:
	static std::shared_ptr<Renderer::Shape> buildFloor()
	{
		auto shape = std::make_shared<Plane>();
		Renderer::Shape floor = Renderer::Shape(shape);
		Transform floorTransform = Transform()
			.scale(10, 0.01, 10);
		Material bgMaterial = Material();
		bgMaterial.pattern = std::make_shared<SolidColor>(Color(0.2f, 0.4f, 0.5f));
		bgMaterial.specular = 0.1f;
		bgMaterial.reflective = 0.95f;
		floor.material(bgMaterial);
		floor.transform(floorTransform);
		return std::make_shared<Renderer::Shape>(floor);
	}

	static std::shared_ptr<Renderer::CSG> buildCSG()
	{
		auto sphere = buildCSGSphere();
		auto cylinder = buildCSGCylinder();
		
		auto root = CSG::build(CSG::Operation::DIFFERENCE, sphere, cylinder);

		root->transform(Transform().translate(0, 3, 2));

		return root;
	}

	static std::shared_ptr<Renderer::Shape> buildCSGSphere()
	{
		auto sphere = std::make_shared<Sphere>();
		auto result = std::make_shared<Renderer::Shape>(sphere);

		auto mat = Material();
		mat.pattern = std::make_shared<SolidColor>(Color(0.9f, 0.1f, 0.1f));

		result->material(mat);

		result->transform(Transform().scale(2, 2, 2));

		return result;
	}

	static std::shared_ptr<Renderer::Shape> buildCSGCylinder()
	{
		auto sphere = std::make_shared<Cylinder>(-2, 2);
		auto result = std::make_shared<Renderer::Shape>(sphere);

		auto mat = Material();
		mat.pattern = std::make_shared<SolidColor>(Color(0.8f, 0.2f, 0.2f));

		result->material(mat);

		result->transform(Transform().scale(0.75, 0, 0.75));

		return result;
	}

	static std::shared_ptr<Renderer::Shape> buildConeA()
	{
		auto shape = std::make_shared<Cone>(-1, 1, true);
		Renderer::Shape cone = Renderer::Shape(shape);
		Transform transform = Transform()
			.rotate(40, 30, 20)
			.translate(-4.5, 2.5, 3);
		Material material = Material();
		material.pattern = std::make_shared<SolidColor>(Color(1, 1, 0));
		cone.material(material);
		cone.transform(transform);

		return std::make_shared<Renderer::Shape>(cone);
	}

	static std::shared_ptr<Renderer::Shape> buildRoom()
	{
		auto shape = std::make_shared<Cube>();
		auto cube = Renderer::Shape(shape);
		Transform transform = Transform()
			//.translate(0, 1, -1)
			.scale(100, 100, 100);
		Material material = Material();
		material.pattern = std::make_shared<Stripe>(Color(0.2f, 0.3f, 0.7f), Color(0.2f, 0.4f, 0.8f));
		material.pattern->transform(Transform().rotate(30, 30, 30).scale(0.25, 1, 0.25));
		cube.material(material);
		cube.transform(transform);

		return std::make_shared<Renderer::Shape>(cube);
	}

	static PointLight buildLight()
	{
		return PointLight({ -3, 8, -5 }, Color(0.9f, 0.9f, 1));
	}
};

class WorldF
{
public:
	static World build()
	{
		World world = World();

		// Build Light
		auto light1 = PointLight({ 50, 100, -50 }, Graphics::Color::white());
		world.addLight(light1);

		auto light2 = PointLight({ -400, 50, -10 }, Graphics::Color(0.2f, 0.2f, 0.2f));
		world.addLight(light2);

		// Define Materials
		auto whiteMaterial = Material();
		auto whiteMaterialPattern = std::make_shared<Patterns::SolidColor>(Graphics::Color::white());
		whiteMaterial.pattern = whiteMaterialPattern;
		whiteMaterial.diffuse = 0.7f;
		whiteMaterial.ambient = 0.1f;
		whiteMaterial.specular = 0.0f;
		whiteMaterial.reflective = 0.1f;

		auto blueMaterial = whiteMaterial;
		auto blueMaterialPattern = std::make_shared<Patterns::SolidColor>(Graphics::Color(0.537f, 0.831f, 0.914f));
		blueMaterial.pattern = blueMaterialPattern;

		auto redMaterial = whiteMaterial;
		auto redMaterialPattern = std::make_shared<Patterns::SolidColor>(Graphics::Color(0.941f, 0.322f, 0.388f));
		redMaterial.pattern = redMaterialPattern;

		auto purpleMaterial = whiteMaterial;
		auto purpleMaterialPattern = std::make_shared<Patterns::SolidColor>(Graphics::Color(0.373f, 0.404f, 0.550f));
		purpleMaterial.pattern = purpleMaterialPattern;

		auto baseTransform = Transform()
			.translate(1, -1, 1)
			.scale(0.5, 0.5, 0.5);

		auto largeObject = baseTransform
			.scale(3.5, 3.5, 3.5);

		auto mediumObject = baseTransform
			.scale(3, 3, 3);

		auto smallObject = baseTransform
			.scale(2, 2, 2);

		// BG Plane
		auto plane = Renderer::Shape::build<Plane>();
		auto planeMat = Material();
		planeMat.pattern = std::make_shared<SolidColor>(Graphics::Color { 1,1,1 });
		planeMat.ambient = 1;
		planeMat.diffuse = 0;
		planeMat.specular = 0;
		plane->material(planeMat);
		plane->transform(Transform()
			.rotate(Trig::radians_to_degrees(PI / 2), 0, 0)
			.translate(0, 0, 500)
		);
		world.addObject(plane);

		// Scene Elements
		auto group = Group();

		auto s1 = Renderer::Shape::build<Sphere>();
		auto s1Mat = Material();
		s1Mat.pattern = std::make_shared<SolidColor>(Graphics::Color{0.373f, 0.404f, 0.550f});
		s1Mat.diffuse = 0.2f;
		s1Mat.ambient = 0;
		s1Mat.specular = 1.0f;
		s1Mat.shininess = 200;
		s1Mat.reflective = 0.7f;
		s1Mat.transparency = 0.7f;
		s1Mat.refractiveIndex = 1.5f;
		s1Mat.shadowcastMode = ShadowcastMode::ALWAYS;
		s1->material(s1Mat);
		s1->transform(largeObject);
		group.addObject(s1);

		auto c1 = Renderer::Shape::build<Cube>();
		c1->material(whiteMaterial);
		c1->transform(mediumObject.translate(4, 0, 0));
		group.addObject(c1);

		auto c2 = Renderer::Shape::build<Cube>();
		c2->material(blueMaterial);
		c2->transform(largeObject.translate(8.5, 1.5, -0.5));
		group.addObject(c2);

		auto c3 = Renderer::Shape::build<Cube>();
		c3->material(redMaterial);
		c3->transform(largeObject.translate(0, 0, 4));
		group.addObject(c3);

		auto c4 = Renderer::Shape::build<Cube>();
		c4->material(whiteMaterial);
		c4->transform(smallObject.translate(4, 0, 4));
		group.addObject(c4);

		auto c5 = Renderer::Shape::build<Cube>();
		c5->material(purpleMaterial);
		c5->transform(mediumObject.translate(7.5, 0.5, 4));
		group.addObject(c5);

		auto c6 = Renderer::Shape::build<Cube>();
		c6->material(whiteMaterial);
		c6->transform(mediumObject.translate(-0.25, 0.25, 8));
		group.addObject(c6);

		auto c7 = Renderer::Shape::build<Cube>();
		c7->material(blueMaterial);
		c7->transform(largeObject.translate(4, 1, 7.5));
		group.addObject(c7);

		auto c8 = Renderer::Shape::build<Cube>();
		c8->material(redMaterial);
		c8->transform(mediumObject.translate(10, 2, 7.5));
		group.addObject(c8);

		auto c9 = Renderer::Shape::build<Cube>();
		c9->material(whiteMaterial);
		c9->transform(smallObject.translate(8, 2, 12));
		group.addObject(c9);

		auto c10 = Renderer::Shape::build<Cube>();
		c10->material(whiteMaterial);
		c10->transform(smallObject.translate(20, 1, 9));
		group.addObject(c10);

		auto c11 = Renderer::Shape::build<Cube>();
		c11->material(blueMaterial);
		c11->transform(largeObject.translate(-0.5, -5, 0.25));
		group.addObject(c11);

		auto c12 = Renderer::Shape::build<Cube>();
		c12->material(redMaterial);
		c12->transform(largeObject.translate(4, -4, 0));
		group.addObject(c12);

		auto c13 = Renderer::Shape::build<Cube>();
		c13->material(whiteMaterial);
		c13->transform(largeObject.translate(8.5, -4, 0));
		group.addObject(c13);

		auto c14 = Renderer::Shape::build<Cube>();
		c14->material(whiteMaterial);
		c14->transform(largeObject.translate(0, -4, 4));
		group.addObject(c14);

		auto c15 = Renderer::Shape::build<Cube>();
		c15->material(purpleMaterial);
		c15->transform(largeObject.translate(-0.5, -4.5, 8));
		group.addObject(c15);

		auto c16 = Renderer::Shape::build<Cube>();
		c16->material(whiteMaterial);
		c16->transform(largeObject.translate(0, -8, 4));
		group.addObject(c16);

		auto c17 = Renderer::Shape::build<Cube>();
		c17->material(whiteMaterial);
		c17->transform(largeObject.translate(-0.5, -8.5, 8));
		group.addObject(c17);

		group.divide(3);

		world.addGroup(group);
	
		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = -6;
		double camPosY = 6;
		double camPosZ = -10;

		double camLookX = 6;
		double camLookY = 0;
		double camLookZ = 6;

		double camUpX = -0.45;
		double camUpY = 1;
		double camUpZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, { camUpX, camUpY, camUpZ });
	}

	static int fov()
	{
		return 80;
	}
};

class WorldG
{
public:
	static World build()
	{
		auto floor = buildFloor();
		PointLight light = buildLight();

		World world = World();

		world.defaultRemainingCalls = 8;

		world.addObject(*floor);
		world.addLight(light);
		world.addObject(*floor);

		return world;
	}

	static Matrix<double, 4, 4> cameraMatrix()
	{
		double camPosX = 0;
		double camPosY = 1.5;
		double camPosZ = -10;

		double camLookX = 0;
		double camLookY = 1;
		double camLookZ = 0;

		return Camera::viewMatrix({ camPosX, camPosY, camPosZ }, { camLookX, camLookY, camLookZ }, Vector::up());
	}

	static int fov()
	{
		return 70;
	}
private:
	static std::shared_ptr<Renderer::Pattern> buildFloorPattern()
	{
		std::shared_ptr<Pattern> a = std::make_shared<Stripe>(Color(0.5f, 0, 0), Color(0.2f, 0, 0));
		a->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> b = std::make_shared<Stripe>(Color(0, 0, 0.5f), Color(0, 0, 0.2f));
		b->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

		std::shared_ptr<Pattern> masked = std::make_shared<Masked<Checker>>(a, b);
		masked->transform(Math::Transform().rotate(0, 23, 0).translate(0, 0.01f, 0));

		std::shared_ptr<Pattern> pattern = std::make_shared<Perturbed>(masked);
		return pattern;
	}

	static std::shared_ptr<Renderer::Shape> buildFloor()
	{
		auto shape = std::make_shared<Plane>();
		Renderer::Shape floor = Renderer::Shape(shape);
		Transform floorTransform = Transform()
			.scale(10, 0.01, 10);
		Material bgMaterial = Material();
		bgMaterial.pattern = buildFloorPattern();
		bgMaterial.specular = 0.1f;
		bgMaterial.reflective = 0.95f;
		floor.material(bgMaterial);
		floor.transform(floorTransform);
		return std::make_shared<Renderer::Shape>(floor);
	}

	static PointLight buildLight()
	{
		return PointLight({ -10, 10, -10 }, Color(1, 1, 1));
	}
};

*/

Image doRealRender()
{
	auto startTime = std::chrono::high_resolution_clock::now();

	log("Rendering scene -> Start: " + std::to_string(startTime.time_since_epoch().count()));
	auto result = camera->render(*world);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto elapsed = endTime - startTime;

	log("Render complete -> End: " + std::to_string(endTime.time_since_epoch().count()) + ", Elapsed: " + std::to_string(elapsed.count()));

	return result;
}

void writeImage(Image image, BaseImageSerializer& serializer)
{
	serializer.serialize(image);

	const std::vector<unsigned char> ppmBuffer = serializer.buffer();

	TCHAR appData[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
		NULL,
		SHGFP_TYPE_CURRENT,
		appData))) {
		std::basic_ostringstream<TCHAR> filePath;

		std::string imageName = std::string("\\generated_image.") + std::string(serializer.fileExtension());

		filePath << appData << _TEXT(imageName.c_str());

		std::ofstream file;
		file.open(filePath.str().c_str(), std::ios_base::binary);

		file.write((const char *)&ppmBuffer[0], ppmBuffer.size());
		file.close();

		std::cout << "File written" << std::endl;
	}
	else
	{
		std::cout << "Failed to write file" << std::endl;
	}
}

Image generatePerlin()
{
	PerlinNoise perlin = PerlinNoise();

	int canvasWidth = 1080;
	int canvasHeight = 1080;

	Canvas canvas = Canvas(canvasWidth, canvasHeight);

	double offsetX = 2348;
	double offsetY = 512304;
	double scale = 0.1;

	for (int x = 0; x < canvas.width(); ++x)
	{
		for (int y = 0; y < canvas.height(); ++y)
		{
			float r = (float)perlin.at({ static_cast<double>(x) / (canvasWidth * scale) + offsetX, static_cast<double>(y) / (canvasHeight * scale) + offsetY, 0 });
			float g = (float)perlin.at({ static_cast<double>(x) / (canvasWidth * scale) + offsetX, 0, static_cast<double>(y) / (canvasHeight * scale) + offsetY });
			float b = (float)perlin.at({ 0, static_cast<double>(x) / (canvasWidth * scale) + offsetX, static_cast<double>(y) / (canvasHeight * scale) + offsetY });

			Color color = Color(r, g, b);

			canvas.writePixel(x, y, color);
		}
	}

	return static_cast<Image>(canvas);
}

void renderTask(std::atomic<bool>* threadProgress)
{
	Image image = doRealRender();
	//PortablePixmapImageSerializer serializer;
	PortableNetworkGraphicsSerializer serializer;
	
	writeImage(image, serializer);

	threadProgress->store(true);
}

bool initGLFW()
{
	return glfwInit();
}

void endGLFW()
{
	glfwTerminate();
}

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"	TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"	FragColor = texture(ourTexture, TexCoord);\n"
"};\0";

float verts[] = {
	// positions          // colors           // texture coords
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

std::vector<byte> renderData;
byte* loadRenderData()
{
	if (!camera)
	{
		return renderData.data();
	}

	auto buffer = camera->renderedImage().toBuffer();

	renderData.clear();
	renderData.reserve(buffer.size() * 4);

	for (int colorIndex = 0; colorIndex < buffer.size(); ++colorIndex)
	{
		for (int componentIndex = 0; componentIndex < 4; ++componentIndex)
		{
			byte b = 0x0b;
			Graphics::Color& color = buffer.at(colorIndex);

			if (componentIndex == 0)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.red() * 255), 0, 255));
			}
			else if (componentIndex == 1)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.green() * 255), 0, 255));
			}
			else if (componentIndex == 2)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.blue() * 255), 0, 255));
			}
			else if (componentIndex == 3)
			{
				b = static_cast<byte>(0xFFb); // Alpha 1
			}

			renderData.push_back(b);
		}
	}

	return renderData.data();
}

class RAIIglfw
{
public:
	RAIIglfw()
	{
		m_window = glfwCreateWindow(camera->resolutionWidth(), camera->resolutionHeight(), "Ray Tracer", NULL, NULL);
		glfwMakeContextCurrent(m_window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetErrorCallback(errorCallback);
		glfwSwapInterval(1);
		
		// Shader Compilation
		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(m_vertexShader);

		int success;
		char infoLog[512];
		
		glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cout << "SUCCESS::SHADER::VERTEX::COMPILATION_COMPLETE" << std::endl;
		}

		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(m_fragmentShader);
		glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION_COMPLETE" << std::endl;
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, m_vertexShader);
		glAttachShader(m_shaderProgram, m_fragmentShader);
		glLinkProgram(m_shaderProgram);

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cout << "SUCCESS::SHADER::PROGRAM::LINKAGE_COMPLETE" << std::endl;
		}

		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);

		// Draw

		glGenBuffers(1, &m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_EBO);
		glGenTextures(1, &m_texture);
		
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		unsigned char data[16] = {
			255, 0, 0, 255,
			0, 255, 0, 255,
			0, 0, 255, 255,
			255, 255, 255, 255
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glfwSwapInterval(1);
	}

	~RAIIglfw()
	{
		glfwTerminate();
	}

	bool windowShouldOpen()
	{
		return !glfwWindowShouldClose(m_window);
	}

	void loop()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgram);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, camera->resolutionWidth(), camera->resolutionHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loadRenderData());
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
		updateFramebufferSize();

		if (!windowShouldOpen())
		{
			std::cout << "Window closed\n";
			glfwDestroyWindow(m_window);
		}
	}
private:
	GLFWwindow* m_window;
	int m_width = 0;
	int m_height = 0;
	float m_aspectRatio = 0;
	unsigned int m_VBO = 0;
	unsigned int m_VAO = 0;
	unsigned int m_EBO = 0;
	unsigned int m_vertexShader = 0;
	unsigned int m_fragmentShader = 0;
	unsigned int m_shaderProgram = 0;
	unsigned int m_texture = 0;

	void updateFramebufferSize()
	{
		int newWidth, newHeight;
		glfwGetFramebufferSize(m_window, &newWidth, &newHeight);

		if (newWidth != m_width || newHeight != m_height)
		{
			m_width = newWidth;
			m_height = newHeight;
			m_aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
			std::cout << "Framebuffer change: w" << m_width << ", h" << m_height << ", a" << m_aspectRatio << "\n";

			glViewport(0, 0, m_width, m_height);
		}
	}

	static void keyCallback(GLFWwindow* window, int key, int , int action, int )
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	static void errorCallback(int error, const char* description)
	{
		std::cout << "Error: " << std::to_string(error) << "\n" << description << "\n";
	}
};

class Options
{
public:
	Options(int argc, char* argv[])
	{
		m_exePath = std::string(argv[0]);

		for (int i = 1; i < argc; ++i)
		{
			std::string str(argv[i]);

			if (m_state == ParseState::NONE)
			{
				if (str == "--output" || str == "-o")
				{
					m_state = ParseState::READ_OUTPUT_PATH;
					continue;
				}
				else if (str == "--input" || str == "-i")
				{
					m_state = ParseState::READ_INPUT_PATH;
					continue;
				}
				else if (str == "--pipe" || str == "-p")
				{
					m_state = ParseState::READ_PIPED_INPUT;
				}
				else if (str == "--help" || str == "-h")
				{
					m_helpFlag = true;
					continue;
				}
				else
				{
					std::cout << "Unrecognised argument: " << str << std::endl;
					continue;
				}
			}
			else if (m_state == ParseState::READ_OUTPUT_PATH)
			{
				m_outputPath = str;
				m_outputSet = true;
				m_state = ParseState::NONE;
				continue;
			}
			else if (m_state == ParseState::READ_INPUT_PATH)
			{
				m_inputPath = str;
				m_inputSet = true;
				m_state = ParseState::NONE;
				continue;
			}
			else if (m_state == ParseState::READ_PIPED_INPUT)
			{
				m_inputPiped = str;
				m_inputSet = true;
				m_state = ParseState::NONE;
				continue;
			}
		}
	}

	const std::string path()
	{
		return m_exePath;
	}

	const std::string outputPath()
	{
		return m_outputPath;
	}

	const std::string inputPath()
	{
		return m_inputPath;
	}

	const std::string inputPiped()
	{
		return m_inputPiped;
	}

	const bool helpFlag()
	{
		return m_helpFlag;
	}

	const bool inputSet()
	{
		return m_inputSet;
	}

	const bool outputSet()
	{
		return m_outputSet;
	}
private:
	enum ParseState { NONE, READ_OUTPUT_PATH, READ_INPUT_PATH, READ_PIPED_INPUT };
	ParseState m_state = ParseState::NONE;

	std::vector<std::string> m_argv;
	std::string m_exePath;
	std::string m_outputPath;
	std::string m_inputPath;
	std::string m_inputPiped;
	bool m_helpFlag = false;
	bool m_inputSet = false;
	bool m_outputSet = false;
};

int main(int argc, char* argv[])
{
	// I know using comments instead of functions is """""bad code""""" but I just want to get this done for tooljam.
	// Can always refactor later!

	// Parse CLI args
	Options options(argc, argv);

	if (options.helpFlag())
	{
		std::cout << "=== MANDATORY OPTIONS ===" << "\n";
		std::cout << "Set Output File" << "\n";
		std::cout << "    Flag: --output / -o" << "\n";
		std::cout << "    Argument: Absolute path to output file" << "\n";
		std::cout << "    Optional? No" << "\n";
		std::cout << "\n";
		std::cout << "Set Input File" << "\n";
		std::cout << "    Flag: --input / -i" << "\n";
		std::cout << "    Argument: Absolute path to input file" << "\n";
		std::cout << "    Optional? Either this or inline data must be set." << "\n";
		std::cout << "\n";
		std::cout << "Pass Input File as Inline Data" << "\n";
		std::cout << "    Flag: --pipe / -p" << "\n";
		std::cout << "    Argument: Content of input file" << "\n";
		std::cout << "    Optional? Either this or input file must be set." << "\n";
		std::cout << "\n";
		std::cout << "Run this application again without the help flag to run." << "\n";
		std::cout << "\n";
		std::cout << "Exiting..." << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Ray Tracer by Aaron Meaney" << "\n";
		std::cout << "\n";
		std::cout << "Options:" << "\n";
		std::cout << "Output File: " << options.outputPath() << "\n";
		std::cout << "Input File: " << options.inputPath() << "\n";
		std::cout << "Input Pipe: " << options.inputPiped() << "\n";
		std::cout << "\n";
	}

	if (!options.inputSet() || !options.outputSet())
	{
		std::cout << "Error: Invalid Options" << "\n";

		if (!options.inputSet())
		{
			std::cout << "> Please ensure a valid input is provided. A file or file contents is sufficient." << "\n";
		}

		if (!options.outputSet())
		{
			std::cout << "> Please ensure a valid output path is provided." << "\n";
		}

		std::cout << "Resolve issues and try again. Use --help for more information." << std::endl;
		return 0;
	}

	// Load the world file and validate it
	std::ifstream ymlfile(options.inputPath());
	const std::string ymlstr((std::istreambuf_iterator<char>(ymlfile)),
		(std::istreambuf_iterator<char>()));

	camera = Serializer::CameraDeserializer(ymlstr).camera();
	world = Serializer::WorldDeserializer(ymlstr).world();

	// Init GLFW and render the image
	if (!glfwInit())
	{
		std::cout << "FATAL: GLFW init failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::atomic<bool> threadProgress(0);
	std::thread renderThread(renderTask, &threadProgress);

	RAIIglfw glfw = RAIIglfw();

	while (glfw.windowShouldOpen())
	{
		glfw.loop();
	}

	// Wait for render threads to finish before exiting the program
	while (!threadProgress.load() && !renderThread.joinable()) {}

	renderThread.join();

	return 0;
}
