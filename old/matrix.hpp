//
// FILENAME: matrix.hpp | Shifting Stones Search
// DESCRIPTION: A matrix of of integral type and arbitrary size
//

///
/// @file matrix.hpp
/// @brief A matrix of of integral type and arbitrary size 
///

#pragma once

#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <cassert>
#include <iterator>
#include <type_traits>

//
// Define the Matrix and Row class templates
//
// In C++20 and later, the first template argument (the type of data the matrix
// stores) can be constrained using a concept. In earlier versions of C++, a similar
// result can be achieved using std::enable_if.
//
// The matrix row type can be represented as a class containing an underlying array of
// a type that requires IntegralType, or with std::enable_if as described above.
//

#if __cplusplus >= __cpp_concepts
	namespace __detail {
		template<typename _T>
		concept IntegralType = std::is_integral<_T>::value;
	}
#endif

///
/// @class Row
/// @brief Represent a matrix row
///
#if __cplusplus >= __cpp_concepts
	template<__detail::IntegralType Type>
#else
	template<class Type>
#endif
class Row {
public:
	using value_type 	  	= Type;
	using pointer 	  		= value_type*;
	using const_pointer		= const value_type*;
	using reference 	  	= value_type&;
	using const_reference 	= const value_type&;
	using size_type		  	= std::size_t;

public:
	Row() = default;

	///
	/// @brief Construct a new Row with all elements the same value
	/// @param value The value to store in the row
	/// @param size The number of values in the row
	///
	explicit Row(value_type value, size_type size):
		m_Row(std::vector<value_type>(size, value))
	{}

	///
	/// @brief Construct a new Row from a @c std::vector
	/// @param row A @c std::vector to construct the row from
	///
	Row(const std::vector<value_type>& row):
		m_Row(row)
	{}

	///
	/// @brief Construct a new Row from an initializer list
	/// @param elements An initializer list of values
	///
	Row(std::initializer_list<value_type> elements):
		m_Row(std::vector<value_type>(elements))
	{}

	///
	/// @brief Construct a new Row from a C array
	/// @param row The array to construct the row from
	///
	Row(const_pointer row, std::size_t size):
		m_Row(std::vector<value_type>(row, row + size))
	{}

	///
	/// @brief Get the number of elements in the row
	/// @return The size of the Row's underlying vector
	///
	constexpr size_type size() const{
		return m_Row.size();
	}

	///
	/// @brief Get the row's underlying data as a vector
	/// @return The Row's underlying vector
	///
	const std::vector<value_type>& vector() const {
		return m_Row;
	}

	///
	/// @brief Get the row's underlying data as an array
	/// @return A pointer to Row's underlying array (the underlying array of the underlying vector)
	///
	const_pointer data() const {
		return m_Row.data();
	}

	/// @brief Allow indexing with assignment
	/// @param index The index of the element to access
	/// @return A reference to the accessed element
	///
	reference operator[](size_type index) {
		return m_Row[index];
	}
	
	/// @brief Allow indexing without assignment
	/// @param index The index of the element to access
	/// @return A constant reference to the accessed element
	const_reference operator[](size_type index) const {
		return m_Row[index];
	}

private:
	std::vector<value_type> m_Row;
};


///
/// @class Matrix
/// @brief Represent an @c m @c x @c n matrix
///
#if __cplusplus >= __cpp_concepts
	template <
		__detail::IntegralType Type,
		std::size_t Rows, std::size_t Columns
	>
#else
	template<
		class Type, std::size_t Rows, std::size_t Columns,
		typename std::enable_if<std::is_integral<Type>::value, bool>::type = true
	>
#endif
class Matrix {
public:
	using value_type 	  	= Type;
	using row_type			= Row<Type>;
	using pointer		  	= value_type*;
	using const_pointer   	= const value_type*;
	using reference		  	= value_type&;
	using const_reference 	= const value_type&;
	using size_type		  	= std::size_t;

public:
	Matrix():
		m_Rows(Rows),
		m_Columns(Columns)
	{}

	///
	/// @brief Construct a new Matrix with all elements set to the same value
	/// @param value The value to store in the matrix
	///
	explicit Matrix(value_type value):
		Matrix()
	{
		m_MatrixData.fill(value); // Fill the data array with values
		m_Matrix.fill(Row<value_type>(value, m_Columns)); // Fill the rows with values, then fill the matrix with rows
	}

	///
	/// @brief Construct a new Matrix from an initializer list
	/// @param elements An initializer list of values
	///
	Matrix(std::initializer_list<value_type> elements):
		Matrix()
	{
		assert(elements.size() == m_TotalElements);

		// Copy the initializer list into the data array
		std::vector<value_type> data = elements;
		std::copy_n(data.begin(), data.size(), m_MatrixData.begin());

		std::size_t row = 0;
		for (auto iter = elements.begin(); iter != elements.end(); iter += m_Columns, row++) {
			std::vector<value_type> rowData {iter, iter + m_Columns}; // Create a row from the initializer list
			m_Matrix[row] = Row<value_type>(rowData); 				  // Add it to the matrix
		}
	}

