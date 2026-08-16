#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../src/utils.h"
#include "type.h"

void test_split_string() {
    Task_list *list = split_string("Linux:Networking:Vim", ':');
    
    assert(list != NULL);
    assert(list->count == 3);
    assert(strcmp(list->task_list[0].description, "Linux") == 0);
    assert(list->task_list[0].completed == false);
    assert(strcmp(list->task_list[1].description, "Networking") == 0);
    assert(strcmp(list->task_list[2].description, "Vim") == 0);
    
    free_tasklist(list);
    printf("✓ test_split_string passed\n");
}


void test_task_to_line() {
    Task t1 = {"Linux", true};
    Task t2 = {"Networking", false};
    
    char *line1 = strdup(task_to_line(t1));
    char *line2 = strdup(task_to_line(t2));
    
    assert(strcmp(line1, "+Linux") == 0);
    assert(strcmp(line2, "-Networking") == 0);
    
    free(line1);
    free(line2);
    printf("✓ test_task_to_line passed\n");
}

void test_line_to_task() {
    Task t1 = line_to_task("+Linux");
    Task t2 = line_to_task("-Networking");
    
    assert(t1.completed == true);
    assert(strcmp(t1.description, "Linux") == 0);
    assert(t2.completed == false);
    assert(strcmp(t2.description, "Networking") == 0);
    
    free(t1.description);
    free(t2.description);
    printf("✓ test_line_to_task passed\n");
}

void test_print_functions() {
    Task_list list;
    Task tasks[3] = {
        {"Linux", true},
        {"Networking", false},
        {"Vim", true}
    };
    
    for (int i = 0; i < 3; i++) {
        list.task_list[i] = tasks[i];
    }
    list.count = 3;
    
    printf("--- print_tasks output ---\n");
    print_tasks(&list);
    printf("--- print_progressbar output ---\n");
    print_progressbar(&list);
    printf("✓ test_print_functions passed (visual check)\n");
}

int main() {
    test_split_string();
    test_task_to_line();
    test_line_to_task();
    test_print_functions();
    printf("\nAll tests passed!\n");
    return 0;
}