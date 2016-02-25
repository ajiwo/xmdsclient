#include "xml2-xmds-response.h"
#include "xmds-params-func.h"
#include "xmds-util.h"

#define MAX_RESPONSE_SIZE 1024 * 1024 + 1

static void *_bool_response(xmlNode *response_node, const xmlChar *xstring) {
    xmlChar *xcontent;
    int is_true;

    if(!xmlStrEqual(response_node->name, xstring)) {
        return NULL;
    }

    response_node = xmlFirstElementChild(response_node);

    if(!xmlStrEqual(response_node->name, BAD_CAST "success")) {
        return NULL;
    }

    xcontent = xmlNodeGetContent(response_node);
    is_true = xmlStrEqual(xcontent, BAD_CAST "true");
    xmlFree(xcontent);

    return is_true ? "" : NULL;
}


static xmdsNode *_regdisplay_detail(xmlNode *xnode) {
    xmdsNode *fnode;
    xmlChar *xtmp;
    registerDisplayDetail detail;

    fnode = NULL;
    if(xnode)
        fnode = registerDisplayDetail_new();

    while(xnode) {
        if(xnode->type != XML_ELEMENT_NODE) {
            xnode = xnode->next;
            continue;
        }
        memset(&detail, '\0', sizeof(registerDisplayDetail));

        if(xnode->name) {
            detail.name = str_duplicate((const char*) xnode->name);
        }

        xtmp = xmlGetProp(xnode, BAD_CAST "type");
        if(xtmp) {
            detail.type = str_duplicate((const char*) xtmp);
        }
        xmlFree(xtmp);

        xtmp = xmlNodeGetContent(xnode);
        if(xtmp) {
            detail.value = str_duplicate((const char*) xtmp);
        }
        xmlFree(xtmp);

        registerDisplayDetail_add(&fnode, detail);
        xnode = xnode->next;
    }
    return fnode;
}

