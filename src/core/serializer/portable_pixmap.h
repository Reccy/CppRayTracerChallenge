#ifndef _CPPRAYTRACERCHALLENGE_CORE_IO_PORTABLE_PIXMAP
#define _CPPRAYTRACERCHALLENGE_CORE_IO_PORTABLE_PIXMAP

#include <fstream>
#include <sstream>
#include <vector>

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// A PortablePixmap file (.ppm) used for storing bitmap image data
	/// </summary>
	class PortablePixmap
	{
	public:
		/// <summary>
		/// Creates a new PortablePixmap file
		/// </summary>
		/// <param name="width">The width of the image in pixels</param>
		/// <param name="height">The height of the image in pixels</param>
		/// <param name="payload">Array of each pixel value from top-left to bottom-right</param>
		PortablePixmap(const int width, const int height, const std::vector<char> payload) : m_width{ width }, m_height{ height }, m_payload{ payload }
		{
			buildData();
		};

		/// <summary>
		/// Returns the raw data of the PPM file
		/// </summary>
		std::stringstream data() const;
	private:
		int m_width, m_height;
		std::vector<char> m_payload;
		std::stringstream m_data;

		void buildData()
		{
			m_data << "P3\n";
			m_data << std::to_string(m_width) << " " << std::to_string(m_height) << "\n";
			m_data << "255\n";
		};
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_IO_PORTABLE_PIXMAP
