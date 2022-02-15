#include "webcrawler.h"

#include <curl/curl.h>

#include <iostream>
#include <string>

#include "url.h"

using namespace std;

static size_t
write_data(void *contents, size_t sz, size_t nmemb, void *ctx) {

    // TODO: Remove debug statement
    //std::cout << "In writer callback\n";

    size_t realsize = sz * nmemb;
    memory_t *mem = (memory_t*) ctx;
    char *ptr = (char *)realloc(mem->buf, mem->size + realsize);
    if(!ptr) {
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

    curl = curl_easy_init();
    if (curl) {
        // const char *data = "data to send";
        mem = (memory_t*)malloc(sizeof(memory_t));
        mem->size = 0;
        mem->buf = (char*)malloc(1);
        const char* data = "robots.txt";

        /* Important: use HTTP2 over HTTPS */
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

        // FIXME: Pointer to member function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
        //curl_easy_setopt(curl, CURLOPT_PRIVATE, mem->buf);

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

    curl_easy_setopt(curl, CURLOPT_URL,
                         destination->get_root_address().c_str());

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        cout << "We received " << mem->size << "B of data" << endl;
        curl_easy_cleanup(curl);
        free(mem->buf);
        free(mem);
    } else
        cout << "Request Failed - CURLCode: " << curl_easy_strerror(res) << endl;

    return res;
}

webCrawler::~webCrawler() {
    
    //if (curl)
    curl_global_cleanup();

}