void *build_regdisplay_response(xmlNode *response_node) {
    xmlDoc *doc;
    xmlNode *xnode;
    xmlChar *xcontent;
    xmlChar *xtmp;
    char *ctmp;
    xmdsNode *entry_node;
    xmdsNode *detail_node;
    registerDisplayEntry *entry;

    char *ccontent;
    int len;


    if(!xmlStrEqual(response_node->name, BAD_CAST "RegisterDisplayResponse")) {
        return NULL;
    }

    response_node = xmlFirstElementChild(response_node);

    if(!xmlStrEqual(response_node->name, BAD_CAST "ActivationMessage")) {
        return NULL;
    }

    xcontent = xmlNodeGetContent(response_node);
    ccontent = str_duplicate((const char *) xcontent);
    xmlFree(xcontent);
    len = strlen(ccontent);

    doc = xmlReadMemory(ccontent, len, "xmds", "UTF-8", 0);
    xnode = xmlDocGetRootElement(doc);

    if(!xmlStrEqual(xnode->name, BAD_CAST "display")) {
        xmlFreeDoc(doc);
        free(ccontent);
        return NULL;
    }

    entry_node = registerDisplayEntry_new();
    entry = entry_node->data;

    while(xnode) {
        if(xnode->type != XML_ELEMENT_NODE) {
            xnode = xnode->next;
            continue;
        }

        xtmp = xmlGetProp(xnode, BAD_CAST "status");
        if(xtmp) {
            ctmp = str_duplicate((const char *) xtmp);
            entry->status = atoi(ctmp);
            free(ctmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(xnode, BAD_CAST "code");
        if(xtmp) {
            entry->code = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(xnode, BAD_CAST "message");
        if(xtmp) {
            entry->message = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(xnode, BAD_CAST "version_instructions");
        if(xtmp) {
            entry->version_instructions = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        detail_node = _regdisplay_detail(xnode->children);
        if(detail_node) {
            entry->details = detail_node;
        }
        xnode = xnode->next;
    }

    xmlFreeDoc(doc);
    free(ccontent);
    return entry_node;

}

void *build_reqfiles_response(xmlNode *response_node) {
    xmlDoc *doc;
    xmlNode *node;
    xmlChar *xcontent;
    xmlChar *xtmp;
    char *ctmp;
    xmdsNode *entry_node;
    requiredFilesEntry entry;

    char *ccontent;
    int len;


    if(!xmlStrEqual(response_node->name, BAD_CAST "RequiredFilesResponse")) {
        return NULL;
    }

    response_node = xmlFirstElementChild(response_node);

    if(!xmlStrEqual(response_node->name, BAD_CAST "RequiredFilesXml")) {
        return NULL;
    }

    xcontent = xmlNodeGetContent(response_node);
    ccontent = str_duplicate((const char*) xcontent);
    xmlFree(xcontent);
    len = strlen(ccontent);

    doc = xmlReadMemory(ccontent, len, "xmds", "UTF-8", 0);
    node = xmlDocGetRootElement(doc);

    if(!xmlStrEqual(node->name, BAD_CAST "files")) {
        xmlFreeDoc(doc);
        free(ccontent);
        return NULL;
    }

    entry_node = requiredFilesEntry_new();
    node = node->children;

    while(node) {
        if(node->type != XML_ELEMENT_NODE) {
            node = node->next;
            continue;
        }
        memset(&entry, '\0', sizeof(requiredFilesEntry));

        xtmp = xmlGetProp(node, BAD_CAST "type");
        entry.type = str_duplicate((const char *) xtmp);
        xmlFree(xtmp);

        xtmp = xmlGetProp(node, BAD_CAST "id");
        if(xtmp) {
            entry.id = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(node, BAD_CAST "size");
        if(xtmp) {
            ctmp = str_duplicate((const char *) xtmp);
            entry.size = atol(ctmp);
            free(ctmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(node, BAD_CAST "md5");
        if(xtmp) {
            entry.md5 = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(node, BAD_CAST "download");
        if(xtmp) {
            entry.download = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        xtmp = xmlGetProp(node, BAD_CAST "path");
        if(xtmp) {
            entry.path = str_duplicate((const char *) xtmp);
            xmlFree(xtmp);
        }

        if(!strcmp(entry.type, "resource")) {
            xtmp = xmlGetProp(node, BAD_CAST "layoutid");
            if(xtmp) {
                entry.layoutid = atoi((char *) xtmp);
                xmlFree(xtmp);
            }
            xtmp = xmlGetProp(node, BAD_CAST "regionid");
            if(xtmp) {
                entry.regionid = str_duplicate((const char *) xtmp);
                xmlFree(xtmp);
            }
            xtmp = xmlGetProp(node, BAD_CAST "mediaid");
            if(xtmp) {
                entry.mediaid = str_duplicate((const char *) xtmp);
                xmlFree(xtmp);
            }
            xtmp = xmlGetProp(node, BAD_CAST "updated");
            if(xtmp) {
                entry.updated = atol((char *) xtmp);
                xmlFree(xtmp);
            }
        }

        requiredFilesEntry_add(&entry_node, entry);
        node = node->next;
    }

    xmlFreeDoc(doc);
    free(ccontent);
    return entry_node;
}

void *build_getfile_response(transportResponse resp) {
    char *retval;
    transportResponse *tr;

    tr = malloc(sizeof(transportResponse));

    retval = str_between(resp.memory, &(tr->size), "base64Binary\">", "</file>");
    free(resp.memory);

    tr->memory = retval;
    return tr;
}

void parse_sched_layouts(xmlNode *xnode, xmdsNode *schedule_fnode) {
    xmlChar *xtmp;
    char *ctmp;
    xmdsNode *layout_fnode;
    scheduleLayoutEntry *layout;

    layout_fnode = scheduleLayoutEntry_new();
    layout = layout_fnode->data;
    xtmp = xmlGetProp(xnode, BAD_CAST "file");
    if(xtmp) {
        layout->file = str_duplicate((const char*) xtmp);
        xmlFree(xtmp);
    }

    xtmp = xmlGetProp(xnode, BAD_CAST "fromdt");
    if(xtmp) {
        layout->fromDate = str_duplicate((const char*) xtmp);
        xmlFree(xtmp);
    }

    xtmp = xmlGetProp(xnode, BAD_CAST "todt");
    if(xtmp) {
        layout->toDate = str_duplicate((const char*) xtmp);
        xmlFree(xtmp);
    }

    xtmp = xmlGetProp(xnode, BAD_CAST "scheduleid");
    if(xtmp) {
        layout->scheduleId = str_duplicate((const char*) xtmp);
        xmlFree(xtmp);
    }

    xtmp = xmlGetProp(xnode, BAD_CAST "priority");
    if(xtmp) {
        ctmp = str_duplicate((const char *) xtmp);
        layout->priority = atoi(ctmp);
        free(ctmp);
        xmlFree(xtmp);
    }

    xtmp = xmlGetProp(xnode, BAD_CAST "dependents");
    if(xtmp) {
        layout->dependents = str_duplicate((const char*) xtmp);
        xmlFree(xtmp);
    }
    scheduleEntry_add_layout(&schedule_fnode, layout_fnode);
}

xmdsNode *parse_sched_deps(xmlNode *xnode, xmdsNode *schedule_fnode) {
    xmlChar *xtmp;
    xmlNode *child;
    xmdsNode *dep_fnode;
    scheduleDependents dep;

    dep_fnode = scheduleDependents_new();

    child = xnode->children;
    while(child) {
        if(child->type != XML_ELEMENT_NODE) {
            child = child->next;
            continue;
        }
        memset(&dep, '\0', sizeof(scheduleDependents));

        if(xmlStrEqual(child->name, BAD_CAST "file")) {
            xtmp = xmlNodeGetContent(child);
            dep.file = str_duplicate((const char *)xtmp);
            /*fprintf(stderr, "INFO parse_schedule_deps() file: %s\n", dep->file);*/
            xmlFree(xtmp);
        }
        scheduleDependents_add(&dep_fnode, dep);
        child = child->next;
    }
    scheduleEntry_add_deps(&schedule_fnode, dep_fnode);
    return dep_fnode;
}

void *build_sched_response(xmlNode *response_node) {
    xmlDoc *doc;
    xmlNode *xnode;
    xmlChar *xcontent;
    xmlChar *xtmp;
    char *ctmp;
    xmdsNode *sched_fnode;
    scheduleEntry *entry;

    char *ccontent;
    int len;

    ccontent = NULL;
    if(!xmlStrEqual(response_node->name, BAD_CAST "ScheduleResponse")) {
        return NULL;
    }

    response_node = xmlFirstElementChild(response_node);

    if(!xmlStrEqual(response_node->name, BAD_CAST "ScheduleXml")) {
        return NULL;
    }

    xcontent = xmlNodeGetContent(response_node);
    ccontent = str_duplicate((const char *) xcontent);
    xmlFree(xcontent);
    len = strlen(ccontent);

    doc = xmlReadMemory(ccontent, len, "xmds://build_schedule_response", "UTF-8", 0);
    xnode = xmlDocGetRootElement(doc);

    if(!xmlStrEqual(xnode->name, BAD_CAST "schedule")) {
        xmlFreeDoc(doc);
        free(ccontent);
        return NULL;
    }

    sched_fnode = scheduleEntry_new();
    entry = sched_fnode->data;

    xnode = xnode->children;
    while(xnode) {
        if(xnode->type != XML_ELEMENT_NODE) {
            xnode = xnode->next;
            continue;
        }

        if(xmlStrEqual(xnode->name, BAD_CAST "default")) {
            xtmp = xmlGetProp(xnode, BAD_CAST "file");
            if(xtmp) {
                ctmp = str_duplicate((const char *) xtmp);
                entry->defaultLayout = atoi(ctmp);
                free(ctmp);
                xmlFree(xtmp);
            }
        }

        if(xmlStrEqual(xnode->name, BAD_CAST "layout")) {
            parse_sched_layouts(xnode, sched_fnode);
        }

        if(xmlStrEqual(xnode->name, BAD_CAST "dependants")) {
            parse_sched_deps(xnode, sched_fnode);
        }


        xnode = xnode->next;
    }

    xmlFreeDoc(doc);
    free(ccontent);
    return sched_fnode;
}

void *build_blacklist_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "BlackListResponse");
}


void *build_getrsc_response(xmlNode *response_node) {
    char *ccontent;
    xmlChar *xcontent;

    ccontent = NULL;
    if(!xmlStrEqual(response_node->name, BAD_CAST "GetResourceResponse")) {
        return NULL;
    }

    response_node = xmlFirstElementChild(response_node);

    if(!xmlStrEqual(response_node->name, BAD_CAST "resource")) {
        return NULL;
    }

    xcontent = xmlNodeGetContent(response_node);
    ccontent = str_duplicate((const char *) xcontent);
    xmlFree(xcontent);
    return ccontent;
}

void *build_submitlog_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "SubmitLogResponse");
}

void *build_submitstat_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "SubmitStatsResponse");
}

void *build_notifystatus_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "NotifyStatusResponse");
}

void *build_mediainv_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "MediaInventoryResponse");
}

