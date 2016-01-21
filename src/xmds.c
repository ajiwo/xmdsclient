#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "mbedtls-base64.h"
#include "xmds-params-func.h"
#include "xmds-soap.h"
#include "xmds-util.h"
#include "xmds.h"


xmdsNode *xmdsRegisterDisplay(xmdsConfig cfg, const char *name, const char *os, const char *type, const char *version, int code, const char *mac) {
    registerDisplayParam param;

    memset(&param, '\0', sizeof(param));

    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.displayName = name;
    param.operatingSystem = os;
    param.clientType = type;
    param.clientVersion = version;
    param.clientCode = code;
    param.macAddress = mac;
    return send_request(cfg.url, XMDS_REGISTER_DISPLAY, &param);
}

xmdsNode *xmdsRequiredFiles(xmdsConfig cfg, int *len) {
    xmdsNode *node;
    requiredFilesParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;

    node = send_request(cfg.url, XMDS_REQUIRED_FILES, &param);
    *len = xmdsNodeLength(node);
    return node;
}

static size_t _decodeAndSave(transportResponse *resp, const char *outname) {
    unsigned char *data;
    FILE *file;
    size_t len;
    size_t written;

    data = malloc(resp->size);
    mbedtls_base64_decode(data, resp->size, &len, (unsigned char *) resp->memory, resp->size);
    free(resp->memory);
    free(resp);

    file = fopen(outname, "a");
    written = fwrite(data, 1, len, file);
    free(data);
    fclose(file);

    return written;
}

static size_t _getPartialFile(xmdsConfig cfg, getFileParam param, const char *filename) {
    getFileParam param0;
    transportResponse *resp;

    char outname[513];


    long offset;
    long size;
    size_t downloaded;
    long remaining;

    long partial_chunk;

    param0.key = param.key;
    param0.fileId = param.fileId;
    param0.fileType = param.fileType;

    size = 0;
    offset = 0;
    downloaded = 0;

    partial_chunk = cfg.maxChunk ? cfg.maxChunk : XMDS_MAX_CHUNK;

    if(filename) {
        sprintf(outname, "%s/%s", cfg.saveDir, filename);
    } else {
        sprintf(outname, "%s/%d.%s", cfg.saveDir, param.fileId, param.fileType);
    }

    /* TODO: instead of deleting, add option to overwrite or continue last failed download */
    unlink(outname);
    if(param.chuckSize >= partial_chunk) {
        while(offset < param.chuckSize) {
            if(offset + size > param.chuckSize) {
                break;
            }
            param0.chunkOffset = offset;
            param0.chuckSize = partial_chunk;
            resp = send_request(cfg.url, XMDS_GET_FILE, &param0);
            if(!resp || !resp->size) {
                break;
            }
            downloaded += _decodeAndSave(resp, outname);
            offset += partial_chunk;
        }
    }

    remaining = param.chuckSize - downloaded;
    param0.chunkOffset = offset;
    param0.chuckSize = remaining;

    resp = send_request(cfg.url, XMDS_GET_FILE, &param0);
    if(!resp || !resp->size) {
        return downloaded;
    }
    downloaded += _decodeAndSave(resp, outname);
    return downloaded;
}

static size_t _xmdsGetFile(xmdsConfig cfg, getFileParam param,  const char *filename) {
    return _getPartialFile(cfg, param, filename);
}

size_t xmdsGetFileNamed(xmdsConfig cfg, int id, const char *type, long offset, long size, const char *filename) {
    getFileParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.fileId = id;
    param.fileType = type;
    param.chunkOffset = offset;
    param.chuckSize = size;

    return _xmdsGetFile(cfg, param, filename);
}

size_t xmdsGetFile(xmdsConfig cfg, int id, const char *type, long offset, long size) {
    return xmdsGetFileNamed(cfg, id, type, offset, size, NULL);
}

