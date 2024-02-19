#include <cstdlib>

#include "matrix.hpp"

constexpr static int ITERATIONS = 100000;

template<typename T, std::size_t Rows, std::size_t Columns>
float lookup(const Matrix<T, Rows, Columns>& matrix) {
	auto _matrix = matrix;
	int sum = 0;
	for (int _ = 0; _ < ITERATIONS; _++) {
		srand(static_cast<unsigned int>(time(0)));
		int row = rand() % 3, column = rand() % 3;

		auto start = std::chrono::high_resolution_clock::now();
			_matrix[row][column] = 5; // Perform a useless lookup
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
		sum += duration.count();
	}

	return sum / static_cast<float>(ITERATIONS);
}

template<typename T, std::size_t Rows, std::size_t Columns>
float fullLookup(const Matrix<T, Rows, Columns>& matrix) {
	int sum = 0;
	for (int _ = 0; _ < ITERATIONS; _++) {
		auto start = std::chrono::high_resolution_clock::now();
			for (std::size_t i = 0; i < matrix.rows(); i++) {
				for (std::size_t j = 0; j < matrix.columns(); j++) {
					matrix[i][j]; // Perform a useless lookup
				}
			}
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
		sum += duration.count();
	}

	return sum / static_cast<float>(ITERATIONS);
}