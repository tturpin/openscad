#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "feature.h"
#include "printutils.h"

/**
 * Feature registration map/list for later lookup. This must be initialized
 * before the static feature instances as those register with this map. 
 */
Feature::map_t Feature::feature_map;
Feature::list_t Feature::feature_list;

/*
 * List of features, the names given here are used in both command line
 * argument to enable the option and for saving the option value in GUI
 * context.
 */
const Feature Feature::ExperimentalInputDriverDBus("input-driver-dbus", "Enable DBus input drivers (requires restart)");
const Feature Feature::ExperimentalFunctionLiterals("function-literals", "Enable support for function literals");
const Feature Feature::ExperimentalLazyUnion("lazy-union", "Enable lazy unions.");

Feature::Feature(const std::string &name, const std::string &description)
	: enabled(false), name(name), description(description)
{
	feature_map[name] = this;
	feature_list.push_back(this);
}

Feature::~Feature()
{
}

const std::string &Feature::get_name() const
{
	return name;
}
    
const std::string &Feature::get_description() const
{
	return description;
}
    
bool Feature::is_enabled() const
{
	return enabled;
}

void Feature::enable(bool status)
{
	enabled = status;
}
    
void Feature::enable_feature(const std::string &feature_name, bool status)
{
	auto it = feature_map.find(feature_name);
	if (it != feature_map.end()) {
		it->second->enable(status);
	} else {
		PRINTB("WARNING: Ignoring request to enable unknown feature '%s'.", feature_name);
	}
}

Feature::iterator Feature::begin()
{	
	return feature_list.begin();
}

Feature::iterator Feature::end()
{
	return feature_list.end();
}

void Feature::dump_features()
{
	for (map_t::iterator it = feature_map.begin(); it != feature_map.end(); it++) {
		std::cout << "Feature('" << it->first << "') = " << (it->second->is_enabled() ? "enabled" : "disabled") << std::endl;
	}
}

ExperimentalFeatureException::ExperimentalFeatureException(const std::string &what_arg)
    : EvaluationException(what_arg)
{

}

ExperimentalFeatureException::~ExperimentalFeatureException() throw()
{

}

void ExperimentalFeatureException::check(const Feature &feature)
{
	if (!feature.is_enabled()) {
		throw ExperimentalFeatureException(STR("ERROR: Experimental feature not enabled: '" << feature.get_name() << "'. Please check preferences."));
	}
}