xmdsNode *xmdsSchedule(xmdsConfig cfg, int *nlayout, int *ndep) {
    xmdsNode *node;
    scheduleEntry *entry;

    scheduleParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;

    node = send_request(cfg.url, XMDS_SCHEDULE, &param);
    if(node) {
        entry = node->data;
        *nlayout = xmdsNodeLength(entry->layouts);
        *ndep = xmdsNodeLength(entry->dependents);
    } else {
        *nlayout = 0;
        *ndep = 0;
    }

    return node;
}

int xmdsBlacklist(xmdsConfig cfg, int id, const char *type, const char *reason) {
    blacklistParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.mediaId = id;
    param.type = type;
    param.reason = reason;

    return send_request(cfg.url, XMDS_BLACKLIST, &param) ? 1 : 0;
}

int xmdsSubmitLog(xmdsConfig cfg, const char *log) {
    submitLogParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.logXml = log;

    return send_request(cfg.url, XMDS_SUBMIT_LOG, &param) ? 1 : 0;
}

int xmdsSubmitLogNode(xmdsConfig cfg, xmdsNode *log) {
    char *txtLog;
    int success;

    txtLog = submitLogXml_build(log);
    success = xmdsSubmitLog(cfg, txtLog);
    free(txtLog);

    return success;
}

int xmdsSubmitStats(xmdsConfig cfg, const char *stat) {
    submitStatsParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.statXml = stat;

    return send_request(cfg.url, XMDS_SUBMIT_STATS, &param) ? 1 : 0;
}

int xmdsSubmitStatsNode(xmdsConfig cfg, xmdsNode *stat) {
    char *txtStat;
    int success;

    txtStat = submitStatsXml_build(stat);
    success = xmdsSubmitStats(cfg, txtStat);
    free(txtStat);

    return success;
}

int xmdsMediaInventory(xmdsConfig cfg, const char *inv) {
    mediaInventoryParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.mediaInventory = inv;

    return send_request(cfg.url, XMDS_MEDIA_INVENTORY, &param) ? 1 : 0;
}

int xmdsMediaInventoryNode(xmdsConfig cfg, xmdsNode *inv) {
    char *txtInv;
    int success;

    txtInv = mediaInventoryXml_build(inv);
    success = xmdsMediaInventory(cfg, txtInv);
    free(txtInv);

    return success;
}

size_t xmdsGetResourceFile(xmdsConfig cfg, int layout, const char *media, const char *region, const char *path) {
    getResourceParam param;
    char *reso;
    FILE *file;
    char outname[513];
    size_t written;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.layoutId = layout;
    param.mediaId = media;
    param.regionId = region;

    if(path) {
        sprintf(outname, "%s/%s", cfg.saveDir, path);
    } else {
        sprintf(outname, "%s/%s.rsc", cfg.saveDir, param.mediaId);
    }

    reso = send_request(cfg.url, XMDS_GET_RESOURCE, &param);
    written = 0;

    if(reso) {
        file = fopen(outname, "w");
        if(file) {
            written = fwrite(reso, 1, strlen(reso), file);
            fclose(file);
        }
        free(reso);
    }

    return written;
}

size_t xmdsGetResource(xmdsConfig cfg, int layout, const char *media, const char *region) {
    return xmdsGetResourceFile(cfg, layout, media, region, NULL);
}

int xmdsNotifyStatus(xmdsConfig cfg, int curLayout, long availSpace, long totalSpace) {
    notifyStatusParam param;
    notifyStatusJson json;

    char *status;
    int success;

    memset(&json, '\0', sizeof(json));

    json.currentLayoutId = curLayout;
    json.availableSpace = availSpace;
    json.totalSpace = totalSpace;

    status = notifyStatusJson_build(json);

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.status = status;

    success = send_request(cfg.url, XMDS_NOTIFY_STATUS, &param) ? 1 : 0;
    free(status);

    return success;
}

int xmdsSubmitScreenshot(xmdsConfig cfg, const char *screenshot) {
    submitScreenshotParam param;

    memset(&param, '\0', sizeof(param));
    param.key.server = cfg.serverKey;
    param.key.hardware = cfg.hardwareKey;
    param.screenShot = screenshot;

    return send_request(cfg.url, XMDS_SUBMIT_SCREENSHOT, &param) ? 1 : 0;
}

