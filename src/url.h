#ifndef URL_H
#define URL_H

#include <string>

class URL {
private:
	const std::string _url;
public:
	URL(std::string address) : _url(address) {}
	std::string get_root_address() {return this->_url;}
};

#endif // URL_H