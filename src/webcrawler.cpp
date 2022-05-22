#include "webcrawler.h"

#include <curl/curl.h>

#include <iostream>
#include <set>
#include <regex>
#include <string>

#include "url.h"

using namespace std;

static size_t write_data(void *contents, size_t sz, size_t nmemb, void *ctx) {

  // TODO: Remove debug statement
  // std::cout << "In writer callback\n";

  size_t realsize = sz * nmemb;
  memory_t *mem = (memory_t *)ctx;
  char *ptr = (char *)realloc(mem->buf, mem->size + realsize);
  if (!ptr) {
    /* out of memory */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->buf = ptr;
  memcpy(&(mem->buf[mem->size]), contents, realsize);
  mem->size += realsize;
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
    // curl_easy_setopt(curl, CURLOPT_PRIVATE, mem->buf);

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

    // Buffer clean-up
    free(mem->buf);
    free(mem);
  } else
    cout << "Request Failed - CURLCode: " << curl_easy_strerror(res) << endl;

  return res;
}

std::set<std::string> extract_hyperlinks( std::string html_file_name ) {
    static const std::regex hl_regex( "<a href=\"(.*?)\">", std::regex_constants::icase  ) ;

    //const std::string text = file_to_string(html_file_name) ;

    return { std::sregex_token_iterator( html_file_name.begin(), html_file_name.end(), hl_regex, 1 ),
             std::sregex_token_iterator{} } ;
}

// TODO: Find URLs in this buf
std::unique_ptr<URL> webCrawler::findURLs_in_buf(char *received_buf,
                                                 unique_ptr<URL> parent_url) {

  string web_site = string(received_buf);
  const auto sub_urls = [] ( std::string str ) { return str.find( "/" ) == 1 ; }; 

  // Pattern to match
  static const regex url_regex(R"!!(<\s*A\s+[^>]*href\s*=\s*"([^"]*)")!!", std::regex::icase);

  auto new_url = make_unique<URL>("");

  std::copy(
      sregex_token_iterator(web_site.begin(), web_site.end(), url_regex, 1),
      std::sregex_token_iterator(),
      std::ostream_iterator<std::string>(std::cout, "\n"));

   const auto hlinks = extract_hyperlinks(web_site);
  
  //TODO: Here you need to parse URLs and populate a queue??
  new_url->set_root_address(parent_url->cur_address);

  // TODO: Remove debug
  cout << "I am coming from " << new_url->get_root_address() <<
            " - I am: " << new_url->cur_address << endl;

  return new_url;
}

webCrawler::~webCrawler() {

  // if (curl)
  curl_global_cleanup();
}
