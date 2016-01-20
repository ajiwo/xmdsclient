#include "xml2-xmds-request.h"
#include "xmds-util.h"

char *build_request(xmdsRequestType type, void *param) {
    soapNsInfo ns;
    soapEnvelope *evp;
    void (*cleanup_func)(void *);

    xmlDoc *doc;
    xmlChar *req_xstring;
    char *req_cstring;
    int reqlen;

    ns = soap_nsinfo_default();
    evp = soap_envelope_new(ns);

    cleanup_func = NULL;
    if(type == XMDS_REGISTER_DISPLAY) {
        evp = soap_register_display(evp, param);
    } else
    if(type == XMDS_REQUIRED_FILES) {
        evp = soap_required_files(evp, param);
    } else
    if(type == XMDS_GET_FILE) {
        evp = soap_get_file(evp, param);
    } else
    if(type == XMDS_SCHEDULE) {
        evp = soap_schedule(evp, param);
    } else
    if(type == XMDS_BLACKLIST) {
        evp = soap_blacklist(evp, param);
    } else
    if(type == XMDS_SUBMIT_LOG) {
        evp = soap_submit_log(evp, param);
    } else
    if(type == XMDS_SUBMIT_STATS) {
        evp = soap_submit_stats(evp, param);
    } else
    if(type == XMDS_MEDIA_INVENTORY) {
        evp = soap_media_inventory(evp, param);
    } else
    if(type == XMDS_GET_RESOURCE) {
        evp = soap_get_resource(evp, param);
    } else
    if(type == XMDS_NOTIFY_STATUS) {
        evp = soap_notify_status(evp, param);
    } else
    if(type == XMDS_SUBMIT_SCREENSHOT) {
        evp = soap_submit_screenshot(evp, param);
    }

    xmlInitParser();
    doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    xmlDocSetRootElement(doc, evp->node);

    xmlDocDumpFormatMemoryEnc(doc, &req_xstring, &reqlen, "UTF-8", 0);
    req_cstring = str_duplicate((const char*) req_xstring);

    xmlFree(req_xstring);
    xmlFreeDoc(doc);
    free(evp);

    if(cleanup_func != NULL) {
        cleanup_func(param);
    }

    xmlCleanupParser();

    return req_cstring;
}
