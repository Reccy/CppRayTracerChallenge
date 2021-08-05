#include "args.h"
#include <CLI/CLI.hpp>
#include <thread>

using namespace CppRayTracerChallenge::Core::IO;
using namespace std::filesystem;

Args::Args(int argc, char* argv[])
{
	CLI::App app{"CppRayTracerChallenge"};

	app.add_option("-o,--output", m_outputPath, "Sets output directory for rendered image.")
		->check(CLI::TypeValidator<std::string>())
		->required();

	app.add_option("-i,--input", m_inputPath, "Sets the input path for the YAML file which describes the world to render.")
		->check(CLI::TypeValidator<std::string>())
		->required();

	app.add_option("-t,--threads", m_threads, "Sets the number of threads to use when rendering in parallel.")
		->check(CLI::Validator(CLI::PositiveNumber))
		->default_val(1);

	try
	{
		app.parse(argc, argv);
		m_status = Status::VALID;
	}
	catch (const CLI::Success& e)
	{
		app.exit(e);
		m_status = Status::HELP;
		return;
	}
	catch (const CLI::ParseError& e)
	{
		m_errors.push_back(e.what());
		m_status = Status::INVALID;
		return;
	}

	validateInputPath();
	validateOutputPath();
}

void Args::validateInputPath()
{
	std::error_code error;

	if (!exists(m_inputPath, error))
	{
		m_errors.push_back("Input path points to a location that does not exist");
		m_status = Status::INVALID;
	}

	if (is_directory(m_inputPath, error))
	{
		m_errors.push_back("Input path points to a directory. It must point to a valid YAML file");
		m_status = Status::INVALID;
	}
}

void Args::validateOutputPath()
{
	std::error_code error;

	if (exists(m_outputPath, error) && is_directory(m_outputPath, error))
	{
		// Valid, we are pointing to a directory where we can put the generated images into
		return;
	}

	m_errors.push_back("Output path does not point to a directory. It must point to one");
	m_status = Status::INVALID;
}

bool Args::isHelp() const
{
	return m_status == Status::HELP;
}

bool Args::isInvalid() const
{
	return m_status == Status::INVALID;
}

bool Args::isValid() const
{
	return m_status == Status::VALID;
}

std::string Args::errorMessage() const
{
	if (isValid())
		return "";

	std::stringstream ss;
	ss << "Error when parsing Args\n";

	for (auto& error : m_errors)
	{
		ss << "    " << error << "\n";
	}

	ss << "Run with --help for more information\n";

	return ss.str();
}

std::filesystem::path Args::outputPath() const
{
	return m_outputPath;
}

std::filesystem::path Args::inputPath() const
{
	return m_inputPath;
}

unsigned int Args::threads() const
{
	return m_threads;
}
