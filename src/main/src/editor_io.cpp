#include "editor_io.h"

#include <sstream>
#include <map>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include <assert.h>

#include "editor_core.h"

namespace EditorIO
{
	static Renderer::ShadowcastMode _StringToShadowcastMode(const std::string& str)
	{
		if (str == "ALWAYS") return Renderer::ShadowcastMode::ALWAYS;
		if (str == "NEVER") return Renderer::ShadowcastMode::NEVER;
		if (str == "WHEN_TRANSPARENT") return Renderer::ShadowcastMode::WHEN_TRANSPARENT;

		assert(false); // Invalid state

		return Renderer::ShadowcastMode::ALWAYS;
	}

	static std::string _ShadowcastModeToString(Renderer::ShadowcastMode mode)
	{
		if (mode == Renderer::ShadowcastMode::ALWAYS) return "ALWAYS";
		if (mode == Renderer::ShadowcastMode::NEVER) return "NEVER";
		if (mode == Renderer::ShadowcastMode::WHEN_TRANSPARENT) return "WHEN_TRANSPARENT";

		assert(false); // Invalid state

		return "";
	}

	static std::string _EditorObjectTypeToString(EditorObject::Type objectType)
	{
		if (objectType == EditorObject::Type::CUBE) return "CUBE";
		if (objectType == EditorObject::Type::PLANE) return "PLANE";
		if (objectType == EditorObject::Type::LIGHT) return "LIGHT";
		if (objectType == EditorObject::Type::SPHERE) return "SPHERE";
		if (objectType == EditorObject::Type::CYLINDER) return "CYLINDER";

		assert(false); // This should not get called

		return "";
	}

	static EditorObject::Type _StringToEditorObjectType(const std::string& str)
	{
		if (str == "CUBE") return EditorObject::Type::CUBE;
		if (str == "PLANE") return EditorObject::Type::PLANE;
		if (str == "LIGHT") return EditorObject::Type::LIGHT;
		if (str == "SPHERE") return EditorObject::Type::SPHERE;
		if (str == "CYLINDER") return EditorObject::Type::CYLINDER;

		assert(false); // This should not get called

		return EditorObject::Type::CUBE;
	}

	static void _SerializeSceneSettings(tinyxml2::XMLPrinter& printer, const CameraSettings& cameraSettings)
	{
		printer.OpenElement("scene");

		printer.OpenElement("render_dimensions");
		printer.PushAttribute("width", cameraSettings.renderWidth);
		printer.PushAttribute("height", cameraSettings.renderHeight);
		printer.CloseElement();

		printer.CloseElement();
	}

	static void _SerializeCameraSettings(tinyxml2::XMLPrinter& printer, const ROGLL::Camera& editorCamera, const CameraSettings& cameraSettings)
	{
		printer.OpenElement("camera");
		printer.PushAttribute("fov", cameraSettings.fov);

		printer.OpenElement("position");
		printer.PushAttribute("x", editorCamera.transform.position.x());
		printer.PushAttribute("y", editorCamera.transform.position.y());
		printer.PushAttribute("z", editorCamera.transform.position.z());
		printer.CloseElement();

		printer.OpenElement("rotation");
		printer.PushAttribute("pitch", cameraSettings.xRot);
		printer.PushAttribute("yaw", cameraSettings.yRot);
		printer.PushAttribute("roll", 0); // Keep roll so that if roll is supported in the future, won't need to add a check to see if roll is in the XML element
		printer.CloseElement();

		printer.CloseElement();
	}

