/* taken from http://curl.haxx.se/libcurl/c/getinmemory.html */
#ifndef SOAP_TRANSPORT_CURL_H
#define SOAP_TRANSPORT_CURL_H
#include <curl/curl.h>

typedef struct _transportResponse {
  char *memory;
  size_t size;
} transportResponse;

typedef size_t (*tsr_cb)(void *, size_t, size_t, void *);

size_t tsr_default_cb(void *contents, size_t size, size_t nmemb, void *userp);
int transport_send_request(transportResponse *resp, const char *url, const void *payload, tsr_cb cb);
#endif /* SOAP_TRANSPORT_CURL_H */

