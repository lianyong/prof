#pragma once

using namespace std;

template<typename CharT>
typename std::basic_string<CharT>
::size_type split(std::vector<std::basic_string<CharT> >& vec, const std::basic_string<CharT>& str, const std::basic_string<CharT>& sep)
{
	typedef std::basic_string<CharT> string_type;
	if (!str.empty())
	{
		typename string_type::size_type pos_begin = 0;
		typename string_type::size_type comma_pos = str.find_first_of(sep, pos_begin);
		while (comma_pos != string_type::npos)
		{
			if (pos_begin < comma_pos)
			{
				vec.push_back(str.substr(pos_begin, comma_pos - pos_begin));
			}

			pos_begin = comma_pos + 1;
			comma_pos = str.find_first_of(sep, pos_begin);
		}

		if (pos_begin < str.length())
		{// Ê£Óà×Ö·û´®
			vec.push_back(str.substr(pos_begin));
		}
	}

	return !vec.empty();
}

template<typename T>
std::string toString(const T& value)
{
	std::ostringstream ostr;
	ostr << value;
	return ostr.str();
}

template<typename T>
T fromString(const std::string& str)
{
	std::istringstream istr(str);
	T value;
	istr >> value;
	return value;
}

template<class T1>
void string_format(boost::format& fmt, T1&& first)
{
	fmt % first;
}

template<class T1, class... T2>
void string_format(boost::format& fmt, T1&& first, T2&&... other)
{
	fmt % first;
	string_format(fmt, other...);
}

template<class T1, class... T2>
string string_format(const char* format, T1&& first, T2&&... other)
{
	boost::format fmt(format);
	string_format(fmt, first, other...);
	return fmt.str();
}