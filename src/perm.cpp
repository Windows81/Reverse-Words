#include "perm.hpp"
extern cmutree CMU_TREE;

void perm_add(permspace& space, const std::vector<std::string>& tokens) {
	size_t length = tokens.size();
	permspace::perm p{
		.matrix = std::vector<std::vector<std::vector<phon_str>>>(
			length, std::vector<std::vector<phon_str>>(
				length, std::vector<phon_str>()
			)
		),
	};
	space.cmus.push_back(p);
	size_t b = 0;
	for (auto iter = tokens.cbegin(); iter != tokens.cend(); ++iter, ++b)
		p.matrix[b][b] = phon_get(iter, iter + 1);

	/*
	for (size_t s = 1; s < length; ++s)
		for (size_t b = 0; b < length - s; ++b)
			p.matrix[b][b + s] = MODULO * p.matrix[b][b + s - 1] + p.matrix[b + s][b + s];
	*/
}

permspace& make_permspace(const std::vector<std::vector<std::string>>& cmus) {
	permspace space;
	for (auto iter = cmus.cbegin(); iter != cmus.cend(); ++iter)
		perm_add(space, *iter);
	return space;
}