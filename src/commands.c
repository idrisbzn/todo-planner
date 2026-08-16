#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "storage.h"
#include "utils.h"
#include "state.h"


bool exec_cmd(ParsedCmd *cmd) {
    if (cmd->target == TARGET_START) {
        if( storage_start() ){
            printf("started new goals successfully!!\n");
            return true;
        }
        else{
            return false;
        }
    }

    else if (cmd->target == TARGET_NEXT) {
        Position p = state_read();
        
        Task_list *day_list = storage_read(TARGET_DAY, p.day);
        remove_completed(day_list);
        
        if (p.day < 7) {
            p.day++;
            state_write(p);
            if (day_list->count > 0) {
                storage_write(TARGET_DAY, day_list, p.day);
            }
        } else {
            Task_list *week_list = storage_read(TARGET_WEEK, 0);
            remove_completed(week_list);
            
            storage_new_week();
            
            p.week++;
            p.day = 1;
            
            if (day_list->count > 0) {
                storage_write(TARGET_DAY, day_list, p.day);
            }
            if (week_list->count > 0) {
                storage_write(TARGET_WEEK, week_list, p.week);
            }
            free_tasklist(week_list);
        }
        
        free_tasklist(day_list);
        printf("moved to next day successfully!!\n");
        return true;
    }
    else{
        Task_list *list = storage_read(cmd->target,cmd->count);
        if( cmd -> act == ACTION_LIST ){
            print_tasks(list);
            print_progressbar(list);
            free_tasklist(list);
            return true;
        }
        if( cmd -> act == ACTION_ADD ){
            if( list -> count + (cmd -> add_tasks -> count) > 20 ){
                printf("cannot add tasks \n maxtasks %d\n",CAPACITY);
                free_tasklist(list);
                free_tasklist(cmd->add_tasks);
                return false;
            }
            else{
                for( int i = 0 ; i < cmd ->add_tasks->count ; i++ ){
                    list -> task_list[ list->count ] = cmd->add_tasks->task_list[i];
                    list -> count += 1;
                }
                storage_write(cmd->target,list,cmd->count);
                free_tasklist(list);
                free(cmd -> add_tasks); 
                return true;
            }
        }
        if( cmd -> act == ACTION_DONE ){
            int i = 0 ;
            while( cmd -> ids[i] != -1 ) {
                if( cmd -> ids[i] - 1 < list -> count ){
                    list -> task_list[ cmd->ids[i]-1 ].completed = true;
                    printf(" the task with id : %d is done successfully!!\n",cmd->ids[i]);
                }
                else {
                    printf( " the id : %d is not availlable \n",cmd -> ids[i]);
                }
                i++;
            }
            storage_write(cmd->target, list, cmd->count);
            free_tasklist(list);
            return true;
        }
        if( cmd -> act == ACTION_REMOVE ){
            remove_tasks_by_ids(list, cmd->ids);
            storage_write(cmd->target, list, cmd->count);
            free_tasklist(list);
            return true;
        }
        return false;
    }

}