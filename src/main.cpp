#include "wordenc.hpp"
#include "perm.hpp"
#include "phon.hpp"
#include "cmu.hpp"

int main()
{
	make_cmu();
	make_phon();
	make_permspace(encode("abboud base"));
	return 0;
}