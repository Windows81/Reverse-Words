#include "phontree.hpp"
phontree PHON_ROOT;

inline void phon_add_line(const std::string& line)
{
	int score;
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
		// Keey catching until we reach an parseable integer.
		catch (std::invalid_argument const& e) {
			froms.push_back(word);
		};
	}

	while (getline(ss, word, ' '))
		tos.push_back(word);

	phon_str p = {
		.phon = tos,
		.score = score,
	};
	trail_data(&PHON_ROOT, froms).data.push_back(p);
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