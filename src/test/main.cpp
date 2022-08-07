#include "gtest/gtest.h"
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	auto result = RUN_ALL_TESTS();

	std::cout << "\nTest Suite Finished\n"
		<< "Press any key to continue" << std::endl;

	std::cin.get();

	return result;
}
