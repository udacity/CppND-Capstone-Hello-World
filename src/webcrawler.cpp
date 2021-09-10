#include <curl/curl.h>
#include <string>
#include <iostream>

#include "url.h"
#include "webcrawler.h"

int webCrawler::make_request(std::unique_ptr<URL> destination) {

	CURL *curl = curl_easy_init();

    if(curl) {
        //const char *data = "data to send";
        const char *data = "robots.txt";

        /* Important: use HTTP2 over HTTPS */
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

        //curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/robots.txt");
        curl_easy_setopt(curl, CURLOPT_URL, destination->get_root_address().c_str());
 
        /* size of the POST data */
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 12L);
 
        /* pass in a pointer to the data - libcurl will not copy */
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
 
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
        
        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }
    return 1;

} 


// CURL *webCrawler::make_handle(char *url) {

//     CURL *handle = curl_easy_init();
 
//     /* Important: use HTTP2 over HTTPS */
//     curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
//     curl_easy_setopt(handle, CURLOPT_URL, url);
 
//     /* buffer body */
//     memory *mem = malloc(sizeof(memory));
//     mem->size = 0;
//     mem->buf = malloc(1);
//     curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, grow_buffer);
//     curl_easy_setopt(handle, CURLOPT_WRITEDATA, mem);
//     curl_easy_setopt(handle, CURLOPT_PRIVATE, mem);

//   /* For completeness */
//     curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
//     curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
//     curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
//     curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);
//     curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 2L);
//     curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
//     curl_easy_setopt(handle, CURLOPT_FILETIME, 1L);
//     curl_easy_setopt(handle, CURLOPT_USERAGENT, "mini crawler");
//     curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
//     curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
//     curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
//     curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
//     return handle;
// }