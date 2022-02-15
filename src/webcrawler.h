#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <curl/curl.h>

#include <string>

#include "url.h"

typedef struct {
  char *buf;
  size_t size;
} memory_t;

class webCrawler {
 private:
  CURL* curl;
  memory_t *mem;

 public:
  webCrawler();
  ~webCrawler();
  CURLcode make_request(std::unique_ptr<URL> destination);
  //size_t write_data(void* ptr, size_t size, size_t nmemb, std::string* data);

  // int make_request(std::unique_ptr<URL>);
};

#endif  // WEBCRAWLER_H