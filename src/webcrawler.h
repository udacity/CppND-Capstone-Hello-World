#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <curl/curl.h>

#include <memory>
#include <string>

#include "url.h"

typedef struct {
  char *buf;
  size_t size;
} memory_t;

class webCrawler {
 private:
  CURL *curl;

  // TODO: Can I have this as a shared_ptr?
  memory_t *mem;

 public:
  webCrawler();
  ~webCrawler();

  // TODO: High level function that makes the request and fetch the web-site
  CURLcode make_request(std::unique_ptr<URL> destination);

  /* Utility function to parse urls */
  std::unique_ptr<URL> findURLs_in_buf(char *received_buf,
                                       std::unique_ptr<URL> parent_url);

  size_t write_data(void *contents, size_t sz, size_t nmemb, void *ctx);

  static void write_data_callback(void *ctx, size_t sz, size_t nmemb,
                                  void *context) {
    auto *object = (webCrawler *)ctx;
    object->write_data(ctx, sz, nmemb, context);
  }

  int buf_size() {
    return mem->size;
  }  // Utility function to check
     // how many bytes are in the buffer
};

#endif  // WEBCRAWLER_H
