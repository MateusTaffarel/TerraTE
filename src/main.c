#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/utilities.h"

int main(int argc, char* argv[]){
    if (argc < 2){
        fprintf(stdout, "Usage: %s <path_to_file>", argv[0]);
        exit(0);
    }

    // read and print content
    char* content = read_content(argv[1]);
    print_content(sizeof(content), content);
    printf("Test 1 passed\n");
    
    // amount of lines
    int amount_lines = get_amount_lines(sizeof(content), content);
    printf("%i\n", amount_lines);
    
    // seleting a line to edit
    int selection = select_number(1, amount_lines);
    char* cptr = edit_line(selection - 1, content);
    
    // editing file
    FILE* f = fopen(argv[1], "w");
    
    if (f){
        fwrite(cptr, strlen(cptr), 1, f);
        fclose(f);
        printf("\n\nSaved!");
    }
    else{
        fprintf(stderr, "Error writing to file.");
    }
}
