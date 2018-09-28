#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <INIReader.h>

/*

example:

id = 0
x = 10.0
y = 15.0

*/

class config
{
public:
	config(const std::string& path);

	std::set<std::string>::const_iterator begin() const;
	std::set<std::string>::const_iterator end() const;
	
	std::string at(
		const std::string& section,
		const std::string& name,
		const char* default_value) const;

	std::string at(
		const std::string& section,
		const std::string& name,
		const std::string& default_value) const;

	bool at(
		const std::string& section,
		const std::string& name,
		bool default_value) const;

	int at(
		const std::string& section,
		const std::string& name,
		int default_value) const;

	float at(
		const std::string& section,
		const std::string& name,
		float default_value) const;

private:
	INIReader reader;
};

#endif
