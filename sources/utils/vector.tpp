#include "vector.hpp"

#include <algorithm>

/**
 * @brief find in vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return std::vector<T>::iterator 
 */
template <typename T>
typename std::vector<T>::iterator	ft_vecfind(std::vector<T>& v, T e)
{
	return (std::find(v.begin(), v.end(), e));
}

/**
 * @brief remove from vector
 * 
 * @tparam T 
 * @param v 
 * @param e 
 */
template <typename T>
void	ft_vecrem(std::vector<T>& v, T e)
{
	v.erase(ft_vecfind(v, e));
}