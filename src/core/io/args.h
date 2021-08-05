#ifndef _CPPRAYTRACERCHALLENGE_CORE_IO_ARGS
#define _CPPRAYTRACERCHALLENGE_CORE_IO_ARGS

#include <filesystem>
#include <vector>

namespace CppRayTracerChallenge::Core::IO
{
	class Args
	{
	public:
		Args(int argc, char* argv[]);

		bool isHelp() const;
		bool isInvalid() const;
		bool isValid() const;

		std::string errorMessage() const;

		std::filesystem::path outputPath() const;
		std::filesystem::path inputPath() const;
		unsigned int threads() const;
	private:
		enum class Status
		{
			// Args were parsed successfully
			VALID,
			// Args were not parsed successfully
			INVALID,
			// User invoked help on CLI
			HELP
		};

		Status m_status;

		std::vector<std::string> m_errors;
		
		std::filesystem::path m_outputPath;
		std::filesystem::path m_inputPath;
		unsigned int m_threads;

		void validateInputPath();
		void validateOutputPath();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_IO_ARGS
