#ifndef XMDS_H
#define XMDS_H
#include "xmds-params.h"

typedef struct _xmdsConfig {
    const char *serverKey;          /*!< */
    const char *hardwareKey;        /*!< */
    const char *url;                /*!< */
    const char *saveDir;            /*!< */
    long maxChunk;
    long cmsTzOffset;               /*!< CMS timezone to UTC offset in seconds,
                                        UTC+1 will be 86400,
                                        UTC-1 will be -86400 */
    unsigned long collectInterval;  /*!< default collectInterval when CMS is unreachable */
} xmdsConfig;

#define XMDS_MAX_CHUNK 1234567

#ifdef	__cplusplus
extern "C" {
#endif

/* version */
#define XMDSCLIENT_VER_MAJOR 0
#define XMDSCLIENT_VER_MINOR 0
#define XMDSCLIENT_VER_PATCH 1
#define XMDSCLIENT_VER_EXTRA "development"
const char *libXmdsVersion(int *major, int *minor, int *patch);

/* core xmds verbs */
xmdsNode *xmdsRegisterDisplay(xmdsConfig cfg, const char *name, const char *os, const char *type, const char *version, int code, const char *mac);
xmdsNode *xmdsRequiredFiles(xmdsConfig cfg, int *len);
size_t xmdsGetFile(xmdsConfig cfg, int id, const char *type, long offset, long size);
xmdsNode *xmdsSchedule(xmdsConfig cfg, int *nlayout, int *ndep);
int xmdsBlacklist(xmdsConfig cfg, int id, const char *type, const char *reason);
int xmdsSubmitLog(xmdsConfig cfg, const char *log);
int xmdsSubmitStats(xmdsConfig cfg, const char *stat);
int xmdsMediaInventory(xmdsConfig cfg, const char *inv);
size_t xmdsGetResource(xmdsConfig cfg, int layout, const char *media, const char *region);
int xmdsNotifyStatus(xmdsConfig cfg, int curLayout, long availSpace, long totalSpace);
int xmdsSubmitScreenshot(xmdsConfig cfg, const char *screenshot);

/* shortcuts */
size_t xmdsGetFileNamed(xmdsConfig cfg, int id, const char *type, long offset, long size, const char *filename);
int xmdsSubmitScreenshotFile(xmdsConfig cfg, const char *path);
int xmdsSubmitLogNode(xmdsConfig cfg, xmdsNode *log);
int xmdsSubmitStatsNode(xmdsConfig cfg, xmdsNode *stat);
int xmdsMediaInventoryNode(xmdsConfig cfg, xmdsNode *inv);
size_t xmdsGetResourceFile(xmdsConfig cfg, int layout, const char *media, const char *region, const char *path);

int xmdsFree(xmdsNode *node);
void xmdsMd5sumFile(char *md5sum, const char *path);
registerDisplayEntry *xmdsRegisterDisplayEntry(xmdsNode *node, int *dlen);
registerDisplayDetail *xmdsRegisterDisplayDetail(registerDisplayEntry *entry, int *index, const char *name);
requiredFilesEntry *xmdsRequiredFilesEntry(xmdsNode *node, int index);

scheduleLayoutEntry *xmdsScheduleLayout(scheduleEntry *entry, int index);
scheduleDependents *xmdsScheduleDeps(scheduleEntry *entry, int index);

xmdsNode *logXmlNew();
char *logXmlBuild(xmdsNode *logXmlNode);
int logXmlAdd(xmdsNode *parent, submitLogXml log);

xmdsNode *statXmlNew();
char *statXmlBuild(xmdsNode *statXmlNode);
int statXmlAdd(xmdsNode *parent, submitStatsXml stat);

xmdsNode *mediaInvXmlNew();
char *mediaInvXmlBuild(xmdsNode *invXmlNode);
int mediaInvXmlAdd(xmdsNode *parent, mediaInventoryXml inv);

#ifdef	__cplusplus
}
#endif

#endif /* XMDS_H */
