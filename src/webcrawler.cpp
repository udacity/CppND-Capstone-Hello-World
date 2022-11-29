#include "webcrawler.h"

#include <curl/curl.h>

#include <iostream>
#include <memory>
#include <regex>
#include <set>
#include <string>

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

CURLcode webCrawler::make_request(CURLU *destination_handle) {
  char *url;
  CURLUcode rc = curl_url_get(destination_handle, CURLUPART_URL, &url, 0);
  CURLcode res;

  if (!rc) {
    curl_easy_setopt(curl, CURLOPT_URL, url);

    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
      cout << "We received " << mem->size << "B of data" << endl;

      curl_easy_cleanup(curl);

      // TODO: Find URLs in this buf
      findURLs_in_buf(mem->buf);

      // TODO: write into file

    } else
      cout << "Request Failed - CURLCode: " << curl_easy_strerror(res) << endl;

    curl_free(url);
  }

  return res;
}

// TODO: Find URLs in this buf
int webCrawler::findURLs_in_buf(char *received_buf) {
  int num_url = 0;
  string web_site = string(received_buf);

  return num_url;
}

webCrawler::~webCrawler() {
  if (curl) {
    // Buffer clean-up
    free(mem->buf);
    free(mem);

    curl_global_cleanup();
  }
}
