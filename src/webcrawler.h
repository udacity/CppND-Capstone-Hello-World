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
  CURLcode make_request(CURLU *destination);

  /* Utility function to parse urls */
  int findURLs_in_buf(char *received_buf);

  size_t write_data(void *contents, size_t sz, size_t nmemb, void *ctx);

  static void write_data_callback(void *ctx, size_t sz, size_t nmemb,
                                  void *context) {
    // TODO: Verify that the object / ctx is correct conceptually.
    auto *object = (webCrawler *)ctx;
    object->write_data(ctx, sz, nmemb, context);
  }

  // Utility function to check how many bytes are in the buffer
  int buf_size() { return mem->size; }
};

#endif  // WEBCRAWLER_H
