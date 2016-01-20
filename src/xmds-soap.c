#include "xmds-soap.h"

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
