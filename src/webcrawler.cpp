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

        //curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/robots.txt");
        curl_easy_setopt(curl, CURLOPT_URL, destination->get_root_address().c_str());
 
        /* size of the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 12L);
 
        /* pass in a pointer to the data - libcurl will not copy */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
 
        curl_easy_perform(curl);
    }
    return 1;

} 