#include <gtest/gtest.h>
#include <serializer/wavefront_obj_serializer.h>
#include <renderer/group.h>
#include <math/point.h>
#include <math/triangle.h>

using namespace CppRayTracerChallenge::Core::Serializer;
using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJSerializer, ignore_unrecognised_lines)
{
	WavefrontOBJSerializer serializer = WavefrontOBJSerializer();

	std::stringstream ss;
	ss << "There was a young lady named Bright\n"
	"who traveled much faster than light.\n"
	"She set out one day\n"
	"in a relative way,\n"
	"and came back the previous night.";

	std::string input = ss.str();

	serializer.deserialize({input.begin(), input.end()});

	EXPECT_EQ(serializer.ignoredLines(), 5);
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJSerializer, vertex_records)
{
	WavefrontOBJSerializer serializer = WavefrontOBJSerializer();

	std::stringstream ss;
	ss << "v -1 1 0\n"
		"v -1.0000 0.5000 0.0000\n"
		"v 1 0 0\n"
		"v 1 1 0\n"
		"v 2.5 1.25 1\n"
		"v 1 1 1 1f2 5 9\n"
		"v 1 a b";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });

	EXPECT_EQ(serializer.ignoredLines(), 2);

	auto verts = serializer.vertices();

	EXPECT_EQ(verts.at(0), Point(-1, 1, 0));
	EXPECT_EQ(verts.at(1), Point(-1, 0.5, 0));
	EXPECT_EQ(verts.at(2), Point(1, 0, 0));
	EXPECT_EQ(verts.at(3), Point(1, 1, 0));
	EXPECT_EQ(verts.at(4), Point(2.5, 1.25, 1));
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJSerializer, parsing_triangle_faces)
{
	WavefrontOBJSerializer serializer = WavefrontOBJSerializer();

	std::stringstream ss;
	ss << "v -1 1 0\n"
		"v -1 0 0\n"
		"v 1 0 0\n"
		"v 1 1 0\n"
		"f 1 2 3\n"
		"f 1 3 4\n"
		"f 1 3 423 3 4\n"
		"f 1 6 9\n";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });

	Group group = serializer.defaultGroup();

	auto t1 = std::dynamic_pointer_cast<Triangle>(group.child(0)->shape());
	auto t2 = std::dynamic_pointer_cast<Triangle>(group.child(1)->shape());

	EXPECT_EQ(serializer.ignoredLines(), 2);

	EXPECT_EQ(t1->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t1->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t1->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t2->p2(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p3(), serializer.vertices().at(3));
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJSerializer, triangulate_polygons)
{
	WavefrontOBJSerializer serializer = WavefrontOBJSerializer();

	std::stringstream ss;
	ss << "v -1 1 0\n"
		"v -1 0 0\n"
		"v 1 0 0\n"
		"v 1 1 0\n"
		"v 0 2 0\n"
		"f 1 2 3 4 5";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });

	Group group = serializer.defaultGroup();

	auto t1 = std::dynamic_pointer_cast<Triangle>(group.child(0)->shape());
	auto t2 = std::dynamic_pointer_cast<Triangle>(group.child(1)->shape());
	auto t3 = std::dynamic_pointer_cast<Triangle>(group.child(2)->shape());

	EXPECT_EQ(t1->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t1->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t1->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t2->p2(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p3(), serializer.vertices().at(3));
	EXPECT_EQ(t3->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t3->p2(), serializer.vertices().at(3));
	EXPECT_EQ(t3->p3(), serializer.vertices().at(4));
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJSerializer, triangles_in_groups)
{
	WavefrontOBJSerializer serializer = WavefrontOBJSerializer();

	std::stringstream ss;
	ss << "v -1 1 0\n"
		"v -1 0 0\n"
		"v 1 0 0\n"
		"v 1 1 0\n"
		"\n"
		"gFirstGroup\n"
		"f 1 2 3\n"
		"g Second Group \n"
		"f 1 3 4";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });

	Group g1 = serializer.group("FirstGroup");
	Group g2 = serializer.group("Second Group");

	auto t1 = std::dynamic_pointer_cast<Triangle>(g1.child(0)->shape());
	auto t2 = std::dynamic_pointer_cast<Triangle>(g2.child(0)->shape());

	EXPECT_EQ(t1->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t1->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t1->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t2->p2(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p3(), serializer.vertices().at(3));
}
