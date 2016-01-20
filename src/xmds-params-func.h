#ifndef XMDS_PARAMS_FUNC_H
#define XMDS_PARAMS_FUNC_H
#include "xmds-params.h"

int registerDisplayEntry_add(xmdsNode **parent, registerDisplayEntry entry_value);
int registerDisplayEntry_delete(xmdsNode **node);

xmdsNode *registerDisplayDetail_new();
int registerDisplayDetail_add(xmdsNode **parent, registerDisplayDetail detail_value);
int registerDisplayDetail_delete(xmdsNode **node);

xmdsNode *scheduleDependents_new();
int scheduleDependents_add(xmdsNode **parent, scheduleDependents entry_value);

xmdsNode *scheduleLayoutEntry_new();
int scheduleLayoutEntry_add(xmdsNode **parent, scheduleLayoutEntry entry_value);

xmdsNode *scheduleEntry_new();
int scheduleEntry_add(xmdsNode **parent, scheduleEntry entry_value);
void scheduleEntry_add_layout(xmdsNode **entry_node, xmdsNode *layout_node);
void scheduleEntry_add_deps(xmdsNode **entry_node, xmdsNode *deps_node);
int scheduleEntry_delete(xmdsNode **node);

xmdsNode *requiredFilesEntry_new();
int requiredFilesEntry_append(xmdsNode **parent, xmdsNode *child);
int requiredFilesEntry_add(xmdsNode **parent, requiredFilesEntry entry_value);
int requiredFilesEntry_delete(xmdsNode **node);

xmdsNode *submitLogXml_new();
int submitLogXml_add(xmdsNode **parent, submitLogXml log);

int submitLogXml_delete(xmdsNode **node);

xmdsNode *submitStatsXml_new();
int submitStatsXml_add(xmdsNode **parent, submitStatsXml entry_value);
int submitStatsXml_delete(xmdsNode **node);

xmdsNode *mediaInventoryXml_new();
int mediaInventoryXml_add(xmdsNode **parent, mediaInventoryXml entry_value);
int mediaInventoryXml_delete(xmdsNode **node);


#endif
