#ifndef SOAP_NSINFO_H
#define SOAP_NSINFO_H

typedef struct _SoapNsInfo {
    char *evpPrefix;
    char *evpUri;
    char *encPrefix;
    char *encUri;
    char *xsdPrefix;
    char *xsdUri;
    char *xsiPrefix;
    char *xsiUri;
    char *xmdsUri;
    char *xmdsPrefix;
} soapNsInfo;

soapNsInfo soap_nsinfo_default();

#endif /* SOAP_NSINFO_H */

