#include <curl/curl.h>
#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>

#include "url.h"
#include "webcrawler.h"

using namespace std;

int main() {
  webCrawler crawl;

  auto first_hop = make_unique<URL>("https://www.reuters.com");

  // FIXME: This needs to be fixed
  cout << first_hop->cur_address << "\n";
  crawl.make_request(move(first_hop));

  return 0;
}