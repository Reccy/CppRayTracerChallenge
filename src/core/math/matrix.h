#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX

#include <array>
#include <sstream>
#include "comparison.h"
#include "tuple.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A Matrix of numbers
	/// </summary>
	/// <typeparam name="T">The type of number to store, e.g. int, float, double...</typeparam>
	template<class T, int ROW, int COL>
	class Matrix
	{
	public:
		template<class T, int ROW2, int COL2>
		friend class Matrix;

		/// <summary>
		/// Creates a Matrix
		/// Initializes all data to 0.
		/// </summary>
		Matrix() : m_rows(ROW), m_columns(COL), m_data(std::array<T, ROW * COL>()) {
			static_assert(ROW > 0, "ROW is too small");
			static_assert(COL > 0, "COL is too small");
		};

		/// <summary>
		/// Creates a Matrix
		/// Initializes all data to the value of initialData
		/// </summary>
		/// <param name="rows">The amount of rows in the Matrix</param>
		/// <param name="columns">The amount of columns in the Matrix</param>
		/// <param name="initialData">List of data used to initialize the Matrix</param>
		Matrix(std::array<T, ROW * COL> initialData) : m_rows(ROW), m_columns(COL), m_data(initialData) {
			static_assert(ROW > 0, "ROW is too small");
			static_assert(COL > 0, "COL is too small");

			if (m_data.size() != m_rows * m_columns)
			{
				throw MatrixInitialDataSizeMismatch((int)m_data.size(), m_rows * m_columns);
			}
		};

		/// <summary>
		/// Creates a square Identity Matrix of a specific size
		/// </summary>
		/// <param name="size">The size of the matrix</param>
		/// <returns>A square Identity Matrix of n x n size</returns>
		static Matrix<T, ROW, COL> identity()
		{
			Matrix<T, ROW, COL> matrix;

			constexpr int size = ROW * COL;

			for (int col = 0; col < COL; ++col)
			{
				for (int row = 0; row < ROW; ++row)
				{
					if (row == col) matrix(row, col) = 1;
				}
			}

			return matrix;
		}

		/// <summary>
		/// Subscript operator, accesses the value at row, column
		/// </summary>
		/// <param name="row">The row to access</param>
		/// <param name="column">The column to access</param>
		T& operator() (const int row, const int column)
		{
			return m_data[indexAt(row, column)];
		};

		/// <summary>
		/// Subscript operator, accesses the value at row, column as readonly
		/// </summary>
		/// <param name="row">The row to access</param>
		/// <param name="column">The column to access</param>
		T operator() (const int row, const int column) const
		{
			return m_data[indexAt(row, column)];
		};

		/// <summary>
		/// Transposes the Matrix, changing its rows into columns, and columns into rows
		/// </summary>
		/// <returns>A Matrix with transposed elements</returns>
		Matrix<T, COL, ROW> transpose() const
		{
			constexpr int transposedColumns = ROW;
			constexpr int transposedRows = COL;

			Matrix<T, transposedRows, transposedColumns> result;

			for (int row = 0; row < transposedRows; ++row)
			{
				for (int col = 0; col < transposedColumns; ++col)
				{
					result(row, col) = m_data[indexAt(col, row)];
				}
			}

			return result;
		}

		auto submatrix(const int removeRow, const int removeColumn) const;

		/// <summary>
		/// Calculates the determinant of the square Matrix
		/// </summary>
		/// <returns>The determinant</returns>
		T determinant() const
		{
			if (m_determinant != 0)
				return m_determinant;

			if constexpr (ROW != COL)
			{
				throw MatrixUndefinedDeterminantException(m_rows, m_columns);
			}
			else
			{
				constexpr int MATRIX_SIZE = ROW;

				if constexpr (MATRIX_SIZE == 1)
				{
					return m_data[0];
				}
				else if constexpr (MATRIX_SIZE == 2)
				{
					return m_data[indexAt(0, 0)] * m_data[indexAt(1, 1)] - m_data[indexAt(1, 0)] * m_data[indexAt(0, 1)];
				}
				else
				{
					T result = 0;

					for (int col = 0; col < m_columns; ++col)
					{
						result += cofactor(0, col) * m_data[indexAt(0, col)];
					}

					m_determinant = result;
					return result;
				}
			}
		}

		/// <summary>
		/// Calculates the minor / determinant of the square submatrix.
		/// </summary>
		/// <param name="removeRow">The row to remove</param>
		/// <param name="removeColumn">The column to remove</param>
		/// <returns>The determinant of the submatrix</returns>
		T minor(const int removeRow, const int removeColumn) const
		{
			return submatrix(removeRow, removeColumn).determinant();
		}

		/// <summary>
		/// Calculates the cofactor of the square Matrix
		/// </summary>
		/// <param name="removeRow">The row to remove</param>
		/// <param name="removeColumn">The column to remove</param>
		/// <returns>The cofactor of the Matrix</returns>
		T cofactor(const int removeRow, const int removeColumn) const
		{
			T result = minor(removeRow, removeColumn);

			if ((removeRow + removeColumn) % 2 == 1)
			{
				result = -result;
			}

			return result;
		}

		/// <summary>
		/// Checks if the Matrix can be inverted
		/// </summary>
		/// <returns>True if it can be inverted, else false</returns>
		bool invertible() const
		{
			return determinant() != 0;
		}

		/// <summary>
		/// Creates a copy of the Matrix that is inverted
		/// </summary>
		/// <returns>The inverted Matrix</returns>
		Matrix invert() const
		{
			if (!!m_inverted)
			{
				return *m_inverted.get();
			}

			if (!invertible())
			{
				throw MatrixNotInvertibleException();
			}

			Matrix<T, ROW, COL> result;

			for (int row = 0; row < m_rows; ++row)
			{
				for (int col = 0; col < m_columns; ++col)
				{
					T c = cofactor(row, col);

					result(col, row) = c / determinant();
				}
			}

			m_inverted = std::make_shared<Matrix<T, COL, ROW>>(result);

			return result;
		}

		template<int ROW2, int COL2>
		Matrix<T, ROW, COL2> operator*(const Matrix<T, ROW2, COL2> & other) const
		{
			if (this->m_columns != other.m_rows)
			{
				throw MatrixUndefinedProductException(this->m_columns, other.m_rows);
			}

			const int newRows = m_rows;
			const int newColumns = other.m_columns;

			Matrix<T, ROW, COL2> result;

			for (int row = 0; row < newRows; ++row)
			{
				for (int col = 0; col < newColumns; ++col)
				{
					T value = 0;

					for (int i = 0; i < m_columns; ++i)
					{
						value += this->m_data[indexAt(row, i)] * other(i, col);
					}

					result(row, col) = value;
				}
			}

			return result;
		};

		Tuple<T> operator*(const Tuple<T>& tuple) const
		{
			Matrix<T, 4, 1> matrix({
				tuple.x(),
				tuple.y(),
				tuple.z(),
				tuple.w()
			});

			matrix = *this * matrix;

			Tuple result(
				matrix(0,0),
				matrix(0,1),
				matrix(0,2),
				matrix(0,3)
			);

			return result;
		}

		template<typename T, int ROW2, int COL2>
		bool operator==(const Matrix<T, ROW2, COL2>& other) const
		{
			if (other.m_data.size() != this->m_data.size())
			{
				return false;
			}

			if (this->m_rows != other.m_rows && this->m_columns != other.m_columns)
			{
				return false;
			}

			for (int i = 0; i < this->m_data.size(); ++i)
			{
				if (!Comparison::equal(this->m_data[i], other.m_data[i])) {
					return false;
				}
			}

			return true;
		};

		bool operator!=(const Matrix& other) const
		{
			return !(*this == other);
		};

		friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
		{
			os << matrix.m_rows << " x " << matrix.m_columns << "\n";

			for (int row = 0; row < matrix.m_rows; ++row)
			{
				for (int col = 0; col < matrix.m_columns; ++col)
				{
					os << matrix(row, col);

					if (col != matrix.m_columns - 1)
					{
						os << ", ";
					}
				}
				os << "\n";
			}

			return os;
		}
	private:
		int m_rows, m_columns;
		mutable T m_determinant = 0;
		mutable std::shared_ptr<Matrix<T, ROW, COL>> m_inverted = nullptr;
		std::array<T, ROW * COL> m_data;

		int indexAt(const int row, const int column) const
		{
			return row * m_columns + column;
		};
	};

	/// <summary>
	/// Returns a copy of the Matrix, with the row and column removed.
	/// </summary>
	/// <param name="removeRow">The row to remove</param>
	/// <param name="removeColumn">The column to remove</param>
	/// <returns>The submatrix</returns>
	template<typename T, int ROW, int COL>
	auto Matrix<T, ROW, COL>::submatrix(const int removeRow, const int removeColumn) const
	{
		if constexpr (ROW <= 1 || COL <= 1)
		{
			throw MatrixTooSmallException();
		}
		else
		{
			constexpr int NEW_ROW = ROW - 1;
			constexpr int NEW_COL = COL - 1;

			std::array<T, NEW_ROW * NEW_COL> data{ -1 };

			int i = 0;

			for (int row = 0; row < m_rows; ++row)
			{
				for (int col = 0; col < m_columns; ++col)
				{
					if (col == removeColumn || row == removeRow) continue;

					data.at(i) = m_data[indexAt(row, col)];
					++i;
				}
			}

			Matrix<T, NEW_ROW, NEW_COL> result(data);

			return result;
		}
	}

	class MatrixNotInvertibleException : public std::exception {
	public:
		MatrixNotInvertibleException()
		{
			std::stringstream ss;
			ss << "Cannot invert Matrix as its determinant is 0\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
	};

	class MatrixTooSmallException : public std::exception {
	public:
		MatrixTooSmallException()
		{
			std::stringstream ss;
			ss << "Cannot create a Matrix with 0 or less rows / columns\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
	};

	class MatrixInitialDataSizeMismatch : public std::exception {
	public:
		MatrixInitialDataSizeMismatch(int initialSize, int expectedSize)
		{
			std::stringstream ss;
			ss << "Failed to construct Matrix. Initial data size is " << initialSize << ". Expected: " << expectedSize << "\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
	};

	class MatrixUndefinedDeterminantException : public std::exception {
	public:
		MatrixUndefinedDeterminantException(const int rows, const int columns)
		{
			std::stringstream ss;
			ss << "Cannot calculate determinant for a non-square Matrix. There are " << rows << " rows and " << columns << " columns.\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
	};

	class MatrixUndefinedProductException : public std::exception {
	public:
		MatrixUndefinedProductException(const int aColumns, const int bRows)
		{
			std::stringstream ss;
			ss << "Cannot multiply Matrix with " << aColumns << " columns and Matrix with " << bRows << " rows.\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
