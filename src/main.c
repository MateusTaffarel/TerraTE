#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/utilities.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stdout, "Usage: %s <path_to_file>", argv[0]);
        exit(0);
    }

    // Read and print content

    char* content = read_content(argv[1]);
    char* updated_content;
    if (!content) {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
        return 1;
    }
    int amount_lines = get_amount_lines(content); //amount of lines starting by 1 then adding
    printf("\n");
    print_content_with_lines(content);
    printf("\n");
    
    while (1) {

        // Menu

        int selection_menu = menu("Edit line.", "Quit.");

        // If edit line selected

        if (selection_menu == 0) {
            
            // Select number
            
            printf("\n\nSelect a line to edit (Press Enter to select or ESC to quit, use up and down arrows to change line):\n[1 to %d] \n\n", amount_lines);
            int selection = select_number(1, amount_lines);
            
            while (selection != -1){ 

                // Edit line

                updated_content = edit_line(selection - 1, content);
                if (!updated_content) {
                    fprintf(stderr, "Error during editing process.\n");
                    free(content);
                    return 1;
                }

                // Open and write

                FILE* f = fopen(argv[1], "wb");
                if (f) {
                    fwrite(updated_content, strlen(updated_content), 1, f);
                    fclose(f);
                    printf("\n\nChanges saved!\n");
                }
                else fprintf(stderr, "Error writing to file.\n");
                
                // Reprint the content
                
                printf("\n");
                print_content_with_lines(updated_content);
                printf("\n");
                content = read_content(argv[1]);
                amount_lines = get_amount_lines(content); //amount of lines starting by 1 then adding
                printf("\n\nSelect a line to edit (Press Enter to select or ESC to quit, use up and down arrows to change line):\n[1 to %d] \n\n", amount_lines);
                selection = select_number(1, amount_lines);
            }
        }

        // If quit selected

        else if (selection_menu == 1) {
            exit(0);
        }
    }

    free(updated_content);
    free(content);

    return 0;
}