int xmdsSubmitScreenshotFile(xmdsConfig cfg, const char *path) {
    FILE *file;
    int retval;

    unsigned char *original;
    unsigned char *encoded;
    size_t len, enc_len;

    retval = 0;
    file = fopen(path, "r");
    if(!file)
        return 0;

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);

    original = malloc(len);

    if(len == fread(original, 1, len, file)) {
        encoded = malloc(2 * len);
        mbedtls_base64_encode(encoded, len * 2, &enc_len, original, len);
        encoded = realloc(encoded, enc_len + 1);
        retval = xmdsSubmitScreenshot(cfg, (const char *) encoded);
        free(encoded);
    }
    free(original);
    fclose(file);
    return retval;
}

int xmdsFree(xmdsNode *node) {
    int retval;

    retval = -1;
    if(!node) {
        return retval;
    }
    switch (node->_type) {
    case NODE_REGISTER_DISPLAY_ENTRY:
        retval = registerDisplayEntry_delete(&node);
        break;
    case NODE_FILES_ENTRY:
        retval = requiredFilesEntry_delete(&node);
        break;
    case NODE_SCHEDULE_ENTRY:
        retval = scheduleEntry_delete(&node);
        break;
    /* TODO: other possible case of value NODE_* */
    default:
        break;
    }

    return retval;
}

registerDisplayEntry *xmdsRegisterDisplayEntry(xmdsNode *node, int *dlen) {
    registerDisplayEntry *entry;

    entry = NULL;
    if(node) {
        entry = node->data;
        *dlen = xmdsNodeLength(entry->details);
    }

    return entry;
}

registerDisplayDetail *xmdsRegisterDisplayDetail(registerDisplayEntry *entry, int index) {
    xmdsNode *node;
    registerDisplayDetail *detail;

    detail = NULL;
    if(entry) {
        node = xmdsNodeGet(entry->details, index);
        detail = node->data;
    }

    return detail;
}

requiredFilesEntry *xmdsRequiredFilesEntry(xmdsNode *node, int index) {
    xmdsNode *entry_node;
    requiredFilesEntry *entry;

    entry = NULL;

    if(node) {
        entry_node = xmdsNodeGet(node, index);
        entry = entry_node->data;
    }

    return entry;
}

scheduleLayoutEntry *xmdsScheduleLayout(scheduleEntry *entry, int index) {
    xmdsNode *node;
    scheduleLayoutEntry *layout;

    layout = NULL;
    if(entry) {
        node = xmdsNodeGet(entry->layouts, index);
        layout = node->data;
    }

    return layout;
}

scheduleDependents *xmdsScheduleDeps(scheduleEntry *entry, int index) {
    xmdsNode *node;
    scheduleDependents *dep;

    dep = NULL;
    if(entry) {
        node = xmdsNodeGet(entry->dependents, index);
        dep = node->data;
    }

    return dep;
}

/* a wrapper for private APIs. Further refactoring may be needed later. */
xmdsNode *logXmlNew() {
    return submitLogXml_new();
}

char *logXmlBuild(xmdsNode *logXmlNode) {
    return submitLogXml_build(logXmlNode);
}

int logXmlAdd(xmdsNode *parent, submitLogXml log) {
    return submitLogXml_add(&parent, log);
}

xmdsNode *statXmlNew() {
    return submitStatsXml_new();
}

char *statXmlBuild(xmdsNode *statXmlNode) {
    return submitStatsXml_build(statXmlNode);
}

int statXmlAdd(xmdsNode *parent, submitStatsXml stat) {
    return submitStatsXml_add(&parent, stat);
}

xmdsNode *mediaInvXmlNew() {
    return mediaInventoryXml_new();
}

char *mediaInvXmlBuild(xmdsNode *invXmlNode) {
    return mediaInventoryXml_build(invXmlNode);
}

int mediaInvXmlAdd(xmdsNode *parent, mediaInventoryXml inv) {
    return mediaInventoryXml_add(&parent, inv);
}
