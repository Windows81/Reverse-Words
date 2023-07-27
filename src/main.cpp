#include "wordenc.hpp"
#include "perm.hpp"
#include "phon.hpp"
#include "cmu.hpp"

int main()
{
	make_cmu();
	make_phon();
	get_perms(encode("abboud base")[0]);
	return 0;
}