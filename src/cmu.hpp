#pragma once
#include "cmutree.hpp"

const std::vector<std::vector<std::string>>& word_to_cmus(const std::string&);
const std::vector<std::string>& cmus_to_word(const std::vector<std::string>&);
void cmu_add_line(const std::string&);
void make_cmu();