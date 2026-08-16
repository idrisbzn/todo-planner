#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>


// building path functions 

const char* get_home_path() {
    static const char* home = NULL;
    if (!home) home = getenv("HOME");
    return home;                      
}
const char* build_todo_dir(){
    static char path[64];
    sprintf(path,"%s/.todo",get_home_path());
    return path;
}
const char* build_current_dir(){
    static char path[64];
    sprintf(path,"%s/.todo/current",get_home_path());
    return path;
}
const char* build_day_path( int week , int day ){
    static char path[64];
    sprintf(path,"%s/.todo/current/week%d/day%d.todo",get_home_path(),week,day);
    return path;
}

const char* build_week_path( int week ){
    static char path[64];
    sprintf(path,"%s/.todo/current/week%d/week.todo",get_home_path(),week);
    return path;
}

const char* build_goals_path(){
    static char path[64];
    sprintf(path,"%s/.todo/current/goals.todo",get_home_path());
    return path;
}

const char* build_state_path(){
    static char path[64];
    sprintf(path,"%s/.todo/current/state",get_home_path());
    return path;
}

const char* build_archive_dir(){
    static char path[64];
    sprintf(path,"%s/.todo/archive",get_home_path());
    return path;
}

const char* build_week_dir( int week ){
    static char path[64];
    sprintf(path,"%s/.todo/current/week%d",get_home_path(),week);
    return path;
}

// string functions

Task_list* split_string(const char *str , char delim){
    int i = 0 ; 
    Task_list* list = (Task_list*)malloc( sizeof(Task_list) );
    list -> count = 0;
    char temp[64];
    int i_temp = 0;
    while( str[i] != '\0' ){
        if( str[i] != delim ){
            temp[i_temp] = str[i];
            i_temp++;
        }
        else{
            if( i_temp != 0 ){
                if( list -> count < 20 ){
                    temp[i_temp] = '\0';
                    list -> task_list[list->count].description = strdup(temp);
                    list -> task_list[list->count].completed = false;
                    list -> count += 1;
                    i_temp = 0;
                }
            }
        }
        i++;
    }
    if ( i_temp != 0 ){
        if( list -> count < 20 ){
            temp[i_temp] = '\0';
            list -> task_list[list->count].description = strdup(temp);
            list -> task_list[list->count].completed = false;
            list -> count += 1;
            i_temp = 0;
        }
    }
    return list;
}

void free_tasklist(Task_list *list){
    for ( int i = 0 ; i < list->count ; i++ ){
        free( list->task_list[i].description );
    }
    free(list);
}


// display functions 

void print_progressbar( Task_list* list ){
    int progress_bar=0;
    float percentage=0;
    if( list->count != 0 ){
        int num_of_completed_tasks = 0 ;
        for ( int i = 0 ; i < list->count ; i++ ){
            if( list->task_list[i].completed == true ){
                num_of_completed_tasks++;
            }
        }
        progress_bar = (num_of_completed_tasks*NUM_HASHES)/list->count;    
        percentage = ((float)num_of_completed_tasks/list->count) * 100;
    }
    printf("\n");
    printf("|");
    for( int i = 0 ; i < NUM_HASHES ; i++ ){
        if( progress_bar > 0 ){
            printf("#");
            progress_bar--;
        }
        else{
            printf("_");
        }
    }
    printf("|");
    printf(" %.2f %% \n",percentage);
    
}

void print_tasks(Task_list* list) {
    printf("\n");
    if (list->count == 0) {
        printf("No tasks.\n");
        return;
    }
    for (int i = 0; i < list->count; i++) {
        char mark = list->task_list[i].completed ? 'x' :  ' ';
        printf("  [%c] %s\n", mark, list->task_list[i].description);
    }
}

// conversion task/line(storage) 

char* task_to_line(Task task) {
    static char line[128];
    char prefix = task.completed ? '+' : '-';
    sprintf(line, "%c%s", prefix, task.description);
    return line;
}

Task line_to_task(char* line) {
    Task task;
    task.completed = (line[0] == '+');
    task.description = strdup(line + 1);
    return task;
}

// task list function 

void remove_completed(Task_list *list) {
    int left = 0, right = list->count - 1;

    while (left < right) {
        while (left < right && !list->task_list[left].completed) left++;
        while (left < right && list->task_list[right].completed) right--;
        if (left < right) {
            Task temp = list->task_list[left];
            list->task_list[left] = list->task_list[right];
            list->task_list[right] = temp;
            left++;
            right--;
        }
    }

    right = list->count - 1;
    while (right >= 0 && list->task_list[right].completed) {
        free(list->task_list[right].description);
        list->count--;
        right--;
    }
}


void remove_tasks_by_ids(Task_list *list, int *ids) {
    for (int i = 0; ids[i] != -1; i++) {
        int idx = ids[i] - 1;
        if (idx >= 0 && idx < list->count && list->task_list[idx].description != NULL) {
            free(list->task_list[idx].description);
            list->task_list[idx].description = NULL;
        }
    }
    
    int new_count = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->task_list[i].description != NULL) {
            if (new_count != i) {
                list->task_list[new_count] = list->task_list[i];
            }
            new_count++;
        }
    }
    list->count = new_count;
}