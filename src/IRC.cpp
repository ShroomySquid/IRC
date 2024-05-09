#include "IRC.hpp"

using std::cout;
using std::endl;

IRC::IRC(void) {}

IRC::IRC(const IRC& old) {(void)old;}

IRC::~IRC(void) {}

IRC & IRC::operator=(IRC const & rhs) {
	(void)rhs;
	return (*this);
}
