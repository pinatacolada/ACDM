#pragma once
#include <string>
#include <regex>

using namespace std;
// Plugin info
const string PLUGIN_NAME = "A-CDM";
const string PLUGIN_VERSION = "v0.0";
const string PLUGIN_AUTHOR = "Ricardo Sousa";
const string PLUGIN_COPYRIGHT = "GPL-3.0 License";
const int TAG_ITEM_ACDM_DUMMY = 455;

const std::regex REGEX_DOF = std::regex("DOF\\/\\d{6}", std::regex_constants::ECMAScript);