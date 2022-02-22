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
  CURL *curl;

  // TODO: Can I this as a shared_ptr?
  memory_t *mem;

public:
  webCrawler();
  ~webCrawler();
  
  // TODO: High level function that makes the request and fetch the web-site
  CURLcode make_request(std::unique_ptr<URL> destination);

  /* Utility function to parse urls */
  std::unique_ptr<URL>
  findURLs_in_buf(char *received_buf, std::unique_ptr<URL> parent_url);

  // size_t write_data(void* ptr, size_t size, size_t nmemb, std::string* data);

  // int make_request(std::unique_ptr<URL>);
};

#endif // WEBCRAWLER_H