#include "phon.hpp"
phonmap PHON_MAP;

const phon_hash_typ PHON_MODULO = 189;
const phon_hash_typ PHON_CHAR_MULTS[3] = {
	125, 74, 64,
};

const phon_hash_typ phon_hash_string(const std::string& str) {
	phon_hash_typ res = 0;
	for (size_t c = 0; c < str.length(); ++c)
		res += str[c] * PHON_CHAR_MULTS[c];
	return res % PHON_MODULO;
}

const phon_hash_typ phon_hash(const std::vector<std::string>::const_iterator& beg, const std::vector<std::string>::const_iterator& end) {
	phon_hash_typ res = 0, part;
	for (auto iter = beg; iter != end; ++iter, part = 0)
		res = PHON_MODULO * res + phon_hash_string(*iter);
	return res;
}

inline void phon_add_line(const std::string& line)
{
	phon_str::score_typ score;
	std::vector<std::string> froms;
	std::vector<std::string> tos;
	std::stringstream ss(line);
	std::string word;

	while (getline(ss, word, ' ')) {
		// Attempt to parse the score.  If not parseable, we must be too early.
		try {
			score = std::stoi(word);
			break;
		}
		// Keep catching until we reach an parseable integer.
		catch (std::invalid_argument const& e) {
			froms.push_back(word);
		};
	}

	while (getline(ss, word, ' '))
		tos.push_back(word);

	phon_str p{ .phon = tos, .score = score, };
	phon_hash_typ h = phon_hash(froms.cbegin(), froms.cend());
	PHON_MAP[h].push_back(p);
}

std::vector<phon_str>& phon_get(const std::vector<std::string>::const_iterator& beg, const std::vector<std::string>::const_iterator& end) {
	return PHON_MAP[phon_hash(beg, end)];
}

std::vector<phon_str> phon_merge(const std::vector<phon_str>& phn1, std::vector<phon_str>& phn2) {
	std::vector<phon_str> result;
	for (auto iter1 = phn1.cbegin(); iter1 != phn1.cend(); ++iter1)
		for (auto iter2 = phn2.cbegin(); iter2 != phn2.cend(); ++iter2) {
			phon_str merge{ .score = iter1->score + iter2->score };
			result.push_back(merge);
		}
}

void make_phon()
{
	bool ok = true;
	std::ifstream f_inter("inter.txt", std::ifstream::in);

	if (!f_inter.is_open()) {
		printf("inner.txt file not found.\n");
		ok = false;
	}

	if (!f_inter.is_open())
		throw std::exception();

	std::string line;

	while (std::getline(f_inter, line))
		phon_add_line(line);
	f_inter.close();
}