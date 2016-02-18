#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "mbedtls-base64.h"
#include "xmds-params-func.h"
#include "xmds-soap.h"
#include "xmds-util.h"
#include "xmds.h"

static int _configFindValue(const char *src, const char *key, int *v_start, int *v_end) {
    int i, j, k;
    int c;

    /* key start index */
    int k_start;

    /* src length, key length */
    int s_len, k_len;

    /* strlen(key) */
    if(key)
        for(k_len = 0; key[k_len] != '\0'; k_len++);
    else
        return 0;
    /* strlen(src) */
    if(src)
        for(s_len = 0; src[s_len] != '\0'; s_len++);
    else
        return 0;

    *v_start = *v_end = 0;
    i = 0;
    while((c = src[i++]) != '\0' && i < s_len) {
        /* skip preceeding spaces after last newline */
        if(c == ' ' || c == '\t' || c == '\r')
            continue;

        /* skip comment line after last newline */
        if(c == '#') {
            while((c = src[i++]) != '\n');
            continue;
        }

        /* strstr(data, key)
         * find occurence of key inside data.
         * j == k_len if found, (i - 1) is the index of the key.
        */
        for(j = 0; j < k_len; j++) {
            if(src[i + j - 1] != key[j]) {
                break;
            }
        }

        /* found */
        if(j == k_len) {
            k_start = i - 1;
            while((c = src[i++]) != '\n') {
                /* last line that doesn't have a terminating newline */
                if(c == '\0')
                    break;
            }

            *v_end = i - 1;
            for(k = k_start + k_len; k < *v_end; k++) {
                c = src[k];
                /* delimiters */
                if(c == ' ' || c == '\t' || c == '=')
                    continue;
                *v_start = k;
                break;
            }

            /* strip trailing spaces */
            k = *v_end;
            while((c = src[--k])) {
                if(c == ' ' || c == '\t' || c == '\r')
                    continue;
                else {
                    *v_end = ++k;
                    break;
                }
            }
            break;
        } else {
            /* not found, skip until the next new line. */
            while((c = src[i++]) != '\n' && i < s_len);
        }
    }

    return *v_end - *v_start;
}

static char *_configGetString(const char *src, const char *key) {
    int start, end, len;
    char *dst;
    int i;

    dst = NULL;
    len = _configFindValue(src, key, &start, &end);
    if(len > 0) {
        dst = malloc(len + 1);
        dst[len] = '\0';
        /* copy src[start] to src[end] -> dst */
        for(i = 0;
            i < len && (dst[i] = *(src + start + i));
            i++);

    }

    return dst;
}

static double _configGetNumber(const char *src, const char *key) {
    char *value;
    double num;

    value = NULL;
    value = _configGetString(src, key);
    num = atof(value);
    free(value);
    return num;
}

static double _configGetSeconds(const char *src, const char *key) {
    char *value;
    char *unit;
    double num;

    value = _configGetString(src, key);
    num = strtod(value, &unit);

    if(unit[0] == 'h' || unit[0] == 'H')
        num *= 3600;
    else if(unit[0] == 'm' || unit[0] == 'm')
        num *= 60;

    free(value);
    return num;
}

void xmdsConfigInit(xmdsConfig *cfg) {
    cfg->url = NULL;
    cfg->serverKey = NULL;
    cfg->hardwareKey = NULL;
    cfg->saveDir = NULL;
    cfg->maxChunk = XMDS_MAX_CHUNK;
    cfg->collectInterval = XMDS_COLLECT_INTERVAL;
    cfg->cmsTzOffset = 0;
}

int xmdsConfigParse(xmdsConfig *cfg, const char *src) {

    cfg->url = _configGetString(src, "url");
    cfg->serverKey = _configGetString(src, "serverKey");
    cfg->hardwareKey = _configGetString(src, "hardwareKey");
    cfg->saveDir = _configGetString(src, "saveDir");
    cfg->maxChunk = _configGetNumber(src, "maxChunk");
    cfg->collectInterval = _configGetSeconds(src, "collectInterval");
    cfg->cmsTzOffset = _configGetSeconds(src, "cmsTzOffset");

    return (cfg->url &&
            cfg->serverKey &&
            cfg->hardwareKey &&
            cfg->saveDir);
}

