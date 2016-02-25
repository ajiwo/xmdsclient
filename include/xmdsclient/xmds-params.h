#ifndef XMDS_PARAMS_H
#define XMDS_PARAMS_H
#include <stdlib.h>

#include "xmds-node.h"

typedef enum _xmdsRequestType {
    XMDS_REGISTER_DISPLAY = 0,
    XMDS_REQUIRED_FILES,
    XMDS_GET_FILE,
    XMDS_SCHEDULE,
    XMDS_BLACKLIST,
    XMDS_SUBMIT_LOG,
    XMDS_SUBMIT_STATS,
    XMDS_MEDIA_INVENTORY,
    XMDS_GET_RESOURCE,
    XMDS_NOTIFY_STATUS,
    XMDS_SUBMIT_SCREENSHOT
} xmdsRequestType;


typedef struct _keyParam {
    const char *server;
    const char *hardware;
} keyParam;


typedef struct _registerDisplayParam {
    keyParam key;
    const char *displayName;
    const char *clientType;
    const char *clientVersion;
    int clientCode;
    const char *operatingSystem;
    const char *macAddress;
} registerDisplayParam;
/*
content of Envelope -> Body -> RegisterDisplayResponse -> ActivationMessage
<?xml version="1.0"?>
<display status="0" code="READY" message="Display is active and ready to start." version_instructions="">
  <emailAddress type="string"/>
  <settingsPassword type="string"/>
  <collectInterval type="int">35</collectInterval>
  <orientation type="int">0</orientation>
  <startOnBoot type="checkbox">1</startOnBoot>
  <actionBarMode type="int">1</actionBarMode>
  <actionBarDisplayDuration type="int">30</actionBarDisplayDuration>
  <screenDimensions type="string"/>
  <autoRestart type="checkbox">1</autoRestart>
  <startOnBootDelay type="int">60</startOnBootDelay>
  <blacklistVideo type="checkbox">0</blacklistVideo>
  <storeHtmlOnInternal type="checkbox">0</storeHtmlOnInternal>
  <sendCurrentLayoutAsStatusUpdate type="checkbox">1</sendCurrentLayoutAsStatusUpdate>
  <screenShotRequestInterval type="int">0</screenShotRequestInterval>
  <expireModifiedLayouts type="checkbox">1</expireModifiedLayouts>
  <screenShotIntent type="string"/>
  <updateStartWindow type="string">0</updateStartWindow>
  <updateEndWindow type="string">0</updateEndWindow>
  <downloadStartWindow type="string">0</downloadStartWindow>
  <downloadEndWindow type="string">0</downloadEndWindow>
  <webViewPluginState type="string">DEMAND</webViewPluginState>
  <displayName type="string">AndroidBox</displayName>
  <screenShotRequested type="checkbox">0</screenShotRequested>
</display>
*/
typedef struct _registerDisplayEntry {
    int status;
    char *code;
    char *message;
    char *version_instructions;
    xmdsNode *details;
} registerDisplayEntry;

xmdsNode *registerDisplayEntry_new();

typedef struct _registerDisplayDetail {
    char *name;
    char *type;
    char *value;
} registerDisplayDetail;

/*
<message name="GetFileRequest">
    <part name="serverKey" type="xsd:string" />
    <part name="hardwareKey" type="xsd:string" />
    <part name="fileId" type="xsd:int" />
    <part name="fileType" type="xsd:string" />
    <part name="chunkOffset" type="xsd:double" />
    <part name="chuckSize" type="xsd:double" />
</message>
*/

/* those 'double' above seem like 'long' to me */
#ifndef xibodouble
#define xibodouble unsigned long
#endif

typedef struct _getFileParam {
    keyParam key;
    int fileId;
    const char *fileType;
    xibodouble chunkOffset;
    xibodouble chuckSize;
} getFileParam;


/* schedule */
typedef struct _scheduleParam {
    keyParam key;
} scheduleParam;

typedef struct _scheduleDependents {
    char *file;
} scheduleDependents;

typedef struct _scheduleLayoutEntry {
    char *file;
    char *fromDate;
    char *toDate;
    char *scheduleId;
    int priority;
    char *dependents;
} scheduleLayoutEntry;

typedef struct _scheduleEntry {
    int defaultLayout;
    xmdsNode *layouts;
    xmdsNode *dependents;
} scheduleEntry;

/* required files */
typedef struct _requiredFilesParam {
    keyParam key;
} requiredFilesParam;

typedef struct _requiredFilesEntry {
    char *type;
    char *id;
    long size;
    char *md5;
    char *download;
    char *path;
    /* for resource files */
    int layoutid;
    char *regionid;
    char *mediaid;
    long updated;
} requiredFilesEntry;

/*
<message name="BlackListRequest">
    <part name="serverKey" type="xsd:string" />
    <part name="hardwareKey" type="xsd:string" />
    <part name="mediaId" type="xsd:int" />
    <part name="type" type="xsd:string" />
    <part name="reason" type="xsd:string" />
</message>
*/
typedef struct _blacklistParam {
    keyParam key;
    int mediaId;
    const char *type;
    const char *reason;
} blacklistParam;

/* submit log */
/*
<logs>
    <log date="" category="" type="" message="" method="" thread=""></log>
</logs>
*/
typedef struct _submitLogXml {
    char *date;
    char *category;
    char *type;
    char *message;
    char *method;
    char *thread;
} submitLogXml;

typedef struct _submitLogParam {
    keyParam key;
    const char* logXml;
} submitLogParam;


/* submit stats */
typedef struct _submitStatsParam {
    keyParam key;
    const char* statXml;
} submitStatsParam;

typedef struct _submitStatsXml {
    char *type;
    char *fromdt;
    char *todt;
    char *scheduleid;
    int layoutid;
    char *mediaid;
    char *tag;
} submitStatsXml;

typedef struct _mediaInventoryParam {
    keyParam key;
    const char* mediaInventory;
} mediaInventoryParam;
/*
<files>
    <file id="1" complete="0|1" md5="c90a4c420dd010a5e95dedb8927a29e7" lastChecked="1284569347" />
</files>
*/
typedef struct _mediaInventoryXml {
    int id;
    int complete;
    char *md5;
    long lastChecked;
} mediaInventoryXml;

/*
<message name="GetResourceRequest">
    <part name="serverKey" type="xsd:string" />
    <part name="hardwareKey" type="xsd:string" />
    <part name="layoutId" type="xsd:int" />
    <part name="regionId" type="xsd:string" />
    <part name="mediaId" type="xsd:string" />
</message>
*/
typedef struct _getResourceParam {
    keyParam key;
    int layoutId;
    const char* regionId;
    const char* mediaId;
} getResourceParam;

typedef struct _notifyStatusParam {
    keyParam key;
    const char *status;
} notifyStatusParam;

typedef struct _notifyStatusJson {
    int currentLayoutId;
    long availableSpace;
    long totalSpace;
} notifyStatusJson;

typedef struct _submitScreenshotParam {
    keyParam key;
    const char *screenShot;
} submitScreenshotParam;

#endif /* XMDS_PARAMS_H */

