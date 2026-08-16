#include <stdio.h>
#include "state.h"
#include "utils.h"

void state_write(Position p){
    FILE *f = fopen( build_state_path(),"w");
    if( f == NULL ){
        fprintf(stderr, "failed to open state file, try again\n");
        return;
    }
        
    fprintf(f,"Week = %d\nDay = %d\n",p.week,p.day);
    fclose(f);
}

Position state_read(){
    FILE *f = fopen( build_state_path(),"r" );
    Position p={0,0};
    if( f == NULL ){
        fprintf(stderr, "failed to open state file, try again\n");
        return p;
    }
 
    char buffer[32];
    fgets(buffer,32,f);
    sscanf(buffer,"Week = %d",&(p.week));
    fgets(buffer,32,f);
    sscanf(buffer,"Day = %d",&(p.day));
    fclose(f);
    return p;
}