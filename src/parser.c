#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "utils.h"
#include "state.h"

ParsedCmd* parse_cmd(int argc, char* argv[]) {
    if (argc < 2) {
        printf("command not valid!!\n");
        return NULL;
    }

    ParsedCmd *c = (ParsedCmd*)malloc(sizeof(ParsedCmd));
    memset(c, 0, sizeof(ParsedCmd));

    if (strcmp(argv[1], "goals") == 0) {
        c->target = TARGET_GOALS;
    } else if (strcmp(argv[1], "week") == 0) {
        c->target = TARGET_WEEK;
    } else if (strncmp(argv[1], "week", 4) == 0 && strlen(argv[1]) > 4) {
        c->target = TARGET_WEEK;
        c->count = atoi(argv[1] + 4);
    } else if (strcmp(argv[1], "day") == 0) {
        c->target = TARGET_DAY;
        c->count = (state_read()).day;
    } else if (strncmp(argv[1], "day", 3) == 0 && strlen(argv[1]) > 3) {
        c->target = TARGET_DAY;
        c->count = atoi(argv[1] + 3);
    } else if (strcmp(argv[1], "start") == 0) {
        c->target = TARGET_START;
    } else if (strcmp(argv[1], "next") == 0) {
        c->target = TARGET_NEXT;
    } else {
        printf("Unknown command: %s\n", argv[1]);
        free(c);
        return NULL;
    }

    if (c->target == TARGET_START || c->target == TARGET_NEXT) {
        return c;
    }

    if (argc < 3) {
        printf("command not valid!!\n");
        free(c);
        return NULL;
    }

    if (strcmp(argv[2], "list") == 0) {
        c->act = ACTION_LIST;
        return c;
    } else if (strcmp(argv[2], "add") == 0) {
        c->act = ACTION_ADD;
        if (argc < 4) {
            printf("missing task text\n");
            free(c);
            return NULL;
        }
        c -> add_tasks = split_string(argv[3], ':');
        return c;
    } else if (strcmp(argv[2], "done") == 0 || strcmp(argv[2], "remove") == 0) {
        if (strcmp(argv[2], "done") == 0) {
            c->act = ACTION_DONE;
        } else {
            c->act = ACTION_REMOVE;
        }
        if (argc < 4) {
            printf("missing task IDs\n");
            free(c);
            return NULL;
        }
        int j = 0;
        for (int i = 3; i < argc && j < CAPACITY + 1 ; i++) {
            c->ids[j] = atoi(argv[i]);
            j++;
        }
        c->ids[j] = -1;  
        return c;
    } else {
        printf("Unknown action: %s\n", argv[2]);
        free(c);
        return NULL;
    }
}