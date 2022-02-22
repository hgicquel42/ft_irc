#include "set.hpp"

#include <algorithm>

/**
 * @brief find in set
 * 
 * @tparam T 
 * @param v 
 * @param e 
 * @return std::set<T>::iterator 
 */
template <typename T>
typename std::set<T>::iterator	ft_setfind(std::set<T>& v, T e)
{
	return (std::find(v.begin(), v.end(), e));
}

/**
 * @brief remove from set
 * 
 * @tparam T 
 * @param v 
 * @param e 
 */
template <typename T>
void	ft_setrem(std::set<T>& v, T e)
{
	v.erase(ft_setfind(v, e));
}

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
bool	ft_setexists(std::set<T> s, T x)
{
	return (s.find(x) == s.end());
}