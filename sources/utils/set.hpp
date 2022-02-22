#pragma once

/**
 * @brief find in set
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return std::set<T>::iterator 
 */
template <typename T>
typename std::set<T>::iterator	ft_setfind(std::set<T>& v, T e);

/**
 * @brief remove from set
 * 
 * @tparam T 
 * @param v 
 * @param e 
 */
template <typename T>
void	ft_setrem(std::set<T>& v, T e);

/**
 * @brief true if x exists in set
 * 
 * @tparam T 
 * @param s 
 * @param x 
 * @return true 
 * @return false 
 */
template <typename T>
bool	ft_setexists(std::set<T> s, T x);

#include "set.tpp"