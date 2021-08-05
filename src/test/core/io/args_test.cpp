#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <io/args.h>
#include <filesystem>
#include <fstream>

using namespace CppRayTracerChallenge::Core::IO;

#define SETUP_ARGS(i) const unsigned int argc = i; char* argv[argc]; argv[0] = "CppRayTracerChallenge";

class ArgsTest
{
public:
	ArgsTest(bool makeInput = true, bool makeOutput = true, bool inputInvalid = false, bool outputInvalid = false)
	{
		inputDirStr = TempDir("CppRayTracerChallenge\\input.yml");
		outputDirStr = TempDir("CppRayTracerChallenge\\output\\");

		inputDir = inputDirStr;
		outputDir = outputDirStr;

		if (makeInput)
		{
			std::filesystem::create_directories(inputDir.parent_path());

			if (inputInvalid)
			{
				std::filesystem::create_directory(inputDir);
			}
			else
			{
				std::ofstream ofs(inputDir);
				ofs << "blah";
				ofs.close();
			}
		}

		if (makeOutput)
		{
			if (outputInvalid)
			{
				std::ofstream ofs(outputDir);
				ofs << "blah";
				ofs.close();
			}
			else
			{
				std::filesystem::create_directories(outputDir);
			}
		}
	}

	static std::string formatErrors(std::vector<std::string> errors)
	{
		std::stringstream ss;
		ss << "Error when parsing Args\n";

		for (auto& error : errors)
		{
			ss << "    " << error << "\n";
		}

		ss << "Run with --help for more information\n";

		return ss.str();
	}

	~ArgsTest()
	{
		if (std::filesystem::exists(inputDir))
		{
			std::filesystem::remove_all(inputDir);
		}

		if (std::filesystem::exists(outputDir))
		{
			std::filesystem::remove_all(outputDir);
		}

		delete[] inputDirStr;
		delete[] outputDirStr;
	}

	std::filesystem::path inputDir;
	std::filesystem::path outputDir;

	char* inputDirStr;
	char* outputDirStr;
private:
	char* TempDir(std::string str)
	{
		std::stringstream ss;
		ss << ::testing::TempDir();
		ss << str;
		std::string cppstr = ss.str();

		char* cstr = new char[cppstr.length() + 1];
		strcpy_s(cstr, cppstr.length() + 1, cppstr.c_str());

		return cstr;
	}
};

TEST(CppRayTracerChallenge_Core_IO_Args, no_args_is_invalid)
{
	SETUP_ARGS(1);

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "--output is required" }));
}

TEST(CppRayTracerChallenge_Core_IO_Args, only_output_is_invalid)
{
	auto a = ArgsTest();

	SETUP_ARGS(3);
	argv[1] = "-o";
	argv[2] = a.inputDirStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "--input is required" }));
}

TEST(CppRayTracerChallenge_Core_IO_Args, only_input_is_invalid)
{
	auto a = ArgsTest();

	SETUP_ARGS(3);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "--output is required" }));
}

TEST(CppRayTracerChallenge_Core_IO_Args, input_and_output_is_valid)
{
	auto a = ArgsTest();

	SETUP_ARGS(5);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;

	Args args = Args(argc, argv);

	EXPECT_EQ(args.inputPath(), a.inputDir);
	EXPECT_EQ(args.outputPath(), a.outputDir);

	EXPECT_TRUE(args.isValid());
}

TEST(CppRayTracerChallenge_Core_IO_Args, input_not_existing_is_invalid)
{
	auto a = ArgsTest(false, true, false, false);

	SETUP_ARGS(5);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "Input path points to a location that does not exist" }));
}

TEST(CppRayTracerChallenge_Core_IO_Args, output_not_existing_is_invalid)
{
	auto a = ArgsTest(true, false, false, false);

	SETUP_ARGS(5);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;

	Args args = Args(argc, argv);
	
	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "Output path does not point to a directory. It must point to one" }));
}


TEST(CppRayTracerChallenge_Core_IO_Args, input_is_directory_is_invalid)
{
	auto a = ArgsTest(true, true, true, false);

	SETUP_ARGS(5);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "Input path points to a directory. It must point to a valid YAML file" }));
}

TEST(CppRayTracerChallenge_Core_IO_Args, output_is_file_is_invalid)
{
	auto a = ArgsTest(true, true, false, true);

	SETUP_ARGS(5);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());

	EXPECT_EQ(args.errorMessage(), ArgsTest::formatErrors({ "Output path does not point to a directory. It must point to one"}));
}

TEST(CppRayTracerChallenge_Core_IO_Args, threads_can_be_positive_number)
{
	auto a = ArgsTest();

	const int threadCount = 5;
	char* threadCountStr = { "5" };

	SETUP_ARGS(7);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;
	argv[5] = "-t";
	argv[6] = threadCountStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isValid());

	EXPECT_EQ(args.threads(), (unsigned int)5);
}

TEST(CppRayTracerChallenge_Core_IO_Args, threads_cant_be_negative_numbers)
{
	auto a = ArgsTest();

	char* threadCountStr = { "-5" };

	SETUP_ARGS(7);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;
	argv[5] = "-t";
	argv[6] = threadCountStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());
}

TEST(CppRayTracerChallenge_Core_IO_Args, threads_cant_be_strings)
{
	auto a = ArgsTest();

	char* threadCountStr = { "adgfad" };

	SETUP_ARGS(7);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;
	argv[5] = "-t";
	argv[6] = threadCountStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());
}

TEST(CppRayTracerChallenge_Core_IO_Args, threads_cant_be_empty)
{
	auto a = ArgsTest();

	char* threadCountStr = { "" };

	SETUP_ARGS(7);
	argv[1] = "-i";
	argv[2] = a.inputDirStr;
	argv[3] = "-o";
	argv[4] = a.outputDirStr;
	argv[5] = "-t";
	argv[6] = threadCountStr;

	Args args = Args(argc, argv);

	EXPECT_TRUE(args.isInvalid());
}
