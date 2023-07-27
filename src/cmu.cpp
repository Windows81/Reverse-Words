#include "cmu.hpp"
cmutree CMU_ROOT;
cmumap CMU_MAP;

const std::vector<std::vector<std::string>>& word_to_cmus(const std::string& word) {
	return CMU_MAP[word];
}

const std::vector<std::string>& cmus_to_word(const std::vector<std::string>& tokens) {
	return trail_data(&CMU_ROOT, tokens).data;
}

void cmu_add_line(const std::string& line)
{
	// If a line contains a #, then everything after it is removed.
	std::string res = line;
	size_t hash_i = res.find('#');
	if (hash_i != std::string::npos)
		res = res.substr(0, hash_i);
	else if (hash_i == 0)
		return;

	std::vector<std::string> tokens;
	std::stringstream ss(res);
	std::string token;

	// Gets word in a separate variable.
	std::string word;
	getline(ss, word, ' ');

	// Sanitises alternative pronunciations.
	size_t paren_i = word.find('(');
	if (paren_i != std::string::npos)
		word = word.substr(0, paren_i);

	// Stores phonemes in token vector.
	while (getline(ss, token, ' '))
		tokens.push_back(token);

	trail_data(&CMU_ROOT, tokens).data.push_back(word);
	CMU_MAP[word].push_back(tokens);
}

void make_cmu() {
	std::ifstream f_dict("cmudict.dict", std::ifstream::in);

	if (!f_dict.is_open())
		throw std::exception();

	std::string line;
	while (std::getline(f_dict, line))
		cmu_add_line(line);
	f_dict.close();
}