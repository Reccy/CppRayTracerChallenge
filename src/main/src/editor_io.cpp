#include "editor_io.h"

#include <sstream>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>

namespace EditorIO
{
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
