#pragma once

#include <vector>

using namespace std;

/**
 * @brief find in vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return vector<T>::iterator 
 */
template <typename T>
typename vector<T>::iterator	ft_vecfind(vector<T>& v, T e);

/**
 * @brief remove from vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 */
template <typename T>
void	ft_vecrem(vector<T>& v, T e);

/**
 * @brief true if e is in v
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return true 
 * @return false 
 */
template <typename T>
bool	ft_vecexists(vector<T>& v, T e);

#include "vector.tpp"