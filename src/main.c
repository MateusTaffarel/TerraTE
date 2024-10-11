#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/utilities.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stdout, "Usage: %s <path_to_file>", argv[0]);
        exit(0);
    }

    // read and print content
    /* NOTE FOR FUTURE DEVS:
     * This function doesn't read files more than or equal to 4kb (from what I tested)
     * Next implementation we should fix this
     * - Ayla
    */
    char* content = read_content(argv[1]);
    if (!content) {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
        return 1;
    }
    int amount_lines = get_amount_lines(content); //amount of lines
    print_content_with_lines(content);

    printf("Select a line to edit (1 to %d): ", amount_lines);
    int selection = select_number(1, amount_lines);

    char* updated_content = edit_line(selection - 1, content);
    if (!updated_content) {
        fprintf(stderr, "Error during editing process.\n");
        free(content);
        return 1;
    }

    FILE* f = fopen(argv[1], "wb");
    if (f) {
        fwrite(updated_content, strlen(updated_content), 1, f);
        fclose(f);
        printf("\n\nChanges saved!\n");
    } 
    else fprintf(stderr, "Error writing to file.\n");

    free(content);
    free(updated_content);

    return 0;
}
