#include "soap-nsinfo.h"

soapNsInfo soap_nsinfo_default() {
    soapNsInfo info;

    info.evpPrefix = "SOAP-ENV";
    info.evpUri = "http://schemas.xmlsoap.org/soap/envelope/";

    info.encPrefix = "SOAP-ENC";
    info.encUri = "http://schemas.xmlsoap.org/soap/encoding/";

    info.xsdPrefix = "xsd";
    info.xsdUri = "http://www.w3.org/2001/XMLSchema";

    info.xsiPrefix = "xsi";
    info.xsiUri = "http://www.w3.org/2001/XMLSchema-instance";

    info.xmdsUri = "urn:xmds";
    info.xmdsPrefix = "ns1";

    return info;
}
