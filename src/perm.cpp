#include "perm.hpp"
extern cmutree CMU_TREE;
extern phontree PHON_ROOT;

/*
typedef long skfms;
const skfms MODULO = 189;
const skfms CHAR_MULTIPLIERS[3] = {
	125, 74, 64,
};

const skfms perm_hash(const std::string& str) {
	skfms res = 0;
	for (size_t c = 0; c < str.length(); ++c)
		res += str[c] * CHAR_MULTIPLIERS[c];
	return res % MODULO;
}
*/

struct permspace {
	typedef long skfms;
	struct perm {
		std::vector<std::vector<skfms>> vec2D;
	};
	std::vector<perm> cmus;
	std::map<skfms, void*> cache;
};

const skfms perm_hash(const std::vector<std::string>::const_iterator& beg, std::vector<std::string>::const_iterator& end) {
	skfms res = 0, part;
	for (auto iter = beg; iter != end; ++iter, part = 0)
		res = MODULO * res + perm_hash(*iter);
	return res;
}

void perm_add(permspace& space, const std::vector<std::string>& tokens) {
	size_t length = tokens.size();
	permspace::perm p{
		.vec2D = std::vector<std::vector<skfms>>(length, std::vector<skfms>(length, 0)),
	};
	space.cmus.push_back(p);
	size_t b = 0;
	for (auto iter = tokens.cbegin(); iter != tokens.cend(); ++iter, ++b)
		p.vec2D[b][b] = perm_hash(*iter);

	for (size_t s = 1; s < length; ++s)
		for (size_t b = 0; b < length - s; ++b)
			p.vec2D[b][b + s] = MODULO * p.vec2D[b][b + s - 1] + p.vec2D[b + s][b + s];
}

/*
permtree get_perms(std::vector<std::string> e) {
	std::vector<phontree::node*> nodes;
	std::list<std::vector<phontree*>> gaps;
	std::map < phontree*, permtree*> trans;
	auto beg = e.begin();
	auto end = e.end();

	phontree* part = new phontree();
	phontree::node* data_n;

	for (auto i = beg; i != end; ++i) {
		gaps.push_back({ part, &PHON_ROOT });
		for (auto t_i = gaps.begin(); t_i != gaps.end(); ++t_i) {
			auto ref = &(*t_i)[1];
			if (!*ref) continue;
			data_n = &(*ref)->map[*i];
			*ref = data_n->next;
			if (*ref)
				continue;

			for (auto&& n : data_n->data);
			gaps.erase(t_i++);
		}
	}
}
*/

permspace& make_permspace(std::vector<std::vector<std::string>> cmus) {
	permspace space;

}

permtree get_perms(std::vector<std::string> e) {
}