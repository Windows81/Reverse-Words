#include "wordenc.hpp"
extern cmumap CMU_MAP;

std::vector<std::vector<std::string>> encode(std::string words) {
	std::vector<std::vector<std::string>> result;
	result.push_back(std::vector<std::string>());

	std::stringstream ss(words);
	std::string word;

	// Gets next word, separated by space.
	while (getline(ss, word, ' ')) {

		// Loads CMU pronunciation guide for word.
		std::vector<std::vector<std::string>> cmus = word_to_cmus(word);
		auto old_res_size = result.size();
		auto cmu_size = cmus.size();

		// Makes (n-1) copies of the previous results if more than one CMU guide exists.
		if (cmu_size > 1) {
			std::vector<std::vector<std::string>> copied_res(result);
			auto cpr_beg = copied_res.begin();
			auto cpr_end = copied_res.end();

			for (size_t c = 1; c < cmu_size; ++c)
				result.insert(result.end(), cpr_beg, cpr_end);
		}

		size_t res_index = 0;
		auto cmu_iter = cmus.begin();
		auto res_iter = result.begin();
		auto res_end = result.end();
		while (res_iter != res_end) {
			res_iter->insert(res_iter->end(), cmu_iter->begin(), cmu_iter->end());
			if (++res_index % old_res_size == 0) ++cmu_iter;
			++res_iter;
		}
	}
	return result;
}