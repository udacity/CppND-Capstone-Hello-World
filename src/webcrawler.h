#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include "url.h"

class webCrawler {
private:

public:
	 int make_request(std::unique_ptr<URL>);

};


#endif // WEBCRAWLER_H