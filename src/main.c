#define _POSIX_C_SOURCE 200809L 
#include "parser.h"
#include "commands.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    ParsedCmd *cmd = parse_cmd(argc, argv);
    if (cmd == NULL) {
        return 1;
    }

    bool success = exec_cmd(cmd);
    
    free(cmd);
    
    return success ? 0 : 1;
}