void xmdsConfigFree(xmdsConfig *cfg) {
    if(cfg) {
        if(cfg->url) {
            free(cfg->url);
            cfg->url = NULL;
        }
        if(cfg->serverKey) {
            free(cfg->serverKey);
            cfg->serverKey = NULL;
        }
        if(cfg->hardwareKey) {
            free(cfg->hardwareKey);
            cfg->hardwareKey = NULL;
        }
        if(cfg->saveDir) {
            free(cfg->saveDir);
            cfg->saveDir = NULL;
        }
    }
}

const char *libXmdsVersion(int *major, int *minor, int *patch) {
    *major = XMDSCLIENT_VER_MAJOR;
    *minor = XMDSCLIENT_VER_MINOR;
    *patch = XMDSCLIENT_VER_PATCH;

    return XMDSCLIENT_VER_EXTRA;
}

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

    written = 0;
    file = fopen(outname, "a");
    if(file) {
        written = fwrite(data, 1, len, file);
        fflush(file);
        free(data);
        fclose(file);
    }

    return written;
}

static size_t _getPartialFile(xmdsConfig cfg, getFileParam param, const char *filename) {
    getFileParam param0;
    transportResponse *resp;

    char outname[513];


    long offset;
    size_t downloaded;
    long remaining;

    long partial_chunk;

    param0.key = param.key;
    param0.fileId = param.fileId;
    param0.fileType = param.fileType;

    offset = 0;
    downloaded = 0;
    remaining = param.chuckSize;

    partial_chunk = cfg.maxChunk ? cfg.maxChunk : XMDS_MAX_CHUNK;

    if(filename) {
        sprintf(outname, "%s", filename);
    } else {
        sprintf(outname, "%s/%d.%s", cfg.saveDir, param.fileId, param.fileType);
    }

    /* TODO: instead of deleting, add option to overwrite or continue last failed download */
    remove(outname);
    if(param.chuckSize >= partial_chunk) {
        while(downloaded < param.chuckSize) {
            param0.chunkOffset = offset;
            param0.chuckSize = partial_chunk < remaining ? partial_chunk : remaining;
            resp = send_request(cfg.url, XMDS_GET_FILE, &param0);
            if(!resp || !resp->size) {
                break;
            }
            downloaded += _decodeAndSave(resp, outname);
            offset += partial_chunk;
            remaining = param.chuckSize - downloaded;
        }
    }

    if(remaining > 0) {
        param0.chunkOffset = offset;
        param0.chuckSize = remaining;

        resp = send_request(cfg.url, XMDS_GET_FILE, &param0);
        if(!resp || !resp->size) {
            return downloaded;
        }
        downloaded += _decodeAndSave(resp, outname);
    }
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

void xmdsMd5sumFile(char *md5sum, const char *path) {
    file_md5sum(md5sum, path);
}

registerDisplayEntry *xmdsRegisterDisplayEntry(xmdsNode *node, int *dlen) {
    registerDisplayEntry *entry;

    *dlen = -1;
    entry = NULL;
    if(node) {
        entry = node->data;
        *dlen = xmdsNodeLength(entry->details);
    }

    return entry;
}

registerDisplayDetail *xmdsRegisterDisplayDetail(registerDisplayEntry *entry, int *index, const char *name) {
    xmdsNode *node;
    registerDisplayDetail *detail;
    int i, len;
    char *tmp1, *tmp2;

    detail = NULL;
    if(entry) {
        if(name != NULL) {
            *index = -1;
            tmp1 = str_duplicate(name);
            len = xmdsNodeLength(entry->details);
            for(i = 0; i < len; i++) {
                node = xmdsNodeGet(entry->details, i);
                tmp2 = str_duplicate(((registerDisplayDetail *)(node->data))->name);
                str_tolower(tmp1);
                str_tolower(tmp2);
                if(!strcmp(tmp1, tmp2)) {
                    *index = i;
                    free(tmp2);
                    break;
                }
                free(tmp2);
            }
            free(tmp1);
            if(*index < 0) {
                detail = NULL;
            } else {
                detail = node->data;
            }
        } else {
            node = xmdsNodeGet(entry->details, *index);
            detail = node->data;
        }
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
