#ifndef STORAGE_H
#define STORAGE_H
#include "type.h"

bool storage_start(void);
Task_list* storage_read( CmdTarget t , int day_count);
void storage_write( CmdTarget t , Task_list* add_tasks , int day_count );
void storage_new_week(void);

#endif
