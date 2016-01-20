#include "xmds-params-func.h"
#include "xml2-xmds.h"
#include "xmds-util.h"

soapEnvelope *soap_register_display(soapEnvelope *evp, registerDisplayParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "RegisterDisplay");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "displayName", param->displayName));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "clientType", param->clientType));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "clientVersion", param->clientVersion));
    xmlAddChild(func_node, soap_new_int_param(evp, BAD_CAST "clientCode", param->clientCode));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "operatingSystem", param->operatingSystem));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "macAddress", param->macAddress));


    return evp;
}

soapEnvelope *soap_required_files(soapEnvelope *evp, requiredFilesParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "RequiredFiles");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));

    return evp;

}

soapEnvelope *soap_get_file(soapEnvelope *evp, getFileParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "GetFile");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_int_param(evp, BAD_CAST "fileId", param->fileId));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "fileType", param->fileType));
    xmlAddChild(func_node, soap_new_xibodouble_param(evp, BAD_CAST "chunkOffset", param->chunkOffset));
    xmlAddChild(func_node, soap_new_xibodouble_param(evp, BAD_CAST "chuckSize", param->chuckSize));

    return evp;
}

soapEnvelope *soap_schedule(soapEnvelope *evp, scheduleParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "Schedule");


    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));

    return evp;
}

soapEnvelope *soap_blacklist(soapEnvelope *evp, blacklistParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "Blacklist");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_int_param(evp, BAD_CAST "mediaId", param->mediaId));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "type", param->type));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "reason", param->reason));

    return evp;
}

soapEnvelope *soap_submit_log(soapEnvelope *evp, submitLogParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "SubmitLog");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "logXml", param->logXml));

    return evp;
}

soapEnvelope *soap_submit_stats(soapEnvelope *evp, submitStatsParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "SubmitStats");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "statXml", param->statXml));

    return evp;
}

soapEnvelope *soap_media_inventory(soapEnvelope *evp, mediaInventoryParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "MediaInventory");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "mediaInventory", param->mediaInventory));

    return evp;
}

soapEnvelope *soap_get_resource(soapEnvelope *evp, getResourceParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "GetResource");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_int_param(evp, BAD_CAST "layoutId", param->layoutId));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "regionId", param->regionId));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "mediaId", param->mediaId));

    return evp;
}

soapEnvelope *soap_notify_status(soapEnvelope *evp, notifyStatusParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "NotifyStatus");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "status", param->status));

    return evp;
}

soapEnvelope *soap_submit_screenshot(soapEnvelope *evp, submitScreenshotParam *param) {
    xmlNode *func_node;

    func_node = soap_add_funcbody(evp, "SubmitScreenShot");

    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "serverKey", param->key.server));
    xmlAddChild(func_node, soap_new_string_param(evp, BAD_CAST "hardwareKey", param->key.hardware));
    xmlAddChild(func_node, soap_new_base64binary_param(evp, BAD_CAST "screenShot", param->screenShot));

    return evp;
}

char *submitLogXml_build(xmdsNode *node) {
    xmlNode *xroot, *xlog, *xchild;
    xmdsNode *iter;
    xmlDoc *doc;
    xmlChar *xtmp;
    submitLogXml *log;
    char *retval;

    int doc_len;

    xroot = xmlNewNode(NULL, BAD_CAST "logs");


    xmlInitParser();
    doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    xmlDocSetRootElement(doc, xroot);


    for(iter = node->first; iter != NULL; iter = iter->next) {
        log = iter->data;
        xlog = xmlNewNode(NULL, BAD_CAST "log");
        xmlSetProp(xlog, BAD_CAST "date", BAD_CAST log->date);
        xmlSetProp(xlog, BAD_CAST "category", BAD_CAST log->category);

        xchild = xmlNewNode(NULL, BAD_CAST "message");
        xmlNodeAddContent(xchild, BAD_CAST log->message);
        xmlAddChild(xlog, xmlCopyNode(xchild, 1));
        xmlFreeNode(xchild);

        xchild = xmlNewNode(NULL, BAD_CAST "method");
        xmlNodeAddContent(xchild, BAD_CAST log->method);
        xmlAddChild(xlog, xmlCopyNode(xchild, 1));
        xmlFreeNode(xchild);

        xchild = xmlNewNode(NULL, BAD_CAST "thread");
        xmlNodeAddContent(xchild, BAD_CAST log->thread);
        xmlAddChild(xlog, xmlCopyNode(xchild, 1));
        xmlFreeNode(xchild);

        xmlAddChild(xroot, xmlCopyNode(xlog, 1));
        xmlFreeNode(xlog);
    }


    xmlDocDumpFormatMemoryEnc(doc, &xtmp, &doc_len, "UTF-8", 0);

    retval = str_duplicate((const char*) xtmp);
    xmlFree(xtmp);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    submitLogXml_delete(&node);
    return retval;
}

