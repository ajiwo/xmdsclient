#ifndef XMDS_SOAP_H
#define XMDS_SOAP_H
#include "xml2-xmds-request.h"
#include "xml2-xmds-response.h"

void *send_request(const char*url, xmdsRequestType type, void *param);


#endif /* XMDS_SOAP_H */

