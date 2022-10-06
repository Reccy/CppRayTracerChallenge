#include "ply.h"
#include <rply.h>

namespace PLY
{
	static struct PlyFaceStruct {
		std::vector<unsigned int>* indices = nullptr;
		unsigned int faceParseIdx = 0;
	};

	static int _PlyVertexCb(p_ply_argument argument)
	{
		long attribute;
		std::vector<float>* vertexBuffer;
		ply_get_argument_user_data(argument, (void**)&vertexBuffer, &attribute);

		double data = ply_get_argument_value(argument);

		vertexBuffer->push_back(data);

		return 1;
	}

	static int _PlyFaceCb(p_ply_argument argument)
	{
		PlyFaceStruct* plyFaceData;
		ply_get_argument_user_data(argument, (void**)&plyFaceData, NULL);
		double data = ply_get_argument_value(argument);

		if (plyFaceData->faceParseIdx % 4 == 0)
		{
			if (data == 3)
			{
				plyFaceData->faceParseIdx++;
				return 1;
			}

			std::cout << "PLY LOAD ERROR: Non Triangle face found in element list. Face size: " << data << std::endl;
			return 0;
		}

		plyFaceData->faceParseIdx++;

		plyFaceData->indices->push_back(data);

		return 1;
	}

	ROGLL::Mesh* LoadMesh(std::string filepath, const ROGLL::VertexAttributes& layout)
	{
		// BEGIN GIZMO LOAD
		std::cout << "Loading ply model at [" << filepath << "]...";

		p_ply plyFile = ply_open(filepath.c_str(), NULL, 0, NULL);
		if (!plyFile || !ply_read_header(plyFile))
		{
			std::cout << "ERROR: Could not read ply file [" << filepath << "]" << std::endl;
			glfwTerminate();
			std::cin.get();
			abort();
		}

		std::vector<float> vertexPositions;
		std::vector<float> vertexNormals;
		std::vector<float> vertexColors;
		std::vector<unsigned int> indices;

		long nvertices, ntriangles;
		nvertices = ply_set_read_cb(plyFile, "vertex", "x", _PlyVertexCb, &vertexPositions, 0);
		ply_set_read_cb(plyFile, "vertex", "y", _PlyVertexCb, &vertexPositions, 1);
		ply_set_read_cb(plyFile, "vertex", "z", _PlyVertexCb, &vertexPositions, 2);
		ply_set_read_cb(plyFile, "vertex", "nx", _PlyVertexCb, &vertexNormals, 3);
		ply_set_read_cb(plyFile, "vertex", "ny", _PlyVertexCb, &vertexNormals, 4);
		ply_set_read_cb(plyFile, "vertex", "nz", _PlyVertexCb, &vertexNormals, 5);
		ply_set_read_cb(plyFile, "vertex", "red", _PlyVertexCb, &vertexColors, 6);
		ply_set_read_cb(plyFile, "vertex", "green", _PlyVertexCb, &vertexColors, 7);
		ply_set_read_cb(plyFile, "vertex", "blue", _PlyVertexCb, &vertexColors, 8);

		PlyFaceStruct pfs;
		pfs.indices = &indices;

		ntriangles = ply_set_read_cb(plyFile, "face", "vertex_indices", _PlyFaceCb, &pfs, 0);

		if (!ply_read(plyFile))
		{
			std::cout << "ERROR: Could not read ply model file" << std::endl;
			glfwTerminate();
			std::cin.get();
			abort();
		}

		ply_close(plyFile);

		std::cout << "Done" << std::endl;

		std::vector<float> vertices;

		for (long i = 0; i < nvertices; i++)
		{
			float offset = i * 3;

			for (const auto& attrib : layout.GetAttributes())
			{
				if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::POSITION3)
				{
					vertices.push_back(vertexPositions.at(offset));
					vertices.push_back(vertexPositions.at(offset + 1));
					vertices.push_back(vertexPositions.at(offset + 2));
				}

				if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::COLOR3)
				{
					vertices.push_back(vertexColors.at(offset) / 255);
					vertices.push_back(vertexColors.at(offset + 1) / 255);
					vertices.push_back(vertexColors.at(offset + 2) / 255);
				}

				if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::NORMAL3)
				{
					vertices.push_back(vertexNormals.at(offset));
					vertices.push_back(vertexNormals.at(offset + 1));
					vertices.push_back(vertexNormals.at(offset + 2));
				}
			}
		}

		return new ROGLL::Mesh(vertices, indices, layout);
	}
}
