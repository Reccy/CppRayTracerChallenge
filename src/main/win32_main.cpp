#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>
#include <chrono>
#include <thread>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "math/vector.h"
#include "math/point.h"
#include "math/transform.h"
#include "math/trig.h"
#include "math/sphere.h"
#include "math/plane.h"
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

using namespace CppRayTracerChallenge::Core;
using namespace CppRayTracerChallenge::Core::Serializer;

using namespace Renderer;
using namespace Renderer::Patterns;

using Graphics::Color;
using Graphics::Image;
using Graphics::Canvas;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int RENDER_WIDTH = 150;
const int RENDER_HEIGHT = 75;

void log(std::string message)
{
	std::cout << message << std::endl;
}

std::shared_ptr<Renderer::Pattern> buildFloorPattern()
{
	std::shared_ptr<Pattern> a = std::make_shared<Stripe>(Color(0.5f, 0, 0), Color(0.2f, 0, 0));
	a->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

	std::shared_ptr<Pattern> b = std::make_shared<Stripe>(Color(0, 0, 0.5f), Color(0, 0, 0.2f));
	b->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

	std::shared_ptr<Pattern> masked = std::make_shared<Masked<Checker>>(a, b);
	masked->transform(Math::Transform().rotate(0,23,0).translate(0,0.01f,0));

	std::shared_ptr<Pattern> pattern = std::make_shared<Perturbed>(masked);
	return pattern;
}

Renderer::Shape buildFloor()
{
	auto shape = std::make_shared<Plane>();
	Renderer::Shape floor = Renderer::Shape(shape);
	Transform floorTransform = Transform()
		.scale(10, 0.01, 10);
	Material bgMaterial = Material();
	bgMaterial.pattern = buildFloorPattern();
	bgMaterial.specular = 0.1f;
	floor.material(bgMaterial);
	floor.transform(floorTransform);
	return floor;
}

Renderer::Shape buildMiddleSphere()
{
	auto shape = std::make_shared<Sphere>(Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Math::Transform transform = Math::Transform()
		.translate(-0.5, 1, 0.5);
	Renderer::Material material = Material();
	material.pattern = std::make_shared<SolidColor>(Color(0.1f, 1.0f, 0.5f));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildRightSphere()
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
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildLeftSphere()
{
	auto shape = std::make_shared<Sphere>(Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Transform transform = Transform()
		.scale(0.33, 0.33, 0.33)
		.translate(-1.5, 0.33, -0.75);
	Material material = Material();
	material.pattern = std::make_shared<Stripe>(Color(1.0f, 0.8f, 0.1f), Color(0.9f, 0.2f, 0.5f));
	material.pattern->transform(Transform().rotate(33, 24, 93).scale(0.2, 1.3, 0.2));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

PointLight buildLight()
{
	return PointLight({ -10, 10, -10 }, Color(1, 1, 1));
}

Image doRealRender()
{
	log("Initializing...");

	log("Building Floor...");
	Renderer::Shape floor = buildFloor();

	log("Building Middle Sphere...");
	Renderer::Shape middleSphere = buildMiddleSphere();

	log("Building Right Sphere...");
	Renderer::Shape rightSphere = buildRightSphere();

	log("Building Left Sphere...");
	Renderer::Shape leftSphere = buildLeftSphere();

	log("Building Light...");
	PointLight light = buildLight();

	log("Setting up world...");
	World world = World();

	log("Adding Floor to World...");
	world.addObject(floor);

	log("Adding Middle Sphere to World...");
	world.addObject(middleSphere);

	log("Adding Right Sphere to World...");
	world.addObject(rightSphere);

	log("Adding Left Sphere to World...");
	world.addObject(leftSphere);

	log("Adding Light to World...");
	world.addLight(light);

	int width = RENDER_WIDTH;
	int height = RENDER_HEIGHT;
	int fov = 70;

	log("Setting up camera: " + std::to_string(width) + ", " + std::to_string(height) + ", " + std::to_string(fov));
	Camera camera = Camera(width, height, fov);
	auto cameraTransform = Camera::viewMatrix({ 0, 1.5, -5 }, { 0,1,0 }, Vector::up());
	camera.transform(cameraTransform);

	log("Initialization Done");

	auto startTime = std::chrono::high_resolution_clock::now();

	log("Rendering scene -> Start: " + std::to_string(startTime.time_since_epoch().count()));
	auto result = camera.render(world);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto elapsed = endTime - startTime;

	log("Render complete -> End: " + std::to_string(endTime.time_since_epoch().count()) + ", Elapsed: " + std::to_string(elapsed.count()));

	return result;
}

void writeImage(Image image, BaseImageSerializer& serializer)
{
	serializer.serialize(image);

	std::vector<char> ppmBuffer = serializer.buffer();

	std::string bufferData(ppmBuffer.begin(), ppmBuffer.end());

	TCHAR appData[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
		NULL,
		SHGFP_TYPE_CURRENT,
		appData))) {
		std::basic_ostringstream<TCHAR> filePath;
		filePath << appData << _TEXT("\\generated_image.ppm");

		std::ofstream file;
		file.open(filePath.str().c_str());

		file << bufferData << std::endl;
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
	//Image image = generatePerlin();
	PortablePixmapImageSerializer serializer;
	
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

class RAIIglfw
{
public:
	RAIIglfw()
	{
		m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ray Tracer", NULL, NULL);
		glfwMakeContextCurrent(m_window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetErrorCallback(errorCallback);
		glfwSwapInterval(1);
	}

	~RAIIglfw()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool windowShouldOpen()
	{
		return !glfwWindowShouldClose(m_window);
	}

	void loop()
	{
		glfwPollEvents();

		if (!windowShouldOpen())
		{
			std::cout << "Window closed\n";
			glfwDestroyWindow(m_window);
			return;
		}

		updateFramebufferSize();

		glViewport(0, 0, m_width, m_height);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
private:
	GLFWwindow* m_window;
	int m_width = 0;
	int m_height = 0;
	float m_aspectRatio = 0;

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

int main()
{
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

	while (!threadProgress.load() && !renderThread.joinable()) {}

	renderThread.join();

	return 0;
}
