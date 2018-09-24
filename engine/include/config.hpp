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
	config(const std::string& path) : reader{path} {}

	std::set<std::string>::const_iterator begin() const
	{ 
		return reader.Sections().begin(); 
	}
	std::set<std::string>::const_iterator end() const
	{ 
		return reader.Sections().end(); 
	}

	std::string at(
		const std::string& section,
		const std::string& name,
		const char* default_value) const
	{
		return reader.Get(section, name, default_value);
		
	}

	std::string at(
		const std::string& section,
		const std::string& name, 
		const std::string& default_value) const
	{
		return reader.Get(section, name, default_value);
	}

	bool at(
		const std::string& section, 
		const std::string& name, 
		bool default_value) const
	{
		return reader.GetBoolean(section, name, default_value);
	}

	int at(
		const std::string& section, 
		const std::string& name, 
		int default_value) const
	{
		return static_cast<int>(
			reader.GetInteger(section, name, default_value));
	}

	float at(
		const std::string& section, 
		const std::string& name, 
		float default_value) const
	{
		return static_cast<float>(
			reader.GetReal(section, name, default_value));
	}

private:
	INIReader reader;
};

#endif
