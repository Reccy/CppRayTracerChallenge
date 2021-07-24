#include "gtest/gtest.h"
#include "math/matrix.h"
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Matrix, construct_blank_matrix_4x4)
{
	Matrix<float, 4, 4> matrix;

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
	Matrix<float, 4, 4> matrix({
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
	Matrix<float, 2, 2> matrix({
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
	Matrix<float, 3, 3> matrix({
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
	Matrix<float, 2, 5> matrix({
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
	Matrix<float, 5, 2> matrix({
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
	Matrix<int, 4, 4> a({
		1,2,3,4,
		5,6,7,8,
		9,8,7,6,
		5,4,3,2
	});

	Matrix<int, 4, 4> b({
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	EXPECT_EQ(a, b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_different_matrices_as_ints)
{
	Matrix<int, 4, 4> a({
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<int, 4, 4> b({
		2,3,4,5,
		6,7,8,9,
		8,7,6,5,
		4,3,2,1
	});

	EXPECT_FALSE(a == b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_identical_matrices_as_floats)
{
	Matrix<float, 4, 4> a({
		1.5, 2.5, 3.5, 4.5,
		5.5, 6.5, 7.5, 8.5,
		9.5, 8.5, 7.5, 6.5,
		5.5, 4.5, 3.5, 2.5
	});

	Matrix<float, 4, 4> b({
		1.5, 2.5, 3.5, 4.5,
		5.5, 6.5, 7.5, 8.5,
		9.5, 8.5, 7.5, 6.5,
		5.5, 4.5, 3.5, 2.5
	});

	EXPECT_EQ(a, b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_equality_with_different_matrices_as_floats)
{
	Matrix<float, 4, 4> a({
		1.5, 2.5, 3.5, 4.5,
		5.5, 6.5, 7.5, 8.5,
		9.5, 8.5, 7.5, 6.5,
		5.5, 4.5, 3.5, 2.5
	});

	Matrix<float, 4, 4> b({
		2.5, 3.5, 4.5, 5.5,
		6.5, 7.5, 8.5, 9.5,
		8.5, 7.5, 6.5, 5.5,
		4.5, 3.5, 2.5, 1.5
	});

	EXPECT_FALSE(a == b);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_4x4)
{
	Matrix<float, 4, 4> a({
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<float, 4, 4> b({
		-2, 1, 2, 3,
		3, 2, 1, -1,
		4, 3, 6, 5,
		1, 2, 7, 8
	});

	Matrix<float, 4, 4> expectedResult({
		20, 22, 50, 48,
		44, 54, 114, 108,
		40, 58, 110, 102,
		16, 26, 46, 42
	});

	EXPECT_EQ(a * b, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_3x2_2x4)
{
	Matrix<float, 3, 2> a({
		1, 3,
		2, 4,
		2, 5
	});

	Matrix<float, 2, 4> b({
		1, 3, 2, 2,
		2, 4, 5, 1
	});

	Matrix<float, 3, 4> expectedResult({
		7, 15, 17, 5,
		10, 22, 24, 8,
		12, 26, 29, 9
	});

	EXPECT_EQ(a * b, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_is_non_commutative)
{
	Matrix<float, 4, 4> a({
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 8, 7, 6,
		5, 4, 3, 2
	});

	Matrix<float, 4, 4> b({
		-2, 1, 2, 3,
		3, 2, 1, -1,
		4, 3, 6, 5,
		1, 2, 7, 8
	});

	EXPECT_FALSE(a * b == b * a);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_with_tuple)
{
	Matrix<float, 4, 4> matrix({
		1, 2, 3, 4,
		2, 4, 4, 2,
		8, 6, 4, 1,
		0, 0, 0, 1
	});

	Tuple<float> tuple(1, 2, 3, 1);

	Tuple<float> expectedResult(18, 24, 33, 1);

	EXPECT_EQ(matrix * tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiplication_undefined)
{
	Matrix<float, 2, 5> a;

	Matrix<float, 2, 5> b;

	try
	{
		a* b;
		FAIL();
	}
	catch (const MatrixUndefinedProductException& err)
	{
		ASSERT_STREQ("Cannot multiply Matrix with 5 columns and Matrix with 2 rows.\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, identity_matrix_3x3)
{
	Matrix<float, 3, 3> matrix({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	});

	auto identity = Matrix<float, 3, 3>::identity();

	EXPECT_EQ(matrix * identity, matrix);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, identity_matrix_4x4)
{
	Matrix<float, 4, 4> matrix({
		1, 2, 3, 4,
		2, 4, 4, 2,
		8, 6, 4, 1,
		0, 0, 0, 1
	});

	auto identity = Matrix<float, 4, 4>::identity();

	EXPECT_EQ(matrix * identity, matrix);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_4x4)
{
	Matrix<float, 4, 4> matrix({
		0, 9, 3, 0,
		9, 8, 0, 8,
		1, 8, 5, 3,
		0, 0, 5, 8
	});

	Matrix<float, 4, 4> expectedResult({
		0, 9, 1, 0,
		9, 8, 8, 0,
		3, 0, 5, 5,
		0, 8, 3, 8
	});

	EXPECT_EQ(matrix.transpose(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_2x4)
{
	Matrix<float, 2, 4> matrix({
		1, 2, 3, 4,
		5, 6, 7, 8
	});

	Matrix<float, 4, 2> expectedResult({
		1, 5,
		2, 6,
		3, 7,
		4, 8
	});

	EXPECT_EQ(matrix.transpose(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_transpose_identity)
{
	auto identity = Matrix<float, 4, 4>::identity();

	EXPECT_EQ(identity.transpose(), identity);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_non_square)
{
	Matrix<float, 1, 2> matrix;

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

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_1x1)
{
	Matrix<float, 1, 1> matrix({ 5 });

	float expectedResult = 5;

	EXPECT_EQ(matrix.determinant(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_2x2)
{
	Matrix<float, 2, 2> matrix({
		1, 5,
		-3, 2
	});

	float expectedResult = 17;

	EXPECT_EQ(matrix.determinant(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_3x3)
{
	Matrix<float, 3, 3> matrix({
		1, 2, 6,
		-5, 8, -4,
		2, 6, 4
	});

	EXPECT_EQ(matrix.cofactor(0, 0), 56);
	EXPECT_EQ(matrix.cofactor(0, 1), 12);
	EXPECT_EQ(matrix.cofactor(0, 2), -46);
	EXPECT_EQ(matrix.determinant(), -196);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_determinant_4x4)
{
	Matrix<float, 4, 4> matrix({
		-2, -8, 3, 5,
		-3, 1, 7, 3,
		1, 2, -9, 6,
		-6, 7, 7, -9
	});

	EXPECT_EQ(matrix.cofactor(0, 0), 690);
	EXPECT_EQ(matrix.cofactor(0, 1), 447);
	EXPECT_EQ(matrix.cofactor(0, 2), 210);
	EXPECT_EQ(matrix.cofactor(0, 3), 51);
	EXPECT_EQ(matrix.determinant(), -4071);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_3x3_is_2x2)
{
	Matrix<float, 3, 3> matrix({
		1, 5, 0,
		-3, 2, 7,
		0, 6, -3
	});

	Matrix<float, 2, 2> expectedResult({
		-3, 2,
		0, 6
	});

	EXPECT_EQ(matrix.submatrix(0, 2), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_4x4_is_3x3)
{
	Matrix<float, 4, 4> matrix({
		-6, 1, 1, 6,
		-8, 5, 8, 6,
		-1, 0, 8, 2,
		-7, 1, -1, 1
	});

	Matrix<float, 3, 3> expectedResult({
		-6, 1, 6,
		-8, 8, 6,
		-7, -1, 1
	});

	EXPECT_EQ(matrix.submatrix(2, 1), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, submatrix_of_1x1_throws_exception)
{
	Matrix<float, 1, 1> matrix;

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
	Matrix<float, 3, 3> matrix({
		3, 5, 0,
		2, -1, -7,
		6, -1, 5
	});

	auto submatrix = matrix.submatrix(1, 0);

	float expectedResult = 25;

	EXPECT_EQ(submatrix.determinant(), expectedResult);
	EXPECT_EQ(matrix.minor(1, 0), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, cofactor_of_a_3x3_matrix)
{
	Matrix<float, 3, 3> matrix({
		3, 5, 0,
		2, -1, -7,
		6, -1, 5
	});

	EXPECT_EQ(matrix.minor(0, 0), -12);
	EXPECT_EQ(matrix.cofactor(0, 0), -12);
	EXPECT_EQ(matrix.minor(1, 0), 25);
	EXPECT_EQ(matrix.cofactor(1, 0), -25);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_is_invertible)
{
	Matrix<float, 4, 4> matrix({
		6, 4, 4, 4,
		5, 5, 7, 6,
		4, -9, 3, -7,
		9, 1, 7, -6
	});

	EXPECT_EQ(matrix.determinant(), -2120);
	EXPECT_EQ(matrix.invertible(), true);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_is_not_invertible)
{
	Matrix<float, 4, 4> matrix({
		-4, 2, -2, -3,
		9, 6, 2, 6,
		0, -5, 1, -5,
		0, 0, 0, 0
	});

	EXPECT_EQ(matrix.determinant(), 0);
	EXPECT_EQ(matrix.invertible(), false);

	try
	{
		matrix.invert();
		FAIL();
	}
	catch (const MatrixNotInvertibleException& err)
	{
		EXPECT_STREQ("Cannot invert Matrix as its determinant is 0\n", err.what());
	}
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_inversion_1)
{
	Matrix<float, 4, 4> a({
		-5, 2, 6, -8,
		1, -5, 1, 8,
		7, 7, -6, -7,
		1, -3, 7, 4
	});

	Matrix b = a.invert();

	Matrix<float, 4, 4> expectedResult({
		0.21805f, 0.45113f, 0.24060f, -0.04511f,
		-0.80827f, -1.45677f, -0.44361f, 0.52068f,
		-0.07895f, -0.22368f, -0.05263f, 0.19737f,
		-0.52256f, -0.81391f, -0.30075f, 0.30639f
	});

	EXPECT_EQ(a.determinant(), 532);
	EXPECT_EQ(a.cofactor(2, 3), -160);
	EXPECT_EQ(b(3, 2), -160.f / 532.f);
	EXPECT_EQ(a.cofactor(3, 2), 105.f);
	EXPECT_EQ(b(2, 3), 105.f / 532.f);
	EXPECT_EQ(b, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_inversion_2)
{
	Matrix<float, 4, 4> matrix({
		8, -5, 9, 2,
		7, 5, 6, 1,
		-6, 0, 9, 6,
		-3, 0, -9, -4
	});

	Matrix<float, 4, 4> expectedResult({
		-0.153846f, -0.153846f, -0.282051f, -0.538462f,
		-0.0769231f, 0.123077f, 0.025641f, 0.0307692f,
		0.358974f, 0.358974f, 0.435897f, 0.923077f,
		-0.692308f, -0.692308f, -0.769231f, -1.92308f
	});

	EXPECT_EQ(matrix.invert(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_inversion_3)
{
	Matrix<float, 4, 4> matrix({
		9, 3, 0, 9,
		-5, -2, -6, -3,
		-4, 9, 6, 4,
		-7, 6, 6, 2
	});

	Matrix<float, 4, 4> expectedResult({
		-0.0407407f, -0.0777778f, 0.144444f, -0.222222f,
		- 0.0777778f, 0.0333333f, 0.366667f, -0.333333f,
		- 0.0290123f, -0.146296f, -0.109259f, 0.12963f,
		0.177778f, 0.0666667f, -0.266667f, 0.333333f
	});

	EXPECT_EQ(matrix.invert(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Matrix, matrix_multiply_by_inverse)
{
	Matrix<float, 4, 4> a({
		3, -9, 7, 3,
		3, -8, 2, -9,
		-4, 4, 4, 1,
		-6, 5, -1, 1
	});

	Matrix<float, 4, 4> b({
		8, 2, 2, 2,
		3, -1, 7, 0,
		7, 0, 5, 4,
		6, -2, 0, 5
	});

	Matrix c = a * b;

	EXPECT_EQ(c * b.invert(), a);
}
