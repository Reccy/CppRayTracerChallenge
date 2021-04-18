#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX

#include "comparison.h"
#include "tuple.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A Matrix of numbers
	/// </summary>
	/// <typeparam name="T">The type of number to store, e.g. int, float, double...</typeparam>
	template<typename T>
	class Matrix
	{
	public:
		Matrix() = delete;

		/// <summary>
		/// Creates a Matrix of size rows x columns.
		/// Initializes all data to 0.
		/// </summary>
		/// <param name="rows">The amount of rows in the Matrix</param>
		/// <param name="columns">The amount of columns in the Matrix</param>
		Matrix(const int rows, const int columns) : m_rows(rows), m_columns(columns), m_data(std::vector<T>(rows * columns, 0)) {};

		/// <summary>
		/// Creates a Matrix of size rows x columns.
		/// Initializes all data to the value of initialData
		/// </summary>
		/// <param name="rows">The amount of rows in the Matrix</param>
		/// <param name="columns">The amount of columns in the Matrix</param>
		/// <param name="initialData">List of data used to initialize the Matrix</param>
		Matrix(const int rows, const int columns, std::vector<T> initialData) : m_rows(rows), m_columns(columns), m_data(initialData) {};

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

		Matrix operator*(const Matrix& other) const
		{
			if (this->m_columns != other.m_rows)
			{
				throw MatrixUndefinedProductException(this->m_columns, other.m_rows);
			}

			const int newRows = m_rows;
			const int newColumns = other.m_columns;

			Matrix<T> result(newRows, newColumns);

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
			Matrix matrix(4, 1, std::vector<T> {
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

		bool operator==(const Matrix& other) const
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
		std::vector<T> m_data;

		int indexAt(const int row, const int column) const
		{
			return row * m_columns + column;
		};
	};

	class MatrixUndefinedProductException : public std::exception {
	public:
		MatrixUndefinedProductException(const int a_columns, const int b_rows) : m_aColumns(a_columns), m_bRows(b_rows)
		{
			std::stringstream ss;
			ss << "Cannot multiply Matrix with " << m_aColumns << " columns and Matrix with " << m_bRows << " rows.\n";
			m_what = ss.str();
		}

		const char* what() const throw() {
			return m_what.c_str();
		}
	private:
		std::string m_what;
		int m_aColumns, m_bRows;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
