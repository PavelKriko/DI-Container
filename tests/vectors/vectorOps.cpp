#include "vectorOps.h"

#include <algorithm>

std::vector<int> operator+(const std::vector<int>& vec1, const std::vector<int>& vec2)
{
    const size_t dim = std::min(vec1.size(), vec2.size());

    std::vector<int> res_vec;
    res_vec.reserve(dim);

    for (size_t i = 0; i < dim; i++)
    {
        res_vec.push_back(vec1[i] + vec2[i]);
    }

    return res_vec;
}

bool operator==(const std::vector<int>& vec1, const std::vector<int>& vec2)
{
    const size_t dim = vec1.size();

    if (dim != vec2.size())
    {
        return false;
    }

    for (size_t i = 0; i < dim; i++)
    {
        if (vec1[i] != vec2[i])
        {
            return false;
        }
    }

    return true;
}

bool vector_each_val_eq(const std::vector<int>& vec, const int val)
{
    return std::all_of(vec.cbegin(), vec.cend(), [&val](const int& el) { return val == el; });
}
