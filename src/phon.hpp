#pragma once
#include "tree.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>


struct phon_str {
	typedef char score_typ;
	std::vector<std::string> phon;
	score_typ score;
};

typedef long long phon_hash_typ;
typedef std::map<phon_hash_typ, std::vector<phon_str>> phonmap;
std::vector<phon_str>& phon_get(const std::vector<std::string>::const_iterator&, const std::vector<std::string>::const_iterator&);
void make_phon();