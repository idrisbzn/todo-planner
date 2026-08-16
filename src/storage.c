#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage.h"
#include "utils.h"
#include "state.h"

bool storage_archive() {
    char archive_name[128];
    sprintf(archive_name, "%s/%ld", build_archive_dir(), time(NULL));

    if (rename(build_current_dir(), archive_name) == -1) {
        fprintf(stderr, "failed to move current folder to archive\n");
        return false;
    }
    return true;
}


bool storage_start(){
    mkdir(build_todo_dir(),0744);
    mkdir(build_archive_dir(),0744);
    int k = mkdir(build_current_dir(),0744);
    if( k == -1 ){
        bool r = storage_archive();
        if(r == false ){
            fprintf(stderr, "failed to archive current folder, try again\n");
            return false;
        }
        mkdir(build_current_dir(),0744);
    }
    state_write((Position){1,1});
    mkdir(build_week_dir(1),0744);
    FILE *f = fopen(build_goals_path(), "w");
    if( f == NULL ){
        fprintf(stderr, "failed to create goal file , try again\n");
        return false;
    }
    fclose(f);
    f = fopen(build_week_path(1),"w");
    if( f == NULL ){
        fprintf(stderr, "failed to create week file , try again\n");
        return false;
    }
    fclose(f);
    for ( int i = 1 ; i < 8 ; i++ ){
        f = fopen(build_day_path(1,i),"w");
        if( f == NULL ){
            fprintf(stderr, "failed to create day file , try again\n");
            return false;
        }
        fclose(f);
    }
    return true;
}

void storage_write( CmdTarget t , Task_list* add_tasks , int day_count ){
    FILE *f;
    switch(t){
        case TARGET_GOALS : 
            f = fopen(build_goals_path(),"w");
            break;
        case TARGET_WEEK :{
            Position p = state_read();
            f = fopen(build_week_path(p.week),"w");
            break;
        } 

        case TARGET_DAY :{
            Position p = state_read();
            f = fopen(build_day_path(p.week,day_count),"w");
            break;
        } 

        default : 
            fprintf(stderr, "error!! cannot write into a file\n");
            return;
    }
    if( f == NULL ){
        fprintf(stderr, "error!! cannot write into a file\n");
        return;
    }
    for( int i = 0 ; i < add_tasks -> count ; i++ ){
        Task t = add_tasks -> task_list[i];
        char *buff = task_to_line(t);
        fprintf(f,"%s\n",buff);
    }
    fclose(f);
}

Task_list* storage_read( CmdTarget t , int day_count){
    FILE *f;
    switch(t){
        case TARGET_GOALS : 
            f = fopen(build_goals_path(),"r");
            break;
        case TARGET_WEEK :{
            Position p = state_read();
            f = fopen(build_week_path(p.week),"r");
            break;
        } 

        case TARGET_DAY :{
            Position p = state_read();
            f = fopen(build_day_path(p.week,day_count),"r");
            break;
        } 

        default : 
            fprintf(stderr, "error!! cannot read the file\n");
            return NULL;
    }
    if( f == NULL ){
        fprintf(stderr, "error!! cannot read the file\n");
        return NULL;
    }
    Task_list *list = (Task_list*)malloc(sizeof(Task_list));
    list -> count = 0 ;

    char buff[256];
    while(fgets(buff,256,f) != NULL ){
        buff[strcspn(buff,"\n")] = '\0';
        Task t = line_to_task(buff);
        list-> task_list[list->count] = t;
        list-> count += 1;
    }
    fclose(f);
    return list;
}

void storage_new_week(void){

    Position p = state_read();
    p.week += 1;
    p.day = 1;
    if( mkdir(build_week_dir(p.week),0744) == -1 ){
        fprintf(stderr,"error creating week folder !! try again\n");
        return;
    }

    FILE *f ;
    f = fopen(build_week_path(p.week),"w");
    if( f == NULL ){
        fprintf(stderr, "failed to create week file , try again\n");
        return;
    }
    fclose(f);

    for ( int i = 1 ; i < 8 ; i++ ){
        f = fopen(build_day_path(p.week,i),"w");
        if( f == NULL ){
            fprintf(stderr, "failed to create day file , try again\n");
            return;
        }
        fclose(f);
    }
    state_write(p);
}