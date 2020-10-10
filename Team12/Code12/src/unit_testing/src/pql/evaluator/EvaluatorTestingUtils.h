/**
 * Helper methods for unit testing of evaluator.
 */
#ifndef UNIT_TESTING_EVALUATOR_TESTING_UTILS_H
#define UNIT_TESTING_EVALUATOR_TESTING_UTILS_H

#include <algorithm>
#include <vector>

#include "catch.hpp"

/**
 * A method to compare two vectors, to see whether
 * they have the same elements regardless of order.
 *
 * @tparam T Type of elements in the vector. Elements
 *           must implement the method
 *           bool cmp(const T&, const T&).
 */
template <typename T>
void requireVectorsHaveSameElements(std::vector<T> vector1, std::vector<T> vector2)
{
    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());
    REQUIRE(vector1 == vector2);
}

/*
#include <iostream>

template <>
void doVectorsHaveSameElementsVoid<std::string>(std::vector<std::string> vector1, std::vector<std::string> vector2)
{
    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());
    for (std::string& item : vector1) {
        std::cout << item << std::endl;
    }
    for (std::string& item : vector2) {
        std::cout << item << std::endl;
    }
    REQUIRE(vector1 == vector2);
}

template <>
void requireVectorsHaveSameElements<std::pair<std::string, std::string>>(
    std::vector<std::pair<std::string, std::string>> vector1, std::vector<std::pair<std::string, std::string>> vector2)
{
    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());
    for (std::pair<std::string, std::string>& item : vector1) {
        std::cout << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
    for (std::pair<std::string, std::string>& item : vector2) {
        std::cout << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
    REQUIRE(vector1 == vector2);
}
*/

#endif // UNIT_TESTING_EVALUATOR_TESTING_UTILS_H
