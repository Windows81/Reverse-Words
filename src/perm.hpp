#pragma once
#include "cmu.hpp"
#include "phon.hpp"

#include <vector>
#include <string>

typedef tree<std::string, std::vector<std::string>> permtree;
permtree get_perms(std::vector<std::string>);