void *build_submitsshot_response(xmlNode *response_node) {
    return _bool_response(response_node, (const xmlChar *) "SubmitScreenShotResponse");
}



void *build_response(xmlNode *node, xmdsRequestType type) {
    void *retval;
    /** TODO: handle soap Fault here */

    if(type == XMDS_REGISTER_DISPLAY) {
        retval = build_regdisplay_response(node);
    } else if(type == XMDS_REQUIRED_FILES) {
        retval = build_reqfiles_response(node);
    } else if(type == XMDS_SCHEDULE) {
        retval = build_sched_response(node);
    } else if(type == XMDS_BLACKLIST) {
        retval = build_blacklist_response(node);
    } else if(type == XMDS_REQUIRED_FILES) {
        retval = build_reqfiles_response(node);
    } else if(type == XMDS_GET_RESOURCE) {
        retval = build_getrsc_response(node);
    } else if(type == XMDS_SUBMIT_LOG) {
        retval = build_submitlog_response(node);
    } else if(type == XMDS_SUBMIT_STATS) {
        retval = build_submitstat_response(node);
    } else if(type == XMDS_MEDIA_INVENTORY) {
        retval = build_mediainv_response(node);
    } else if(type == XMDS_NOTIFY_STATUS) {
        retval = build_notifystatus_response(node);
    } else if(type == XMDS_SUBMIT_SCREENSHOT) {
        retval = build_submitsshot_response(node);
    }
    else {
        retval = NULL;
    }

    return retval;
}

void *parse_response(transportResponse resp, xmdsRequestType type) {
    xmlDoc *doc;
    xmlNode *node;
    void *retval;
    retval = NULL;

    if(type == XMDS_GET_FILE) {
        return build_getfile_response(resp);
    }

    if(resp.size < 1) {
        if(resp.memory) {
            free(resp.memory);
            resp.memory = NULL;
        }
        return NULL;
    }

    xmlInitParser();
    doc = xmlReadMemory(resp.memory, resp.size, "xmds://parse_response/memory", "UTF-8", 0);

    if(doc == NULL) {
        xmlCleanupParser();

        return NULL;
    }
    node = xmlDocGetRootElement(doc);
    if(node == NULL) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    if(!xmlStrEqual(node->name, BAD_CAST "Envelope")) {
        xmlFreeNode(node);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    node = xmlFirstElementChild(node);
    if(!xmlStrEqual(node->name, BAD_CAST "Body")) {
        xmlFreeNode(node);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    node = xmlFirstElementChild(node);
    retval = build_response(node, type);

    xmlFreeDoc(doc);
    xmlCleanupMemory();
    xmlCleanupParser();
    free(resp.memory);
    return retval;
}
