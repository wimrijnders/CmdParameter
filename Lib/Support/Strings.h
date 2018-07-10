#ifndef STRINGS_H_
#define STRINGS_H_


/**
 * @brief Useful string handling methods.
 *
 */
class Strings {
public:

	static bool starts_with(std::string const& haystack, std::string const& needle);
	static std::string implode( const std::vector<std::string>& elements, const char* const separator = ", ");
	static std::vector<std::string> explode(const std::string& str, char sep = ',');
	static std::string trim(const std::string& s, const std::string& delimiters = " \f\n\r\t\v");

#ifndef LITE
	static bool is_number(const std::string& s);

	static std::string toString(int val, const int MAX_WIDTH = 1);
	static std::string to_lower(const std::string &str);

	static bool contains( std::string const& haystack, std::string const& needle);

	static bool ends_with (std::string const &fullString, std::string const &ending);
	static bool ends_with(std::string const &fullString, const char *ending);
	static bool remove_ending(std::string &str, const std::string &ending);
	static bool remove_ending(std::string &str, const char *ending);

	static float to_float(std::istream &is);

	static bool includes(const char *arr[], const char *val);
	static bool includes(const char *arr[], const std::string &val);
#endif  // LITE
};

#endif  // STRINGS_H_
