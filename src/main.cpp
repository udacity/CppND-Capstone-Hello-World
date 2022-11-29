#include <curl/curl.h>
#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>

#include "webcrawler.h"

using namespace std;

int main() {
  webCrawler crawl;
  CURLU *first_url_handle = curl_url();
  CURLUcode rc;

  rc = curl_url_set(first_url_handle, CURLUPART_URL, "https://www.reuters.com",
                    0);

  if (!rc) crawl.make_request(first_url_handle);

  // TODO: Clean the debug prints
  cout << "Crawl object has " << crawl.buf_size() << "B buffered data"
       << "\n";

  return 0;
}