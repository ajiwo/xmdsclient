#ifndef XML2_XMDS_H
#define XML2_XMDS_H
#include "xml2-soap.h"

soapEnvelope *soap_register_display(soapEnvelope *evp, registerDisplayParam *param);
soapEnvelope *soap_required_files(soapEnvelope *evp, requiredFilesParam *param);
soapEnvelope *soap_get_file(soapEnvelope *evp, getFileParam *param);
soapEnvelope *soap_schedule(soapEnvelope *evp, scheduleParam *param);
soapEnvelope *soap_blacklist(soapEnvelope *evp, blacklistParam *param);
soapEnvelope *soap_submit_log(soapEnvelope *evp, submitLogParam *param);
soapEnvelope *soap_submit_stats(soapEnvelope *evp, submitStatsParam *param);
soapEnvelope *soap_media_inventory(soapEnvelope *evp, mediaInventoryParam *param);
soapEnvelope *soap_get_resource(soapEnvelope *evp, getResourceParam *param);
soapEnvelope *soap_notify_status(soapEnvelope *evp, notifyStatusParam *param);
soapEnvelope *soap_submit_screenshot(soapEnvelope *evp, submitScreenshotParam *param);

char *submitLogXml_build(xmdsNode *node);
char *submitStatsXml_build(xmdsNode *node);
char *notifyStatusJson_build(notifyStatusJson value);
char *mediaInventoryXml_build(xmdsNode *node);
#endif /* XML2_XMDS_H */

