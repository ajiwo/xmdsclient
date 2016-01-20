#include <stdio.h>
#include <string.h>
#include "xmds-params-func.h"
#include "xmds-util.h"

static void set_rfiles_entry_value(requiredFilesEntry *dst, requiredFilesEntry src) {
    dst->type = src.type;
    dst->id = src.id;
    dst->size = src.size;
    dst->md5 = src.md5;
    dst->download = src.download;
    dst->path = src.path;
}

xmdsNode *requiredFilesEntry_new() {
    xmdsNode *node;
    requiredFilesEntry *entry;

    node = xmdsNodeNew(NODE_FILES_ENTRY);
    entry = calloc(1, sizeof(requiredFilesEntry));
    node->data = entry;

    return node;
}

int requiredFilesEntry_add(xmdsNode **parent, requiredFilesEntry entry_value) {
    xmdsNode *entry_node;
    requiredFilesEntry *existing, *incoming;

    existing = (*parent)->data;
    if(!existing->type) {
        set_rfiles_entry_value(existing, entry_value);
    } else {
        entry_node = requiredFilesEntry_new();
        incoming = entry_node->data;
        set_rfiles_entry_value(incoming, entry_value);
        xmdsNodeAppend(parent, entry_node);
    }
    return xmdsNodeLength(*parent);
}

