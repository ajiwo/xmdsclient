#ifndef XML2_XMDS_RESPONSE_H
#define XML2_XMDS_RESPONSE_H
#include "xml2-xmds.h"
#include "soap-transport-curl.h"


void *build_response(xmlNode *node, xmdsRequestType type);
void *parse_response(transportResponse resp, xmdsRequestType type);

#endif /* XML2_XMDS_RESPONSE_H */
