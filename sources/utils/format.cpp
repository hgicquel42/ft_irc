#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief format string from vector
 * 
 * @param format 
 * @param values 
 * @return string 
 */
string	ft_format(const string& format, const vector<string>& values)
{
	stringstream	stream;
	size_t			i;
	size_t			j;
	size_t			k;

	i = 0;
	k = 0;
	while (i < format.size())
	{
		j = 0;
		while (format[i + j] && format[i + j] != '%')
			j++;
		if (j)
			stream << format.substr(i, j);
		i += j;
		if (i++ >= format.size())
			break ;
		if (format[i++] == 's')
			stream << values[k++];
	}
	return (stream.str());
}