int requiredFilesEntry_delete_last(xmdsNode **node) {
    xmdsNode *last;
    requiredFilesEntry *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    free(entry->type);
    free(entry->id);
    free(entry->md5);
    free(entry->download);
    free(entry->path);

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int requiredFilesEntry_delete(xmdsNode **node) {
    while(*node) {
        requiredFilesEntry_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *registerDisplayDetail_new() {
    xmdsNode *node;
    registerDisplayDetail *detail;

    node = xmdsNodeNew(NODE_REGISTER_DISPLAY_DETAIL);
    detail = calloc(1, sizeof(registerDisplayDetail));
    node->data = detail;

    return node;
}

static void set_display_detail_value(registerDisplayDetail *dst, registerDisplayDetail src) {
    dst->name = src.name;
    dst->type = src.type;
    dst->value = src.value;
}

int registerDisplayDetail_add(xmdsNode **parent, registerDisplayDetail detail_value) {
    xmdsNode *entry_node;
    registerDisplayDetail *existing;

    existing = (*parent)->data;
    if(existing && !existing->name) {
        set_display_detail_value(existing, detail_value);
    } else {
        entry_node = registerDisplayDetail_new();
        existing = entry_node->data;
        set_display_detail_value(existing, detail_value);
        xmdsNodeAppend(parent, entry_node);
    }
    return xmdsNodeLength(*parent);
}

int registerDisplayDetail_delete_last(xmdsNode **node) {
    xmdsNode *last;
    registerDisplayDetail *detail;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    detail = last->data;

    free(detail->name);
    free(detail->type);
    free(detail->value);

    free(detail);
    detail = NULL;

    return xmdsNodeFreeLast(node);

}

int registerDisplayDetail_delete(xmdsNode **node) {
    while(*node) {
        registerDisplayDetail_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *registerDisplayEntry_new() {
    xmdsNode *node;
    registerDisplayEntry *entry;

    node = xmdsNodeNew(NODE_REGISTER_DISPLAY_ENTRY);
    entry = calloc(1, sizeof(registerDisplayEntry));
    node->data = entry;

    return node;
}

/*
static void set_display_entry_value(registerDisplayEntry *dst, registerDisplayEntry src) {
    dst->status = src.status;
    dst->code = src.code;
    dst->message = src.message;
    dst->version_instructions = src.version_instructions;
    dst->details = src.details;
}

int registerDisplayEntry_add(fooNode **parent, registerDisplayEntry entry_value) {
    fooNode *entry_node;
    registerDisplayEntry *existing, *incoming;

    existing = (*parent)->data;
    if(!existing->code) {
        set_display_entry_value(existing, entry_value);
    } else {
        entry_node = requiredFilesEntry_new();
        incoming = entry_node->data;
        set_display_entry_value(incoming, entry_value);
        foonode_append(parent, entry_node);
    }
    return foonode_length(*parent);
}
*/

int registerDisplayEntry_delete_last(xmdsNode **node) {
    xmdsNode *last;
    registerDisplayEntry *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    free(entry->code);
    free(entry->message);
    free(entry->version_instructions);
    registerDisplayDetail_delete(&(entry->details));

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int registerDisplayEntry_delete(xmdsNode **node) {
    while(*node) {
        registerDisplayEntry_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *scheduleLayoutEntry_new() {
    xmdsNode *node;
    scheduleLayoutEntry *entry;

    node = xmdsNodeNew(NODE_SCHEDULE_LAYOUT);
    entry = malloc(sizeof(scheduleLayoutEntry));
    memset(entry, '\0', sizeof(scheduleLayoutEntry));
    node->data = entry;

    return node;
}

/*
static void set_schedule_layout_value(scheduleLayoutEntry *dst, scheduleLayoutEntry src) {
    dst->file = src.file;
    dst->fromDate = src.fromDate;
    dst->toDate = src.toDate;
    dst->scheduleId = src.scheduleId;
    dst->priority = src.priority;
}

int scheduleLayoutEntry_add(fooNode **parent, scheduleLayoutEntry entry_value) {
    fooNode *entry_node;
    scheduleLayoutEntry *existing;

    existing = (*parent)->data;
    if(!existing->file) {
        set_schedule_layout_value(existing, entry_value);
    } else {
        entry_node = scheduleLayoutEntry_new();
        existing = entry_node->data;
        set_schedule_layout_value(existing, entry_value);
        foonode_append(parent, entry_node);
    }
    return foonode_length(*parent);

}
*/

int scheduleLayoutEntry_delete_last(xmdsNode **node) {
    xmdsNode *last;
    scheduleLayoutEntry *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    free(entry->file);
    free(entry->fromDate);
    free(entry->toDate);
    free(entry->scheduleId);
    free(entry->dependents);

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int scheduleLayoutEntry_delete(xmdsNode **node) {
    while(*node) {
        scheduleLayoutEntry_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *scheduleDependents_new() {
    xmdsNode *node;
    scheduleDependents *entry;

    node = xmdsNodeNew(NODE_SCHEDULE_DEPS);
    entry = malloc(sizeof(scheduleDependents));
    entry->file = NULL;
    node->data = entry;

    return node;
}

static void scheduleDependents_set(scheduleDependents *dst, scheduleDependents src) {
    /*fprintf(stderr, "INFO set_schedule_deps_value() src.file: %s\n", src.file);*/
    dst->file = src.file;
}

int scheduleDependents_add(xmdsNode **parent, scheduleDependents entry_value) {
    xmdsNode *entry_node;
    scheduleDependents *existing, *incoming;

    existing = (*parent)->data;

    if(!existing->file) {
        scheduleDependents_set(existing, entry_value);
    } else {
        entry_node = scheduleDependents_new();
        incoming = entry_node->data;
        scheduleDependents_set(incoming, entry_value);
        xmdsNodeAppend(parent, entry_node);
    }
    return xmdsNodeLength(*parent);

}

int scheduleDependents_delete_last(xmdsNode **node) {
    xmdsNode *last;
    scheduleDependents *entry;

    if(!node || !(*node)) {
        return -1;
    }

    if(!isXmdsNodeType(*node, NODE_SCHEDULE_DEPS)) {
        fprintf(stderr, "ERROR scheduleDependents_delete_last() invalid node type.");
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    free(entry->file);

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int scheduleDependents_delete(xmdsNode **node) {
    while(*node) {
        scheduleDependents_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *scheduleEntry_new() {
    xmdsNode *node;
    scheduleEntry *entry;

    node = xmdsNodeNew(NODE_SCHEDULE_ENTRY);
    entry = malloc(sizeof(scheduleEntry));
    entry->defaultLayout = -99;
    entry->layouts = NULL;
    entry->dependents = NULL;
    node->data = entry;

    return node;
}

/*
static void set_schedule_entry_value(scheduleEntry *dst, scheduleEntry src) {
    dst->defaultLayout = src.defaultLayout;
    dst->layouts = src.layouts;
    dst->dependents = src.dependents;
}

int scheduleEntry_add(fooNode **parent, scheduleEntry entry_value) {
    fooNode *entry_node;
    scheduleEntry *existing, *incoming;

    existing = (*parent)->data;
    if(existing->defaultLayout < -1) {
        set_schedule_entry_value(existing, entry_value);
    } else {
        entry_node = scheduleEntry_new();
        incoming = entry_node->data;
        set_schedule_entry_value(incoming, entry_value);
        foonode_append(parent, entry_node);
    }
    return foonode_length(*parent);

}
*/

void scheduleEntry_add_layout(xmdsNode **entry_node, xmdsNode *layout_node) {
    scheduleEntry *entry;

    entry = (*entry_node)->data;
    if(!entry->layouts) {
        entry->layouts = layout_node;
    } else {
        xmdsNodeAppend(&(entry->layouts), layout_node);
    }
}


void scheduleEntry_add_deps(xmdsNode **entry_node, xmdsNode *deps_node) {
    scheduleEntry *entry;

    entry = (*entry_node)->data;
    if(!entry->dependents) {
        entry->dependents = deps_node;
    } else {
        xmdsNodeAppend(&(entry->dependents), deps_node);
    }
}

int scheduleEntry_delete_last(xmdsNode **node) {
    xmdsNode *last;
    scheduleEntry *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    scheduleLayoutEntry_delete(&(entry->layouts));
    scheduleDependents_delete(&(entry->dependents));

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int scheduleEntry_delete(xmdsNode **node) {
    while(*node) {
        scheduleEntry_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *submitLogXml_new() {
    xmdsNode *node;
    submitLogXml *entry;

    node = xmdsNodeNew(NODE_LOGXML_ENTRY);
    entry = calloc(1, sizeof(submitLogXml));
    node->data = entry;

    return node;
}

static void _logXml_set(xmdsNode *node, submitLogXml src) {
    submitLogXml *dst;

    dst = node->data;
    if(src.date) {
        node->_state |= 0x01;
        dst->date = str_duplicate(src.date);
    }
    if(src.category) {
        node->_state |= 0x02;
        dst->category = str_duplicate(src.category);
    }
    if(src.message) {
        node->_state |= 0x04;
        dst->message = str_duplicate(src.message);
    }
    if(src.method) {
        node->_state |= 0x08;
        dst->method = str_duplicate(src.method);
    }
    if(src.thread) {
        node->_state |= 0x10;
        dst->thread = str_duplicate(src.thread);
    }
    if(src.type) {
        node->_state |= 0x20;
        dst->type= str_duplicate(src.type);
    }
}

int submitLogXml_add(xmdsNode **parent, submitLogXml entry_value) {
    xmdsNode *entry_node;

    if(parent && *parent) {
        if(!(*parent)->_state) {
            _logXml_set(*parent, entry_value);
        } else {
            entry_node = submitLogXml_new();
            _logXml_set(entry_node, entry_value);
            xmdsNodeAppend(parent, entry_node);
        }
        return xmdsNodeLength(*parent);
    }
    return -1;
}

int submitLogXml_delete_last(xmdsNode **node) {
    xmdsNode *last;
    submitLogXml *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    if(last->_state & 0x01) {
        free(entry->category);
    }
    if(last->_state & 0x02) {
        free(entry->date);
    }
    if(last->_state & 0x04) {
        free(entry->message);
    }
    if(last->_state & 0x08) {
        free(entry->method);
    }
    if(last->_state & 0x10) {
        free(entry->thread);
    }
    if(last->_state & 0x20) {
        free(entry->type);
    }

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int submitLogXml_delete(xmdsNode **node) {
    while(*node) {
        submitLogXml_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *submitStatsXml_new() {
    xmdsNode *node;
    submitStatsXml *entry;

    node = xmdsNodeNew(NODE_STATXML_ENTRY);
    entry = calloc(1, sizeof(submitStatsXml));
    node->data = entry;
    node->_state = 0;

    return node;
}

static void _statXml_set(xmdsNode *node, submitStatsXml src) {
    submitStatsXml *dst;

    dst = node->data;
    if(src.type) {
        node->_state |= 0x01;
        dst->type = str_duplicate(src.type);
    }
    if(src.fromdt) {
        node->_state |= 0x02;
        dst->fromdt = str_duplicate(src.fromdt);
    }
    if(src.todt) {
        node->_state |= 0x04;
        dst->todt = str_duplicate(src.todt);
    }
    if(src.scheduleid) {
        node->_state |= 0x08;
        dst->scheduleid = str_duplicate(src.scheduleid);
    }
    if(src.mediaid) {
        node->_state |= 0x10;
        dst->mediaid = str_duplicate(src.mediaid);
    }
    if(src.tag) {
        node->_state |= 0x20;
        dst->tag= str_duplicate(src.tag);
    }
}

int submitStatsXml_add(xmdsNode **parent, submitStatsXml entry_value) {
    xmdsNode *entry_node;

    if(parent && *parent) {
        if(!(*parent)->_state) {
            _statXml_set(*parent, entry_value);
        } else {
            entry_node = submitLogXml_new();
            _statXml_set(entry_node, entry_value);
            xmdsNodeAppend(parent, entry_node);
        }
        return xmdsNodeLength(*parent);
    }
    return -1;
}

int submitStatsXml_delete_last(xmdsNode **node) {
    xmdsNode *last;
    submitStatsXml *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    if(last->_state & 0x01) {
        free(entry->type);
    }
    if(last->_state & 0x02) {
        free(entry->fromdt);
    }
    if(last->_state & 0x04) {
        free(entry->todt);
    }
    if(last->_state & 0x08) {
        free(entry->scheduleid);
    }
    if(last->_state & 0x10) {
        free(entry->mediaid);
    }
    if(last->_state & 0x20) {
        free(entry->tag);
    }

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int submitStatsXml_delete(xmdsNode **node) {
    while(*node) {
        submitStatsXml_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

xmdsNode *mediaInventoryXml_new() {
    xmdsNode *node;
    mediaInventoryXml *entry;

    node = xmdsNodeNew(NODE_MEDIA_INVENTORY_ENTRY);
    entry = calloc(1, sizeof(mediaInventoryXml));
    node->data = entry;
    node->_state = 0;

    return node;
}

static void _mediaInventory_set(xmdsNode *node, mediaInventoryXml src) {
    mediaInventoryXml *dst;

    dst = node->data;

    dst->id = src.id;
    dst->complete = src.complete;
    if(src.md5) {
        node->_state |= 0x01;
        dst->md5 = str_duplicate(src.md5);
    }
    dst->lastChecked = src.lastChecked;
}

int mediaInventoryXml_add(xmdsNode **parent, mediaInventoryXml entry_value) {
    xmdsNode *entry_node;

    if(parent && *parent) {
        if(!(*parent)->_state) {
            _mediaInventory_set(*parent, entry_value);
        } else {
            entry_node = mediaInventoryXml_new();
            _mediaInventory_set(entry_node, entry_value);
            xmdsNodeAppend(parent, entry_node);
        }
        return xmdsNodeLength(*parent);
    }
    return -1;
}

int mediaInventoryXml_delete_last(xmdsNode **node) {
    xmdsNode *last;
    mediaInventoryXml *entry;

    if(!node || !(*node)) {
        return -1;
    }

    last = (*node)->last;
    entry = last->data;

    if(last->_state & 0x01) {
        free(entry->md5);
    }

    free(entry);
    entry = NULL;

    return xmdsNodeFreeLast(node);

}

int mediaInventoryXml_delete(xmdsNode **node) {
    while(*node) {
        mediaInventoryXml_delete_last(node);
    }
    return xmdsNodeLength(*node);
}

