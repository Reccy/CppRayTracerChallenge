#include "gtest/gtest.h"
#include "math/matrix.h"
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_blank_matrix_4x4)
{
	Matrix<float> matrix(4, 4);

	EXPECT_EQ(matrix(0, 0), 0);
	EXPECT_EQ(matrix(0, 1), 0);
	EXPECT_EQ(matrix(0, 2), 0);
	EXPECT_EQ(matrix(0, 3), 0);
	EXPECT_EQ(matrix(1, 0), 0);
	EXPECT_EQ(matrix(1, 1), 0);
	EXPECT_EQ(matrix(1, 2), 0);
	EXPECT_EQ(matrix(1, 3), 0);
	EXPECT_EQ(matrix(2, 0), 0);
	EXPECT_EQ(matrix(2, 1), 0);
	EXPECT_EQ(matrix(2, 2), 0);
	EXPECT_EQ(matrix(2, 3), 0);
	EXPECT_EQ(matrix(3, 0), 0);
	EXPECT_EQ(matrix(3, 1), 0);
	EXPECT_EQ(matrix(3, 2), 0);
	EXPECT_EQ(matrix(3, 3), 0);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_4x4)
{
	Matrix<float> matrix(4, 4, std::vector<float> {
		1, 2, 3, 4,
		5.5, 6.5, 7.5, 8.5,
		9, 10, 11, 12,
		13.5, 14.5, 15.5, 16.5
	});

	EXPECT_EQ(matrix(0, 0), 1);
	EXPECT_EQ(matrix(0, 1), 2);
	EXPECT_EQ(matrix(0, 2), 3);
	EXPECT_EQ(matrix(0, 3), 4);
	EXPECT_EQ(matrix(1, 0), 5.5f);
	EXPECT_EQ(matrix(1, 1), 6.5f);
	EXPECT_EQ(matrix(1, 2), 7.5f);
	EXPECT_EQ(matrix(1, 3), 8.5f);
	EXPECT_EQ(matrix(2, 0), 9);
	EXPECT_EQ(matrix(2, 1), 10);
	EXPECT_EQ(matrix(2, 2), 11);
	EXPECT_EQ(matrix(2, 3), 12);
	EXPECT_EQ(matrix(3, 0), 13.5f);
	EXPECT_EQ(matrix(3, 1), 14.5f);
	EXPECT_EQ(matrix(3, 2), 15.5f);
	EXPECT_EQ(matrix(3, 3), 16.5f);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_2x2)
{
	Matrix<float> matrix(2, 2, std::vector<float> {
		-3, 5,
		1, 2
	});

	EXPECT_EQ(matrix(0, 0), -3);
	EXPECT_EQ(matrix(0, 1), 5);
	EXPECT_EQ(matrix(1, 0), 1);
	EXPECT_EQ(matrix(1, 1), 2);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_3x3)
{
	Matrix<float> matrix(3, 3, std::vector<float> {
		-3, 5, 0,
		1, -2, -7,
		0, 1, 1
	});

	EXPECT_EQ(matrix(0, 0), -3);
	EXPECT_EQ(matrix(0, 1), 5);
	EXPECT_EQ(matrix(0, 2), 0);
	EXPECT_EQ(matrix(1, 0), 1);
	EXPECT_EQ(matrix(1, 1), -2);
	EXPECT_EQ(matrix(1, 2), -7);
	EXPECT_EQ(matrix(2, 0), 0);
	EXPECT_EQ(matrix(2, 1), 1);
	EXPECT_EQ(matrix(2, 2), 1);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_2x5)
{
	Matrix<float> matrix(2, 5, std::vector<float> {
		1, 2, 3, 4, 5,
		6.5, 7.5, 8.5, 9.5, 10.5
	});

	EXPECT_EQ(matrix(0, 0), 1);
	EXPECT_EQ(matrix(0, 1), 2);
	EXPECT_EQ(matrix(0, 2), 3);
	EXPECT_EQ(matrix(0, 3), 4);
	EXPECT_EQ(matrix(0, 4), 5);
	EXPECT_EQ(matrix(1, 0), 6.5f);
	EXPECT_EQ(matrix(1, 1), 7.5f);
	EXPECT_EQ(matrix(1, 2), 8.5f);
	EXPECT_EQ(matrix(1, 3), 9.5f);
	EXPECT_EQ(matrix(1, 4), 10.5f);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_5x2)
{
	Matrix<float> matrix(5, 2, std::vector<float> {
		1, 2,
		3, 4,
		5, 6,
		7, 8,
		9, 10,
	});

	EXPECT_EQ(matrix(0, 0), 1);
	EXPECT_EQ(matrix(0, 1), 2);
	EXPECT_EQ(matrix(1, 0), 3);
	EXPECT_EQ(matrix(1, 1), 4);
	EXPECT_EQ(matrix(2, 0), 5);
	EXPECT_EQ(matrix(2, 1), 6);
	EXPECT_EQ(matrix(3, 0), 7);
	EXPECT_EQ(matrix(3, 1), 8);
	EXPECT_EQ(matrix(4, 0), 9);
	EXPECT_EQ(matrix(4, 1), 10);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_identical_matrices_as_ints)
{
	Matrix<int> a(4, 4, std::vector<int> {
		1,2,3,4,
		5,6,7,8,
		9,8,7,6,
		5,4,3,2
	});

	Matrix<int> b(4, 4, std::vector<int> {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	EXPECT_EQ(a, b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_different_matrices_as_ints)
{
	Matrix<int> a(4, 4, std::vector<int> {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<int> b(4, 4, std::vector<int> {
		2,3,4,5,
		6,7,8,9,
		8,7,6,5,
		4,3,2,1
	});

	EXPECT_FALSE(a == b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_identical_matrices_as_floats)
{
	Matrix<float> a(4, 4, std::vector<float> {
		1.5, 2.5, 3.5, 4.5,
			5.5, 6.5, 7.5, 8.5,
			9.5, 8.5, 7.5, 6.5,
			5.5, 4.5, 3.5, 2.5
	});

	Matrix<float> b(4, 4, std::vector<float> {
		1.5, 2.5, 3.5, 4.5,
			5.5, 6.5, 7.5, 8.5,
			9.5, 8.5, 7.5, 6.5,
			5.5, 4.5, 3.5, 2.5
	});

	EXPECT_EQ(a, b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_different_matrices_as_floats)
{
	Matrix<float> a(4, 4, std::vector<float> {
		1.5, 2.5, 3.5, 4.5,
		5.5, 6.5, 7.5, 8.5,
		9.5, 8.5, 7.5, 6.5,
		5.5, 4.5, 3.5, 2.5
	});

	Matrix<float> b(4, 4, std::vector<float> {
		2.5, 3.5, 4.5, 5.5,
		6.5, 7.5, 8.5, 9.5,
		8.5, 7.5, 6.5, 5.5,
		4.5, 3.5, 2.5, 1.5
	});

	EXPECT_FALSE(a == b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_4x4)
{
	Matrix<float> a(4, 4, std::vector<float> {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<float> b(4, 4, std::vector<float> {
		-2, 1, 2, 3,
		3, 2, 1, -1,
		4, 3, 6, 5,
		1, 2, 7, 8
	});

	Matrix<float> expectedResult(4, 4, std::vector<float> {
		20, 22, 50, 48,
		44, 54, 114, 108,
		40, 58, 110, 102,
		16, 26, 46, 42
	});

	EXPECT_EQ(a * b, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_3x2_2x4)
{
	Matrix<float> a(3, 2, std::vector<float> {
		1, 3,
		2, 4,
		2, 5
	});

	Matrix<float> b(2, 4, std::vector<float> {
		1, 3, 2, 2,
		2, 4, 5, 1
	});

	Matrix<float> expectedResult(3, 4, std::vector<float> {
		7, 15, 17, 5,
		10, 22, 24, 8,
		12, 26, 29, 9
	});

	EXPECT_EQ(a * b, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_is_non_commutative)
{
	Matrix<float> a(4, 4, std::vector<float> {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<float> b(4, 4, std::vector<float> {
		-2, 1, 2, 3,
		3, 2, 1, -1,
		4, 3, 6, 5,
		1, 2, 7, 8
	});

	EXPECT_FALSE(a * b == b * a);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_undefined)
{
	Matrix<float> a(2, 5);

	Matrix<float> b(2, 5);

	try
	{
		a * b;
		FAIL();
	}
	catch (const MatrixUndefinedProductException& err)
	{
		ASSERT_STREQ("Cannot multiply Matrix with 5 columns and Matrix with 2 rows.\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_with_tuple)
{
	Matrix<float> matrix(4, 4, std::vector<float> {
		1, 2, 3, 4,
		2, 4, 4, 2,
		8, 6, 4, 1,
		0, 0, 0, 1
	});

	Tuple<float> tuple(1, 2, 3, 1);

	Tuple<float> expectedResult(18, 24, 33, 1);

	EXPECT_EQ(matrix * tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, identity_matrix_3x3)
{
	Matrix<float> matrix(3, 3, std::vector<float> {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	});

	Matrix<float> identity = Matrix<float>::identity(3);

	EXPECT_EQ(matrix * identity, matrix);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, identity_matrix_4x4)
{
	Matrix<float> matrix(4, 4, std::vector<float> {
		1, 2, 3, 4,
		2, 4, 4, 2,
		8, 6, 4, 1,
		0, 0, 0, 1
	});

	Matrix<float> identity = Matrix<float>::identity(4);

	EXPECT_EQ(matrix * identity, matrix);
}