	///
	/// @brief Construct a new Row from an initializer list
	/// @param elements An initializer list of values
	///
	Matrix(std::initializer_list<value_type[Columns]> matrix) noexcept(false):
		Matrix()
	{
		assert(matrix.size() == m_Rows); // Make sure there are enough rows

		auto startPoint = m_MatrixData.begin();
		for (std::size_t rowIndex = 0; const value_type (&row)[Columns]: matrix) {
			assert(sizeof(row) / sizeof(value_type) == m_Columns);				// Make sure the row is long enough
			startPoint = std::copy(std::begin(row), std::end(row), startPoint); // Copy the row into the matrix
			m_Matrix[rowIndex++] = Row<value_type>(row, Columns);
		}
	}

	bool hasSameOrder(const Matrix& matrix) const {
		using matrix_value_type = typename std::remove_reference<decltype(matrix)>::type::value_type;
		return (
			m_Rows == matrix.rows() &&							 // Rows match
			m_Columns == matrix.columns() &&					 // Columns match
			(std::is_same<value_type, matrix_value_type>::value) // Underlying types match
		);
	}

	#if __cplusplus >= __cpp_consteval
		consteval size_type size() const {
			return m_TotalElements;
		}
	#else
		constexpr size_type size() const {
			return m_TotalElements;
		}
	#endif

	constexpr size_type rows() const {
		return m_Rows;
	}

	constexpr size_type columns() const {
		return m_Columns;
	}

	void set(size_type row, size_type column, value_type value) {
		m_Matrix[row][column] = value;
		m_MatrixData[row * m_Columns + column] = value;
	}

	value_type at(size_type row, size_type column) const noexcept(false) {
		if (row < 0 || column < 0 || row >= m_Rows || column >= m_Columns) {
			throw std::out_of_range("Invalid index (" + std::to_string(row) + ", " + std::to_string(column) + ")");
		}

		return m_Matrix[row][column];
	}

	const std::array<value_type, Rows * Columns>& data() const {
		return m_MatrixData;
	}

	constexpr bool isSquare() {
		return m_Rows == m_Columns;
	}

	static Matrix zero() {
		return Matrix(0);
	}

	static Matrix identity() noexcept(false) {
		if constexpr (Rows != Columns) {
			throw;
		}

		Matrix<value_type, Rows, Columns> idMatrix(0);

		for (size_type i = 0; i < Rows; i++ /*+= m_Rows + 1*/) {
			idMatrix.set(i, i, 1);
		}

		return idMatrix;
	}

	bool operator==(const Matrix& rhs) const {
		if (m_TotalElements != rhs.size()) {
			return false;
		}
		
		auto rhsData = rhs.data();
		for (int i = 0; i < m_TotalElements; i++) {
			if (m_MatrixData[i] != rhsData[i]) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const Matrix& rhs) const {
		return !(*this == rhs);
	}

	Matrix operator-() const {
		return *this * -1;
	}

	Matrix operator+(const Matrix& rhs) const noexcept(false) {
		assert(hasSameOrder(rhs));

		Matrix<value_type, Rows, Columns> sum(0);
		auto lhsData = this->data(), rhsData = rhs.data();
		
		for (int row = 0, column = 0; row < m_Rows; column = ++column % m_Columns, row += (column == 0)) {
			sum.set(row, column, lhsData[row * m_Columns + column] + rhsData[row * rhs.columns() + column]);
		}

		return sum;
	}

	Matrix operator-(const Matrix& rhs) const noexcept(false) {
		assert(hasSameOrder(rhs));

		Matrix<value_type, Rows, Columns> difference(0);
		auto lhsData = this->data(), rhsData = rhs.data();

		for (int row = 0, column = 0; row < m_Rows; column = ++column % m_Columns, row += (column == 0)) {
			difference.set(row, column, lhsData[row * m_Columns + column] - rhsData[row * rhs.columns() + column]);
		}

		return difference;
	}

	Matrix operator*(value_type scalar) const {
		Matrix<value_type, Rows, Columns> product(0);
		auto lhsData = this->data();
		 
		for (int row = 0, column = 0; row < m_Rows; column = ++column % m_Columns, row += (column == 0)) {
			product.set(row, column, lhsData[row * m_Columns + column] * scalar);
		}

		return product;
	}

	Matrix& operator+=(const Matrix& rhs) {
		return *this = *this + rhs;
	}

	Matrix& operator-=(const Matrix& rhs) {
		return *this = *this - rhs;
	}

	Matrix& operator*=(value_type scalar) {
		return *this = *this * scalar;
	}

	row_type& operator[](size_type index) {
		return m_Matrix[index];
	}

	const row_type& operator[](size_type index) const {
		return m_Matrix[index];
	}

	// std::ostream& operator<<(std::ostream& os) const {
	// 	os << 
	// }

private:
	constexpr static size_type m_TotalElements = Rows * Columns;
	size_type m_Rows, m_Columns;

	std::array<row_type, Rows> m_Matrix;
	std::array<value_type, Rows * Columns> m_MatrixData;
};