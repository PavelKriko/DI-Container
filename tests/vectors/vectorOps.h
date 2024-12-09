#pragma once

#include <vector>

std::vector<int> operator+(const std::vector<int>& vec1, const std::vector<int>& vec2);

bool operator==(const std::vector<int>& vec1, const std::vector<int>& vec2);

bool vector_each_val_eq(const std::vector<int>& vec, int val);