char *notifyStatusJson_build(notifyStatusJson value) {
    char *buf;
    int len;
    const char *tpl = "{ \"currentLayoutId\": \"%d\", \"availableSpace\": \"%ld\", \"totalSpace\": \"%ld\"}";

    buf = malloc(5 * strlen(tpl));
    sprintf(buf, tpl, value.currentLayoutId, value.availableSpace, value.totalSpace);
    len = strlen(buf);
    buf = realloc(buf, len + 1);
    buf[len] = '\0';
    return buf;
}

char *submitStatsXml_build(xmdsNode *node) {
    xmlNode *xroot, *xchild;
    xmdsNode *iter;
    xmlDoc *doc;
    xmlChar *xtmp;
    submitStatsXml *stat;
    char ctmp[16];
    char *retval;

    int doc_len;


    xroot = xmlNewNode(NULL, BAD_CAST "stats");

    xmlInitParser();
    doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    xmlDocSetRootElement(doc, xroot);

    for(iter = node->first; iter != NULL; iter = iter->next) {
        stat = iter->data;

        xchild = xmlNewNode(NULL, BAD_CAST "stat");
        xmlSetProp(xchild, BAD_CAST "type", BAD_CAST stat->type);
        xmlSetProp(xchild, BAD_CAST "fromdt", BAD_CAST stat->fromdt);
        xmlSetProp(xchild, BAD_CAST "todt", BAD_CAST stat->todt);
        xmlSetProp(xchild, BAD_CAST "scheduleid", BAD_CAST stat->scheduleid);
        sprintf(ctmp, "%d", stat->layoutid);
        xmlSetProp(xchild, BAD_CAST "layoutid", BAD_CAST ctmp);
        xmlSetProp(xchild, BAD_CAST "mediaid", BAD_CAST stat->mediaid);
        xmlSetProp(xchild, BAD_CAST "tag", BAD_CAST stat->tag);
        xmlAddChild(xroot, xmlCopyNode(xchild, 1));
        xmlFreeNode(xchild);
    }

    xmlDocDumpFormatMemoryEnc(doc, &xtmp, &doc_len, "UTF-8", 0);
    retval = str_duplicate((const char*) xtmp);
    xmlFree(xtmp);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    submitStatsXml_delete(&node);
    return retval;
}

char *mediaInventoryXml_build(xmdsNode *node) {
    xmlNode *xroot, *xchild;
    xmdsNode *iter;
    xmlDoc *doc;
    xmlChar *xtmp;
    mediaInventoryXml *file;
    char ctmp[26];
    char *retval;

    int doc_len;


    xroot = xmlNewNode(NULL, BAD_CAST "files");

    xmlInitParser();
    doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    xmlDocSetRootElement(doc, xroot);

    for(iter = node->first; iter != NULL; iter = iter->next) {
        file = iter->data;

        xchild = xmlNewNode(NULL, BAD_CAST "file");

        sprintf(ctmp, "%d", file->id);
        xmlSetProp(xchild, BAD_CAST "id", BAD_CAST ctmp);

        sprintf(ctmp, "%d", file->complete);
        xmlSetProp(xchild, BAD_CAST "complete", BAD_CAST ctmp);

        xmlSetProp(xchild, BAD_CAST "md5", BAD_CAST file->md5);

        sprintf(ctmp, "%ld", file->lastChecked);
        xmlSetProp(xchild, BAD_CAST "lastChecked", BAD_CAST ctmp);

        xmlAddChild(xroot, xmlCopyNode(xchild, 1));
        xmlFreeNode(xchild);
    }

    xmlDocDumpFormatMemoryEnc(doc, &xtmp, &doc_len, "UTF-8", 0);
    retval = str_duplicate((const char*) xtmp);
    xmlFree(xtmp);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    mediaInventoryXml_delete(&node);
    return retval;
}
