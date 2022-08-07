#include <gtest/gtest.h>
#include <serializer/wavefront_obj_deserializer.h>
#include <renderer/group.h>
#include <RML.h>
#include <math/triangle.h>
#include <math/smooth_triangle.h>

using namespace CppRayTracerChallenge::Core::Serializer;
using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, ignore_unrecognised_lines)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, vertex_records)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, parsing_triangle_faces)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, triangulate_polygons)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, triangles_in_groups)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

	auto g1 = serializer.group("FirstGroup");
	auto g2 = serializer.group("Second Group");

	auto t1 = std::dynamic_pointer_cast<Triangle>(g1->child(0)->shape());
	auto t2 = std::dynamic_pointer_cast<Triangle>(g2->child(0)->shape());

	EXPECT_EQ(t1->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t1->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t1->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t2->p2(), serializer.vertices().at(2));
	EXPECT_EQ(t2->p3(), serializer.vertices().at(3));
}


TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, group_for_scene)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

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

	Group defaultGroup = serializer.defaultGroup();

	EXPECT_EQ(defaultGroup.size(), 2);
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, vertex_normal_records)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

	std::stringstream ss;
	ss << "vn 0 0 1\n"
		"vn 0.707 0 -0.707\n"
		"vn 1 2 3";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });

	EXPECT_EQ(serializer.normals().at(0), Vector(0, 0, 1));
	EXPECT_EQ(serializer.normals().at(1), Vector(0.707, 0, -0.707));
	EXPECT_EQ(serializer.normals().at(2), Vector(1, 2, 3));
}

TEST(CppRayTracerChallenge_Core_Serializer_WavefrontOBJDeserializer, faces_with_normals)
{
	WavefrontOBJDeserializer serializer = WavefrontOBJDeserializer();

	std::stringstream ss;
	ss << "v 0 1 0\n"
		"v -1 0 0\n"
		"v 1 0 0\n"
		"\n"
		"vn -1 0 0\n"
		"vn 1 0 0\n"
		"vn 0 1 0\n"
		"\n"
		"f 1//3 2//1 3//2\n"
		"f 1/0/3 2/102/1 3/14/2";

	std::string input = ss.str();

	serializer.deserialize({ input.begin(), input.end() });
	
	auto g = serializer.defaultGroup();

	auto t1 = std::dynamic_pointer_cast<SmoothTriangle>(g.child(0)->shape());
	auto t2 = std::dynamic_pointer_cast<SmoothTriangle>(g.child(1)->shape());

	EXPECT_EQ(t1->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t1->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t1->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t1->n1(), serializer.normals().at(2));
	EXPECT_EQ(t1->n2(), serializer.normals().at(0));
	EXPECT_EQ(t1->n3(), serializer.normals().at(1));

	EXPECT_EQ(t2->p1(), serializer.vertices().at(0));
	EXPECT_EQ(t2->p2(), serializer.vertices().at(1));
	EXPECT_EQ(t2->p3(), serializer.vertices().at(2));
	EXPECT_EQ(t2->n1(), serializer.normals().at(2));
	EXPECT_EQ(t2->n2(), serializer.normals().at(0));
	EXPECT_EQ(t2->n3(), serializer.normals().at(1));

	EXPECT_EQ(t1->p1(), t2->p1());
	EXPECT_EQ(t1->p2(), t2->p2());
	EXPECT_EQ(t1->p3(), t2->p3());
	EXPECT_EQ(t1->n1(), t2->n1());
	EXPECT_EQ(t1->n2(), t2->n2());
	EXPECT_EQ(t1->n3(), t2->n3());
}
