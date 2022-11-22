#include "webcrawler.h"

#include <curl/curl.h>

#include <iostream>
#include <memory>
#include <regex>
#include <set>
#include <string>

// TODO: Will use curl_url as a next step
#include "url.h"

using namespace std;

size_t webCrawler::write_data(void *contents, size_t sz, size_t nmemb,
                              void *ctx) {
  size_t realsize = sz * nmemb;
  memory_t *tmp_mem = (memory_t *)ctx;
  char *ptr = (char *)realloc(tmp_mem->buf, tmp_mem->size + realsize);
  if (!ptr) {
    /* out of memory */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  tmp_mem->buf = ptr;
  memcpy(&(tmp_mem->buf[tmp_mem->size]), contents, realsize);
  tmp_mem->size += realsize;
  return realsize;
}

webCrawler::webCrawler() {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // curl handle
  curl = curl_easy_init();

  if (curl) {
    // Buffer for received data
    mem = (memory_t *)malloc(sizeof(memory_t));
    mem->size = 0;
    mem->buf = (char *)malloc(1);
    const char *data = "robots.txt";

    /* Important: use HTTP2 over HTTPS */
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

    // FIXME: Pointer to member function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                     webCrawler::write_data_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
    curl_easy_setopt(curl, CURLOPT_PRIVATE, mem);

    /* For completeness */
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "mini crawler");
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_easy_setopt(curl, CURLOPT_UNRESTRICTED_AUTH, 1L);
    curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
    curl_easy_setopt(curl, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
  }
}

CURLcode webCrawler::make_request(unique_ptr<URL> destination) {
  curl_easy_setopt(curl, CURLOPT_URL, destination->cur_address.c_str());

  CURLcode res = curl_easy_perform(curl);

  if (res == CURLE_OK) {
    cout << "We received " << mem->size << " B of data" << endl;
    curl_easy_cleanup(curl);

    // TODO: Find URLs in this buf
    findURLs_in_buf(mem->buf, move(destination));

    // TODO: write into file

  } else
    cout << "Request Failed - CURLCode: " << curl_easy_strerror(res) << endl;

  return res;
}

// TODO: Find URLs in this buf
std::unique_ptr<URL> webCrawler::findURLs_in_buf(char *received_buf,
                                                 unique_ptr<URL> parent_url) {
  string web_site = string(received_buf);

  return parent_url;
  // return new_url;
}

webCrawler::~webCrawler() {
  if (curl) {
    // Buffer clean-up
    free(mem->buf);
    free(mem);

    curl_global_cleanup();
  }
}
