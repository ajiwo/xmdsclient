/* taken from http://curl.haxx.se/libcurl/c/getinmemory.html */
#include "soap-transport-curl.h"
#include <stdlib.h>
#include <string.h>


size_t tsr_default_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    transportResponse *mem = (transportResponse *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


int transport_send_request(transportResponse *resp, const char *url, const void *payload, tsr_cb cb) {
    CURL *curl_handle;
    CURLcode res;

    resp->memory = malloc(1);  /* will be grown as needed by the realloc above */
    resp->size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* zir */
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    /*
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    */
    /* zir */

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)resp);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "xmds-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {
      /*
       * Now, our chunk.memory points to a memory block that is chunk.size
       * bytes big and contains the remote file.
       *
       * Do something nice with it!
       */
      /*
      printf("%lu bytes retrieved\n", (long)resp->size);
      */

      /* zir */
      /*printf("%s\n", resp.memory);*/
      /* zir */
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    /*
    free(resp.memory);
    */
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return 0;

}
