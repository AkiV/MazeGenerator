#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

template <class T>
// A structure for holding a vector of vectors (2D array / a grid).
struct Matrix
{	
	std::vector<std::vector<T>> cells;

	Matrix() { };

	Matrix(int height, int width)
	{
		for (int x = 0; x < width; x++)
		{
			std::vector<T> line = std::vector<T>();
			line.resize(height);
			cells.push_back(line);
		}
	}

	int GetWidth() const { return cells.size(); }
	int GetHeight() const { return cells[0].size(); }
	
	const std::vector<T>& operator[](int index) const
	{
		return cells[index];
	}

	std::vector<T>& operator[](int index)
	{
		return cells[index];
	}
};

#endif