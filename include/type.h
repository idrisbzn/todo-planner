#ifndef TYPES_H 
#define TYPES_H 
#define CAPACITY 20 
#define NUM_HASHES 20
#include <stdbool.h> 
typedef enum CmdTarget{
	TARGET_START,
  	TARGET_NEXT,
	TARGET_GOALS,
	TARGET_WEEK,
	TARGET_DAY
}CmdTarget;

typedef enum Action{
	ACTION_LIST,
	ACTION_ADD,
	ACTION_REMOVE,
	ACTION_DONE
}Action;
typedef struct Position{
    int week;
    int day;
}Position;

typedef struct Task{
	char* description;
	bool completed;
}Task;

typedef struct Task_list{
	Task task_list[CAPACITY];
	int count;
}Task_list;

typedef struct ParsedCmd{
	CmdTarget target;
	int count;
	Action act;
	int ids[CAPACITY + 1];
	Task_list* add_tasks;
}ParsedCmd;

#endif
