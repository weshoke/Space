#include "brigand/sequences/list.hpp"
#include "brigand/sequences/back.hpp"
#include "brigand/sequences/front.hpp"



int main(int argc, const char * argv[]) {
	
	using my_list = brigand::list<bool, int, char *>;

	// head is 'bool'
	using head = brigand::front<my_list>;

	// last is 'char *'
	using last = brigand::back<my_list>;
    return 0;
}
