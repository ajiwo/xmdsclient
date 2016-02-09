#include <string.h>
#include <stdio.h>
#include <time.h>
#include <xmdsclient/xmds.h>

#define XMDS_DATE_FORMAT "%Y-%m-%d %H:%M:%S"

xmdsConfig cfg;

void set_config() {
    extern xmdsConfig cfg;

    memset(&cfg, '\0', sizeof(xmdsConfig));
    cfg.serverKey = "secret-between-client-and-server";
    cfg.hardwareKey = "some-l0n9-device-unique-identifier";
    cfg.url = "http://localhost:8000/xmds.php?v=4";
    cfg.saveDir = "/tmp";
    cfg.maxChunk = 1000000;

}

void test_register_display() {
    xmdsNode *node;
    registerDisplayEntry *entry;
    registerDisplayDetail *detail;

    int i, nd;

    node = xmdsRegisterDisplay(cfg, "libxmdsclient", "Linux", "Android", "0.0.1", 1, "ab:ac:ad:ae:af:bc");

    entry = xmdsRegisterDisplayEntry(node, &nd);
    if(entry) {
        printf("reg_display:");
        printf("  status '%d, code '%s', message '%s'\n", entry->status, entry->code, entry->message);
        for(i = 0; i < nd; i++) {
            detail = xmdsRegisterDisplayDetail(entry, &i, NULL);
            if(detail) {
                printf("  %s(%s) = '%s'\n", detail->name, detail->type, detail->value);
            }
        }
    }
    xmdsFree(node);

}

void test_required_files() {
    xmdsNode *node;
    requiredFilesEntry *entry;
    int i, len;

    node = xmdsRequiredFiles(cfg, &len);
    printf("req files\n");
    for(i = 0; i < len; i++) {
        entry = xmdsRequiredFilesEntry(node, i);
        if(entry) {
            printf("  type '%s', id '%s', md5 '%s', size '%ld', path '%s', download '%s'\n",
                   entry->type, entry->id, entry->md5, entry->size, entry->path, entry->download);
        }
    }
    xmdsFree(node);
}

void test_get_file() {
    size_t downloaded;

    printf("get file\n");
    /* /tmp is the value of cfg.saveDir */

    /* the file will be saved as /tmp/21.media */
    downloaded = xmdsGetFile(cfg, 21, "media", 0, 19304423);
    /* or, to save the file as /tmp/primary-ads.mp4 */
    /* downloaded = xmdsGetFileNamed(cfg, 21, "media", 0, 19304423, "primary-ads.mp4"); */
    printf("  media downloaded: %ld bytes\n", (long)downloaded);

    /* the file will be saved as 7.layout */
    downloaded = xmdsGetFile(cfg, 7, "layout", 0, 828);
    /* or, to save the file as /tmp/default.xlf */
    /* downloaded = xmdsGetFileNamed(cfg, 7, "layout", 0, 828, "default.xlf"); */
    printf("  layout downloaded: %ld bytes\n", (long)downloaded);
}

void test_schedule() {
    xmdsNode *node;
    scheduleEntry *entry;
    scheduleLayoutEntry *layout;
    scheduleDependents *dep;
    int i, nl, nd;

    node = xmdsSchedule(cfg, &nl, &nd);
    if(node) {
        entry = node->data;
        printf("sched layouts\n");
        for(i = 0; i < nl; i++) {
            layout = xmdsScheduleLayout(entry, i);
            if(layout) {
                printf("  file '%s', from '%s', to '%s', schedId '%s', priority '%d', deps '%s'\n",
                       layout->file, layout->fromDate, layout->toDate, layout->scheduleId, layout->priority, layout->dependents);
            }
        }
        printf("sched deps\n");
        for(i = 0; i < nd; i++) {
            dep = xmdsScheduleDeps(entry, i);
            if(layout) {
                printf("  file '%s'\n", dep->file);
            }
        }
    }

    xmdsFree(node);
}

void test_blacklist() {
    if(xmdsBlacklist(cfg, 9, "media", "unplayable")) {
        printf("test_blacklist returns true\n");
    } else {
        printf("test_blacklist returns false\n");
    }
}

