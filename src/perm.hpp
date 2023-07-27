#pragma once
#include "cmu.hpp"
#include "phon.hpp"

#include <vector>
#include <string>

struct permspace {
	struct perm {
		std::vector<std::vector<std::vector<phon_str>>> matrix;
	};
	std::vector<perm> cmus;
};

permspace& make_permspace(const std::vector<std::vector<std::string>>&);