#include <iostream>
#include <stdio.h>
#include <memory>
#include <curl/curl.h>
#include <string>

#include "webcrawler.h"
#include "url.h"


using namespace std;

int main() {
    webCrawler crawl;
    auto first_hop = make_unique<URL>("https://www.google.com");

    cout << "Hello World!" << "\n";

    cout << first_hop->get_root_address() << "\n";

    crawl.make_request(move(first_hop));
    
    return 0;
}