#include "set.hpp"

#include <algorithm>

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