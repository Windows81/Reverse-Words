#pragma once
#include "tree.hpp"

#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

typedef tree<std::string, std::vector<std::string>> cmutree;
typedef std::map<std::string, std::vector<std::vector<std::string>>> cmumap;

const std::vector<std::vector<std::string>>& word_to_cmus(const std::string&);
const std::vector<std::string>& cmus_to_word(const std::vector<std::string>&);
void cmu_add_line(const std::string&);
void make_cmu();