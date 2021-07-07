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

const int WINDOW_WIDTH = 1920/4;
const int WINDOW_HEIGHT = 1080/4;
const int RENDER_WIDTH = 1920/8;
const int RENDER_HEIGHT = 1080/8;

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
	bgMaterial.reflective = 0.95f;
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
	material.reflective = 0.5f;
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
	material.reflective = 0.2f;
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
	material.pattern = std::make_shared<SolidColor>(Color(0.95f, 0.94f, 1.0f));
	material.pattern->transform(Transform().rotate(33, 24, 93).scale(0.2, 1.3, 0.2));
	material.diffuse = 0.2f;
	material.specular = 0.15f;
	material.reflective = 0.2f;
	material.refractiveIndex = 1.34f;
	material.transparency = 0.65f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

PointLight buildLight()
{
	return PointLight({ -10, 10, -10 }, Color(1, 1, 1));
}

std::shared_ptr<Camera> camera = nullptr;

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
	camera = std::make_shared<Camera>(width, height, fov);
	auto cameraTransform = Camera::viewMatrix({ 0, 1.5, -5 }, { 0,1,0 }, Vector::up());
	camera->transform(cameraTransform);

	log("Initialization Done");

	auto startTime = std::chrono::high_resolution_clock::now();

	log("Rendering scene -> Start: " + std::to_string(startTime.time_since_epoch().count()));
	auto result = camera->render(world);

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
		m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ray Tracer", NULL, NULL);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDER_WIDTH, RENDER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadRenderData());
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
