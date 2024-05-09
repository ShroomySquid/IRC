#ifndef IRC_HPP
# define IRC_HPP
# include <string>
# include <iostream>

using std::cout;
using std::endl;
using std::cerr;

class IRC {
	public:
		IRC(void);
		IRC(const IRC& old);
		~IRC(void);
		IRC & operator=(IRC const & rhs);
};

#endif
