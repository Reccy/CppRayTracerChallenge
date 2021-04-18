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

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_0x1)
{
	try
	{
		Matrix<float> matrix(0, 1);
		FAIL();
	}
	catch (const MatrixTooSmallException& err)
	{
		EXPECT_STREQ("Cannot create a Matrix with 0 or less rows / columns\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_1x0)
{
	try
	{
		Matrix<float> matrix(1, 0);
		FAIL();
	}
	catch (const MatrixTooSmallException& err)
	{
		EXPECT_STREQ("Cannot create a Matrix with 0 or less rows / columns\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_0x1_with_data)
{
	try
	{
		Matrix<float> matrix(0, 1, std::vector<float> { 1 });
		FAIL();
	}
	catch (const MatrixTooSmallException& err)
	{
		EXPECT_STREQ("Cannot create a Matrix with 0 or less rows / columns\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_1x0_with_data)
{
	try
	{
		Matrix<float> matrix(1, 0, std::vector<float> { 1 });
		FAIL();
	}
	catch (const MatrixTooSmallException& err)
	{
		EXPECT_STREQ("Cannot create a Matrix with 0 or less rows / columns\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_1x1_with_too_much_data)
{
	try
	{
		Matrix<float> matrix(1, 1, std::vector<float> {0, 1});
		FAIL();
	}
	catch (const MatrixInitialDataSizeMismatch& err)
	{
		EXPECT_STREQ("Failed to construct Matrix. Initial data size is 2. Expected: 1\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_matrix_1x1_with_too_little_data)
{
	try
	{
		Matrix<float> matrix(2, 2, std::vector<float> {0, 1, 3});
		FAIL();
	}
	catch (const MatrixInitialDataSizeMismatch& err)
	{
		EXPECT_STREQ("Failed to construct Matrix. Initial data size is 3. Expected: 4\n", err.what());
	}
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

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_4x4)
{
	Matrix<float> matrix(4, 4, std::vector<float> {
		0, 9, 3, 0,
		9, 8, 0, 8,
		1, 8, 5, 3,
		0, 0, 5, 8
	});

	Matrix<float> expectedResult(4, 4, std::vector<float> {
		0, 9, 1, 0,
		9, 8, 8, 0,
		3, 0, 5, 5,
		0, 8, 3, 8
	});

	EXPECT_EQ(matrix.transpose(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_2x4)
{
	Matrix<float> matrix(2, 4, std::vector<float> {
		1, 2, 3, 4,
		5, 6, 7, 8
	});

	Matrix<float> expectedResult(4, 2, std::vector<float> {
		1, 5,
		2, 6,
		3, 7,
		4, 8
	});

	EXPECT_EQ(matrix.transpose(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_identity)
{
	Matrix<float> identity = Matrix<float>::identity(4);

	EXPECT_EQ(identity.transpose(), identity);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_non_square)
{
	Matrix<float> matrix(1, 2);

	try
	{
		matrix.determinant();
		FAIL();
	}
	catch (const MatrixUndefinedDeterminantException& err)
	{
		ASSERT_STREQ("Cannot calculate determinant for a non-square Matrix. There are 1 rows and 2 columns.\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_2x2)
{
	Matrix<float> matrix(2, 2, std::vector<float> {
		1, 5,
		-3, 2
	});

	float expectedResult = 17;

	EXPECT_EQ(matrix.determinant(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_3x3_is_2x2)
{
	Matrix<float> matrix(3, 3, std::vector<float> {
		1, 5, 0,
		-3, 2, 7,
		0, 6, -3
	});

	Matrix<float> expectedResult(2, 2, std::vector<float> {
		-3, 2,
		0, 6
	});

	EXPECT_EQ(matrix.submatrix(0, 2), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_4x4_is_3x3)
{
	Matrix<float> matrix(4, 4, std::vector<float> {
		-6, 1, 1, 6,
		-8, 5, 8, 6,
		-1, 0, 8, 2,
		-7, 1, -1, 1
	});

	Matrix<float> expectedResult(3, 3, std::vector<float> {
		-6, 1, 6,
		-8, 8, 6,
		-7, -1, 1
	});

	EXPECT_EQ(matrix.submatrix(2, 1), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_1x1_throws_exception)
{
	Matrix<float> matrix(1, 1);

	try
	{
		matrix.submatrix(0, 0);
		FAIL();
	}
	catch (const MatrixTooSmallException& err)
	{
		EXPECT_STREQ("Cannot create a Matrix with 0 or less rows / columns\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, minor_of_a_3x3_matrix)
{
	Matrix<float> matrix(3, 3, std::vector<float>{
		3, 5, 0,
		2, -1, -7,
		6, -1, 5
	});

	Matrix<float> submatrix = matrix.submatrix(1, 0);

	float expectedResult = 25;

	EXPECT_EQ(submatrix.determinant(), expectedResult);
	EXPECT_EQ(matrix.minor(1, 0), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, cofactor_of_a_3x3_matrix)
{
	Matrix<float> matrix(3, 3, std::vector<float> {
		3, 5, 0,
		2, -1, -7,
		6, -1, 5
	});

	EXPECT_EQ(matrix.minor(0, 0), -12);
	EXPECT_EQ(matrix.cofactor(0, 0), -12);
	EXPECT_EQ(matrix.minor(1, 0), 25);
	EXPECT_EQ(matrix.cofactor(1, 0), -25);
}