void test_submit_log() {
    /* craft your log manually */
    /*
    const char *myFineCraftedLog = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<logs><log date=\"2016-01-11 19:44:18\" category=\"audit\"><message>Hello, world!</message><method>test_submit_log(1)</method><thread>main</thread></log><log date=\"2016-01-11 19:44:18\" category=\"audit\"><message>Lorem ipsum message</message><method>test_submit_log(2)</method><thread>main</thread></log></logs>";
    int success = xmdsSubmitLog(cfg, myFineCraftedLog);
    */

    /* but it'd be easier using logXmlAdd and xmdsSubmitLogNode */

    submitLogXml log;
    xmdsNode *node;

    time_t now;
    struct tm *tnow;
    char now_str[32];

    int success;

    now = time(NULL);
    tnow = localtime(&now);
    strftime(now_str, sizeof(now_str) - 1, XMDS_DATE_FORMAT, tnow);

    memset(&log, '\0', sizeof(submitLogXml));

    node = logXmlNew();
    log.category = "audit";
    log.date = now_str;
    log.message = "Hello, world!";
    log.method = "test_submit_log(1)";
    log.thread = "main";
    logXmlAdd(node, log);

    log.category = "audit";
    log.date = now_str;
    log.message = "Lorem ipsum message";
    log.method = "test_submit_log(2)";
    log.thread = "main";
    logXmlAdd(node, log);

    /* partially crafted, eg., for debugging the submitted xml */
    /*
      char *txtLog = logXmlBuild(node);
      success = xmdsSubmitLog(cfg, txtLog);
      printf("%s\n", txtLog);
      free(txtLog);
    */

    success = xmdsSubmitLogNode(cfg, node);
    if(success) {
        printf("test_submit_log returns true\n");
    } else {
        printf("test_submit_log returns false\n");
    }
}

void test_submit_stats() {
    submitStatsXml stat;
    xmdsNode *node;

    time_t now;
    time_t past;

    char now_str[32];
    char past_str[32];

    int success;

    now = time(NULL);
    past = now - 40;

    strftime(now_str, sizeof(now_str) - 1, XMDS_DATE_FORMAT, localtime(&now));
    strftime(past_str, sizeof(past_str) - 1, XMDS_DATE_FORMAT, localtime(&past));

    memset(&stat, '\0', sizeof(submitStatsXml));
    stat.type = "media";
    stat.fromdt = now_str;
    stat.todt = past_str;
    stat.scheduleid = "";
    stat.layoutid = 6;
    stat.mediaid = "21";

    node = statXmlNew();
    statXmlAdd(node, stat);
    success = xmdsSubmitStatsNode(cfg, node);


    if(success) {
        printf("test_submit_stats returns true\n");
    } else {
        printf("test_submit_stats returns false\n");
    }
}

void test_media_inventory() {
    /* the main function is xmdsMediaInventory, but this example is using xmdsMediaInventoryNode,
       for the same reason as xmdsSubmitLogNode and xmdsSubmitStatsNode usage */

    mediaInventoryXml inv;
    xmdsNode *node;

    char md5sum[32];
    char path[257];
    int success;

    sprintf(path, "%s/%s", cfg.saveDir, "21.media");

    xmdsMd5sumFile(md5sum, path);

    node = mediaInvXmlNew();
    memset(&inv, '\0', sizeof(mediaInventoryXml));
    inv.id = 21;
    inv.complete = 1;
    inv.md5 = md5sum;
    inv.lastChecked = time(NULL);
    mediaInvXmlAdd(node, inv);
    success = xmdsMediaInventoryNode(cfg, node);


    if(success) {
        printf("test_media_inventory returns true\n");
    } else {
        printf("test_media_inventory returns false\n");
    }

}

void test_get_resource() {
    size_t downloaded;
    /* the value of media id: 4b3e4d5d7814db4b29c2b12b195f8739 is inside a layout file,
       should be parsed by xlfparser */
    downloaded = xmdsGetResource(cfg, 7, "4b3e4d5d7814db4b29c2b12b195f8739", "56976ba48632f");

    printf("test_get_resource downloaded %ld bytes\n", (long) downloaded);
}

void test_notify_status() {
    int success = 0;

    success = xmdsNotifyStatus(cfg, 6, 12582912, 33554432);
    if(success) {
        printf("test_notify_status returns true\n");
    } else {
        printf("test_notify_status returns false\n");
    }

}

void test_screenshot() {
    /* the core function: xmdsSubmitScreenshot(cfg, veryLongBase64EncodedString)  */
    /* this example is using the xmdsSubmitScreenshotFile(cfg, pathToFile) variant. */

    char path[129];
    int success;
    sprintf(path, "%s/%s", cfg.saveDir, "sshot.png");
    success = xmdsSubmitScreenshotFile(cfg, path);
    if(success) {
        printf("test_screenshot returns true\n");
    } else {
        printf("test_screenshot returns false\n");
    }
}

int main(void)
{
    set_config();
    test_register_display();
    test_required_files();
    test_get_file();
    test_schedule();
    test_blacklist();
    test_submit_log();
    test_submit_stats();
    test_media_inventory();
    test_get_resource();
    test_notify_status();
    test_screenshot();
    return 0;
}

