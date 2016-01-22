#include "xmds-soap.h"

/*
TODO: consider curl callback for XMDS_GET_FILE request, extract base64 encoded
      string inside the callback function, rather than waiting the callback
      to finish then extract big (and accumulated) response content.
      this will allow bigger XMDS_MAX_CHUNK value while keeping memory usage low
      but with slighly higher computing usage.
*/


void *send_request(const char*url, xmdsRequestType type, void *param) {
    char *req_string;
    transportResponse resp;

    memset(&resp, '\0', sizeof(transportResponse));

    req_string = build_request(type, param);
    /*
    printf("%s\n", req_string);
    */
    transport_send_request(&resp, url, req_string, tsr_default_cb);
    free(req_string);

    if(resp.size > 0) {
        /*
        printf("%s", resp.memory);
        */
    } else {
        free(resp.memory);
        return NULL;
    }
    return parse_response(resp, type);
}
