#include "config.hpp"

using std::string;

config::config(const string& path) 
	: reader{ path } 
{

}

std::set<string>::const_iterator config::begin() const
{
	return reader.Sections().begin();
}
std::set<string>::const_iterator config::end() const
{
	return reader.Sections().end();
}

string config::at(
	const string& section,
	const string& name,
	const char* default_value) const
{
	return reader.Get(section, name, default_value);

}

string config::at(
	const string& section,
	const string& name,
	const string& default_value) const
{
	return reader.Get(section, name, default_value);
}

bool config::at(
	const string& section,
	const string& name,
	bool default_value) const
{
	return reader.GetBoolean(section, name, default_value);
}

int config::at(
	const string& section,
	const string& name,
	int default_value) const
{
	return static_cast<int>(
		reader.GetInteger(section, name, default_value));
}

float config::at(
	const string& section,
	const string& name,
	float default_value) const
{
	return static_cast<float>(
		reader.GetReal(section, name, default_value));
}