	static void _SerializeEditorObjects(tinyxml2::XMLPrinter& printer, const std::vector<EditorObject*>& editorObjects, const Graphics::Color& lightColor)
	{
		printer.OpenElement("editor_objects");

		for (const auto& obj : editorObjects)
		{
			printer.OpenElement("editor_object");
			printer.PushAttribute("id", obj->id);
			printer.PushAttribute("name", obj->name.c_str());
			printer.PushAttribute("object_type", _EditorObjectTypeToString(obj->objectType).c_str());
			printer.PushAttribute("editor_material_id", obj->material->id);

			printer.OpenElement("position");
			printer.PushAttribute("x", obj->transform.position.x());
			printer.PushAttribute("y", obj->transform.position.y());
			printer.PushAttribute("z", obj->transform.position.z());
			printer.CloseElement();

			printer.OpenElement("euler_rotation");
			printer.PushAttribute("x", obj->eulerRotation.x());
			printer.PushAttribute("y", obj->eulerRotation.y());
			printer.PushAttribute("z", obj->eulerRotation.z());
			printer.CloseElement();

			printer.OpenElement("scaling");
			printer.PushAttribute("x", obj->transform.scaling.x());
			printer.PushAttribute("y", obj->transform.scaling.y());
			printer.PushAttribute("z", obj->transform.scaling.z());
			printer.CloseElement();

			if (obj->objectType == EditorObject::Type::LIGHT)
			{
				printer.OpenElement("light_color");
				printer.PushAttribute("r", lightColor.red());
				printer.PushAttribute("g", lightColor.green());
				printer.PushAttribute("b", lightColor.blue());
				printer.CloseElement();
			}

			printer.CloseElement();
		}

		printer.CloseElement();
	}

	static void _SerializeMaterial(tinyxml2::XMLPrinter& printer, const std::vector<EditorMaterial*>& editorMaterials)
	{
		printer.OpenElement("materials");

		for (const auto& mat : editorMaterials)
		{
			printer.OpenElement("material");
			printer.PushAttribute("id", mat->id);
			printer.PushAttribute("protected", mat->isProtected);
			printer.PushAttribute("name", mat->name.c_str());
			printer.PushAttribute("ambient", mat->ambient);
			printer.PushAttribute("diffuse", mat->diffuse);
			printer.PushAttribute("specular", mat->specular);
			printer.PushAttribute("shininess", mat->shininess);
			printer.PushAttribute("reflective", mat->reflective);
			printer.PushAttribute("transparency", mat->transparency);
			printer.PushAttribute("refractive_index", mat->refractiveIndex);
			printer.PushAttribute("shadowcast_mode", _ShadowcastModeToString(mat->shadowcastMode).c_str());

			printer.OpenElement("pattern");
			mat->editorPattern->Serialize(printer);
			printer.CloseElement();

			printer.CloseElement();
		}

		printer.CloseElement();
	}

	void SaveWorld(const std::string& path, const CameraSettings& cameraSettings, const ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, const std::vector<EditorMaterial*>& editorMaterials, const Graphics::Color& lightColor)
	{
		std::cout << "Saving file to " << path << std::endl;

		FILE* pFile = fopen(path.c_str(), "w");

		tinyxml2::XMLPrinter printer(pFile);

		printer.OpenElement("metadata");
		printer.PushAttribute("format_version", "0");
		printer.CloseElement();

		_SerializeSceneSettings(printer, cameraSettings);
		_SerializeCameraSettings(printer, editorCamera, cameraSettings);
		_SerializeEditorObjects(printer, editorObjects, lightColor);
		_SerializeMaterial(printer, editorMaterials);

		fclose(pFile);
	}

