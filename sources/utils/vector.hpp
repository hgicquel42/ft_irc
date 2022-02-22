#pragma once

#include <vector>

/**
 * @brief find in vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return std::vector<T>::iterator 
 */
template <typename T>
typename std::vector<T>::iterator	ft_vecfind(std::vector<T>& v, T e);

/**
 * @brief remove from vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 */
template <typename T>
void	ft_vecrem(std::vector<T>& v, T e);

#include "vector.tpp"