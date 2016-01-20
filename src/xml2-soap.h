#ifndef XMDSSOAP
#define XMDSSOAP

#include "xmds-params.h"
#include "soap-nsinfo.h"
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>


typedef struct _soapEnvelope {
    xmlNs *evpNs;
    xmlNs *encNs;
    xmlNs *xsdNs;
    xmlNs *xsiNs;
    xmlNs *xmdsNs;
    xmlNode *node;
} soapEnvelope;


soapEnvelope *soap_envelope_new(soapNsInfo info);
xmlNode *soap_add_funcbody(const soapEnvelope *evp, const char *funcname);
xmlNode *soap_new_string_param(const soapEnvelope *evp, const xmlChar *name, const void *value);
xmlNode *soap_new_base64binary_param(const soapEnvelope *evp, const xmlChar *name, const void *value);
xmlNode *soap_new_int_param(const soapEnvelope *evp, const xmlChar *name, const int value);
xmlNode *soap_new_long_param(const soapEnvelope *evp, const xmlChar *name, const long value);
xmlNode *soap_new_xibodouble_param(const soapEnvelope *evp, const xmlChar *name, const xibodouble value);
xmlNode *soap_new_double_param(const soapEnvelope *evp, const xmlChar *name, const double value);
#endif /* XMDSSOAP */

