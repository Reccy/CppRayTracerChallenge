#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX

#include "comparison.h"

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

		bool operator==(const Matrix& other) const
		{
			if (other.m_data.size() != this->m_data.size())
			{
				return false;
			}

			for (int i = 0; i < m_data.size(); ++i)
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
	private:
		const int m_rows, m_columns;
		std::vector<T> m_data;

		int indexAt(const int row, const int column)
		{
			return row * m_columns + column;
		};
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_MATRIX
