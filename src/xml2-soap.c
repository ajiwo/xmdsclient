#include <limits.h>
#include "xml2-soap.h"

soapEnvelope *soap_envelope_new(soapNsInfo info) {
    soapEnvelope *evp;
    xmlNode *body_node;


    evp = malloc(sizeof(soapEnvelope));

    evp->node = xmlNewNode(NULL, BAD_CAST "Envelope");
    evp->evpNs = xmlNewNs(evp->node, BAD_CAST info.evpUri, BAD_CAST info.evpPrefix);
    evp->encNs = xmlNewNs(evp->node, BAD_CAST info.encUri, BAD_CAST info.encPrefix);
    evp->xsdNs = xmlNewNs(evp->node, BAD_CAST info.xsdUri, BAD_CAST info.xsdPrefix);
    evp->xsiNs = xmlNewNs(evp->node, BAD_CAST info.xsiUri, BAD_CAST info.xsiPrefix);
    evp->xmdsNs = xmlNewNs(evp->node, BAD_CAST info.xmdsUri, BAD_CAST info.xmdsPrefix);

    xmlSetNs(evp->node, evp->evpNs);

    xmlSetNsProp(evp->node, evp->evpNs, BAD_CAST "encodingStyle", BAD_CAST info.encUri);
    body_node = xmlNewNode(evp->evpNs, BAD_CAST "Body");
    xmlAddChild(evp->node, body_node);
    return evp;
}

xmlNode *soap_add_funcbody(const soapEnvelope *evp, const char *funcname) {
    xmlNode *body_node;
    xmlNode *func_node;

    body_node = xmlLastElementChild(evp->node);
    func_node = xmlNewNode(evp->xmdsNs, BAD_CAST funcname);
    return xmlAddChild(body_node, func_node);
}

xmlNode *soap_new_param0(const soapEnvelope *evp, const xmlChar *name, const xmlChar *type, const xmlChar *value) {
    xmlChar *tmp1, *tmp2;
    xmlNode *node;

    node = xmlNewNode(NULL, name);

    tmp1 = xmlStrcat(xmlStrdup(BAD_CAST ":"), type);
    tmp2 = xmlStrcat(xmlStrdup(evp->xsdNs->prefix), tmp1);

    xmlNewNsProp(node, evp->xsiNs, BAD_CAST "type", tmp2);
    xmlNodeAddContent(node, value);
    xmlFree(tmp2);
    xmlFree(tmp1);
    return node;
}

xmlNode *soap_new_string_param(const soapEnvelope *evp, const xmlChar *name, const void *value) {
    return soap_new_param0(evp, name, BAD_CAST "string", value);
}

xmlNode *soap_new_base64binary_param(const soapEnvelope *evp, const xmlChar *name, const void *value) {
    return soap_new_param0(evp, name, BAD_CAST "base64Binary", value);
}

xmlNode *soap_new_int_param(const soapEnvelope *evp, const xmlChar *name, const int value) {
    char *cvalue;
    xmlChar *xvalue;
    xmlNode *node;
    unsigned short cvalue_len;

    cvalue_len = digitlen(value);
    cvalue = malloc(cvalue_len + 1);
    sprintf(cvalue, "%d", value);
    xvalue = xmlStrdup(BAD_CAST cvalue);
    free(cvalue);

    node = soap_new_param0(evp, name, BAD_CAST "int", xvalue);
    xmlFree(xvalue);

    return node;
}

/*
xmlNode *soap_new_long_param(const soapEnvelope *evp, const xmlChar *name, const long value) {
    char cvalue[12];
    xmlChar *xvalue;
    xmlNode *node;
    sprintf(cvalue, "%ld", value);
    xvalue = xmlStrdup(BAD_CAST cvalue);

    node = soap_new_param0(evp, name, BAD_CAST "long", xvalue);
    xmlFree(xvalue);

    return node;
}
*/

xmlNode *soap_new_xibodouble_param(const soapEnvelope *evp, const xmlChar *name, const xibodouble value) {
    char *cvalue;
    xmlChar *xvalue;
    xmlNode *node;
    unsigned short cvalue_len;

    cvalue_len = digitlen(value);
    cvalue = malloc(cvalue_len + 1);
    sprintf(cvalue, "%lu", value);
    xvalue = xmlStrdup(BAD_CAST cvalue);
    free(cvalue);

    node = soap_new_param0(evp, name, BAD_CAST "double", xvalue);
    xmlFree(xvalue);

    return node;
}

/*
xmlNode *soap_new_double_param(const soapEnvelope *evp, const xmlChar *name, const double value) {
    char cvalue[16];
    xmlChar *xvalue;
    xmlNode *node;

    sprintf(cvalue, "%f", value);
    xvalue = xmlStrdup(BAD_CAST (const char*)cvalue);

    node = soap_new_param0(evp, name, BAD_CAST "double", xvalue);

    xmlFree(xvalue);


    return node;
}
*/