	void OpenWorld(const std::string& path, CameraSettings& cameraSettings, ROGLL::Camera& editorCamera, std::vector<EditorObject*>& editorObjects, std::vector<EditorMaterial*>& editorMaterials, Graphics::Color& lightColor)
	{
		auto _PrintAbortString = [](std::string variableName) -> void
		{
			std::cout << "Error retrieving " << variableName << "\n";
			std::cout << "Aborted open operation" << std::endl;
		};

		std::cout << "Opening file " << path << std::endl;

		tinyxml2::XMLDocument doc;
		auto err = doc.LoadFile(path.c_str());

		if (err != tinyxml2::XML_SUCCESS)
		{
			std::cout << "Error parsing XML at " << path << std::endl;
			return;
		}

		// Load Metadata
		auto* pMetadataElement = doc.FirstChildElement("metadata");

		auto formatVersionAttribute = pMetadataElement->FindAttribute("format_version");

		int formatVersion;
		if (formatVersionAttribute->QueryIntValue(&formatVersion) != tinyxml2::XML_SUCCESS)
		{
			_PrintAbortString("format_version");
			return;
		}
		else
		{
			std::cout << "Format Version: " << formatVersion << std::endl;
		}

		// Load Render Settings
		auto sceneElement = doc.FirstChildElement("scene");
		auto renderDimensionsElement = sceneElement->FirstChildElement("render_dimensions");

		int renderWidth;
		int renderHeight;

		auto renderDimensionsWidthAttribute = renderDimensionsElement->FindAttribute("width");
		auto renderDimensionsHeightAttribute = renderDimensionsElement->FindAttribute("height");

		if (renderDimensionsWidthAttribute->QueryIntValue(&renderWidth) != tinyxml2::XML_SUCCESS)
		{
			_PrintAbortString("width");
			return;
		}

		if (renderDimensionsHeightAttribute->QueryIntValue(&renderHeight) != tinyxml2::XML_SUCCESS)
		{
			_PrintAbortString("height");
			return;
		}

		std::cout << "Render Width: " << renderWidth << ", Height: " << renderHeight << std::endl;

		// Load Camera Settings
		auto cameraElement = doc.FirstChildElement("camera");

		auto camFov = cameraElement->FindAttribute("fov")->FloatValue();

		auto cameraRotationElement = cameraElement->FirstChildElement("rotation");

		auto camXRot = cameraRotationElement->FindAttribute("pitch")->FloatValue();
		auto camYRot = cameraRotationElement->FindAttribute("yaw")->FloatValue();

		auto cameraPositionElement = cameraElement->FirstChildElement("position");

		auto camXPos = cameraPositionElement->FindAttribute("x")->FloatValue();
		auto camYPos = cameraPositionElement->FindAttribute("y")->FloatValue();
		auto camZPos = cameraPositionElement->FindAttribute("z")->FloatValue();

		RML::Vector camPosition(camXPos, camYPos, camZPos);

		std::cout << "Camera Fov: " << camFov << "\n";
		std::cout << "Camera Pitch: " << camXRot << "\n";
		std::cout << "Camera Yaw: " << camYRot << "\n";
		std::cout << "Camera Position: " << camPosition << "\n";

		// Load Materials
		std::map<unsigned int, EditorMaterial*> newEditorMaterials;

		auto materialsElement = doc.FirstChildElement("materials");

		for (tinyxml2::XMLElement* child = materialsElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			bool isProtected;
			unsigned int id;
			std::string name;
			float ambient;
			float diffuse;
			float specular;
			float shininess;
			float reflective;
			float transparency;
			float refractiveIndex;
			Renderer::ShadowcastMode shadowcastMode;

			if (child->FindAttribute("id")->QueryUnsignedValue(&id) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("id");
				return;
			}

			if (child->FindAttribute("protected")->QueryBoolValue(&isProtected) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("protected");
				return;
			}

			auto nameCStr = child->FindAttribute("name")->Value();
			name = std::string(nameCStr);

			if (child->FindAttribute("ambient")->QueryFloatValue(&ambient) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("ambient");
				return;
			}

			if (child->FindAttribute("diffuse")->QueryFloatValue(&diffuse) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("diffuse");
				return;
			}

			if (child->FindAttribute("specular")->QueryFloatValue(&specular) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("specular");
				return;
			}

			if (child->FindAttribute("shininess")->QueryFloatValue(&shininess) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("shininess");
				return;
			}

			if (child->FindAttribute("reflective")->QueryFloatValue(&reflective) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("reflective");
				return;
			}

			if (child->FindAttribute("transparency")->QueryFloatValue(&transparency) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("transparency");
				return;
			}

			if (child->FindAttribute("refractive_index")->QueryFloatValue(&refractiveIndex) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("refractive_index");
				return;
			}

			auto shadowcastModeCStr = child->FindAttribute("shadowcast_mode")->Value();
			auto shadowcastModeStr = std::string(shadowcastModeCStr);

			shadowcastMode = _StringToShadowcastMode(shadowcastModeStr);

			std::cout << "--- Material:\n";
			std::cout << "id: " << id << "\n";
			std::cout << "isProtected: " << isProtected << "\n";
			std::cout << "name: " << name << "\n";
			std::cout << "ambient: " << ambient << "\n";
			std::cout << "diffuse: " << diffuse << "\n";
			std::cout << "specular: " << specular << "\n";
			std::cout << "shininess: " << shininess << "\n";
			std::cout << "reflective: " << reflective << "\n";
			std::cout << "transparency: " << transparency << "\n";
			std::cout << "refractiveIndex: " << refractiveIndex << "\n";
			std::cout << "shadowcastMode: " << shadowcastModeStr << "\n";

			// TODO: Make this serialization generic and work for multiple pattern types
			auto patternElement = child->FirstChildElement("pattern");
			auto patternType = std::string(patternElement->FindAttribute("type")->Value());
			float patternColorRed;
			float patternColorGreen;
			float patternColorBlue;
			Graphics::Color patternColor(0, 0, 0);

			if (patternType == "SOLID_COLOR")
			{
				auto colorElement = patternElement->FirstChildElement("color");

				std::cout << colorElement->FindAttribute("r")->Value() << "\n";

				patternColorRed = colorElement->FindAttribute("r")->FloatValue();
				patternColorGreen = colorElement->FindAttribute("g")->FloatValue();
				patternColorBlue = colorElement->FindAttribute("b")->FloatValue();
				patternColor = Graphics::Color(patternColorRed, patternColorGreen, patternColorBlue);

				std::cout << "----- Pattern:\n";
				std::cout << "type: SOLID_COLOR\n";
				std::cout << "color: " << patternColor << "\n";
			}
			else
			{
				_PrintAbortString("type");
				return;
			}

			EditorPatternSolidColor* pattern = new EditorPatternSolidColor();
			pattern->color = patternColor;

			EditorMaterial* newMaterial = new EditorMaterial(pattern);
			newMaterial->id = id;
			newMaterial->isProtected = isProtected;
			newMaterial->name = name;
			newMaterial->ambient = ambient;
			newMaterial->diffuse = diffuse;
			newMaterial->specular = specular;
			newMaterial->shininess = shininess;
			newMaterial->reflective = reflective;
			newMaterial->transparency = transparency;
			newMaterial->refractiveIndex = refractiveIndex;
			newMaterial->shadowcastMode = shadowcastMode;

			newEditorMaterials[id] = newMaterial;
		}

		// Load Editor Objects
		auto editorObjectsElement = doc.FirstChildElement("editor_objects");

		std::vector<EditorObject*> newEditorObjects;

		for (tinyxml2::XMLElement* child = editorObjectsElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			unsigned int editorObjectId;
			std::string editorObjectName;
			std::string editorObjectTypeString;
			EditorObject::Type editorObjectType;
			unsigned int editorMaterialId;

			auto editorObjectIdAttribute = child->FindAttribute("id");
			auto editorObjectNameAttribute = child->FindAttribute("name");
			auto editorObjectTypeAttribute = child->FindAttribute("object_type");
			auto editorObjectMaterialIdAttribute = child->FindAttribute("editor_material_id");

			if (editorObjectIdAttribute->QueryUnsignedValue(&editorObjectId) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("id");
				return;
			}

			auto editorObjectNameCStr = editorObjectNameAttribute->Value();
			editorObjectName = std::string(editorObjectNameCStr);

			auto editorObjectTypeCStr = editorObjectTypeAttribute->Value();
			editorObjectTypeString = std::string(editorObjectTypeCStr);
			editorObjectType = _StringToEditorObjectType(editorObjectTypeString);

			if (editorObjectMaterialIdAttribute->QueryUnsignedValue(&editorMaterialId) != tinyxml2::XML_SUCCESS)
			{
				_PrintAbortString("editor_material_id");
				return;
			}

			auto positionElement = child->FirstChildElement("position");
			auto rotationElement = child->FirstChildElement("euler_rotation");
			auto scalingElement = child->FirstChildElement("scaling");

			RML::Vector position(
				positionElement->FindAttribute("x")->FloatValue(),
				positionElement->FindAttribute("y")->FloatValue(),
				positionElement->FindAttribute("z")->FloatValue());

			RML::Vector rotation(
				rotationElement->FindAttribute("x")->FloatValue(),
				rotationElement->FindAttribute("y")->FloatValue(),
				rotationElement->FindAttribute("z")->FloatValue()
			);

			RML::Vector scaling(
				scalingElement->FindAttribute("x")->FloatValue(),
				scalingElement->FindAttribute("y")->FloatValue(),
				scalingElement->FindAttribute("z")->FloatValue()
			);

			std::cout << "--- Editor Object:\n";
			std::cout << "id: " << editorObjectId << "\n";
			std::cout << "name: " << editorObjectName << "\n";
			std::cout << "type: " << editorObjectTypeString << "\n";
			std::cout << "type_id: " << static_cast<int>(editorObjectType) << "\n";
			std::cout << "editor_material_id: " << editorMaterialId << "\n";
			std::cout << "position: " << position << "\n";
			std::cout << "rotation: " << rotation << "\n";
			std::cout << "scaling: " << scaling << "\n";

			if (editorObjectType == EditorObject::Type::LIGHT)
			{
				auto colorElement = child->FirstChildElement("light_color");

				float r = colorElement->FindAttribute("r")->FloatValue();
				float g = colorElement->FindAttribute("g")->FloatValue();
				float b = colorElement->FindAttribute("b")->FloatValue();

				lightColor = Graphics::Color(r, g, b);

				std::cout << "light_color: " << lightColor << "\n";
			}

			EditorObject* editorObject = new EditorObject();
			editorObject->id = editorObjectId;
			editorObject->name = editorObjectName;
			editorObject->objectType = editorObjectType;
			editorObject->material = newEditorMaterials[editorMaterialId];
			editorObject->meshInstance = EditorObject::CreateMeshInstanceForType(editorObjectType);
			editorObject->eulerRotation = RML::Vector(rotation.x(), rotation.y(), rotation.z());
			editorObject->transform.position = position;
			editorObject->transform.rotation = RML::Quaternion::euler_angles(rotation.x(), rotation.y(), rotation.z());
			editorObject->transform.scaling = scaling;

			newEditorObjects.emplace_back(editorObject);
		}

		//
		// Actual state changes should only happen below here.
		// If anything goes wrong above it's fine, since no state change will happen.
		// Errors from here on should cause a crash, because it will invalidate the editor state.
		//

		std::cout << "File load successful. Updating scene state." << std::endl;

		// Delete existing editor objects

		for (EditorObject* obj : editorObjects)
		{
			delete obj;
		}

		editorObjects.clear();

		// Delete existing editor materials

		for (EditorMaterial* mat : editorMaterials)
		{
			delete mat;
		}

		editorMaterials.clear();

		editorObjects = newEditorObjects;

		for (auto& pair : newEditorMaterials)
		{
			EditorMaterial* mat = pair.second;
			editorMaterials.push_back(mat);

			if (mat->isProtected)
			{
				EditorCore::UpdateDefaultEditorMaterial(mat);
			}
		}

		cameraSettings.renderWidth = renderWidth;
		cameraSettings.renderHeight = renderHeight;

		cameraSettings.xRot = camXRot;
		cameraSettings.yRot = camYRot;

		cameraSettings.fov = camFov;

		editorCamera.transform.position = camPosition;

		std::cout << "Scene Loaded" << std::endl;
	}

	void WriteImage(Graphics::Image image, Serializer::BaseImageSerializer& serializer)
	{
		std::cout << "Writing Image to Desktop... ";

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

			file.write((const char*)&ppmBuffer[0], ppmBuffer.size());
			file.close();

			std::cout << "Success" << std::endl;
		}
		else
		{
			std::cout << "Failed!" << std::endl;
		}
	}
}
