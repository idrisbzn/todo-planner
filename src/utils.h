#ifndef UTILS_H
#define UTILS_H
#include "type.h"


// building bath functions 

const char* get_home_path(void);
const char* build_todo_dir(void);
const char* build_current_dir(void);
const char* build_day_path(int week,int day);
const char* build_week_path( int week );
const char* build_goals_path(void);
const char* build_state_path(void);
const char* build_archive_dir(void);
const char* build_week_dir(int week);

// string functions 

Task_list* split_string(const char *str , char delim);
void free_tasklist(Task_list *list);

// task conversion 
char* task_to_line( Task task );
Task line_to_task ( char* line);

// display 

void print_progressbar( Task_list* list );
void print_tasks( Task_list* list);
// task list functions 

void remove_completed(Task_list *list);
void remove_tasks_by_ids(Task_list *list, int *ids);


#endif