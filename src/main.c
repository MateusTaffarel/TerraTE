#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/utilities.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stdout, "Usage: %s <path_to_file>", argv[0]);
        exit(0);
    }

    set_vtp_mode();

    printf("TerraTerminalEditor\n"
            "Copyright (C) 2024  Mateus Taffarel\n\n"
            "This program is free software: you can redistribute it and/or modify\n"
            "it under the terms of the GNU Affero General Public License as published\n"
            "by the Free Software Foundation, either version 3 of the License, or\n"
            "(at your option) any later version.\n"
            "This program is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU Affero General Public License for more details.\n\n"
            "You should have received a copy of the GNU Affero General Public License\n"
            "along with this program.  If not, see <https://www.gnu.org/licenses/>\n\n");

    // read and print content

    char* content = read_content(argv[1]);
    char* updated_content;
    if (!content) {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
        return 1;
    }
    int amount_lines = get_amount_lines(content); //amount of lines starting by 1 then adding
    printf("\n");
    write_content_with_lines(content, argv[1]);
    printf("\n");
    
    while (1) {

        // Menu

        int selection_menu = menu(3, "Edit line.", "Add lines.", "Quit.");

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

                // Refresh the editor content
                content = read_content(argv[1]);
                amount_lines = get_amount_lines(content); //amount of lines starting by 1 then adding

                refresh_editor(content, argv[1]); // Clear and reprint editor content

                printf("\n\nSelect a line to edit (Press Enter to select or ESC to quit, use up and down arrows to change line):\n[1 to %d] \n\n", amount_lines);
                selection = select_number(1, amount_lines);
            }

            refresh_editor(content, argv[1]); // Refresh if ESC

        }

        else if (selection_menu == 1) {
            
            // Create the selection for new lines
            printf("\n\nSelect a line to edit (Press Enter to select or ESC to quit, use up and down arrows to change line):\n[1 to %d] \n\n", amount_lines);
            int selection = select_number(1, amount_lines);

            if (selection != -1){

                // Updated content is new lines

                    updated_content = add_lines(selection - 1, content);
                    if (!updated_content) {
                            fprintf(stderr, "Error during editing process.\n");
                            free(content);
                            return 1;
                        }

                    // Open and write to file

                    FILE* f = fopen(argv[1], "wb");
                    if (f) {
                        fwrite(updated_content, strlen(updated_content), 1, f);
                        fclose(f);
                        printf("\n\nChanges saved!\n");
                    }
                    else fprintf(stderr, "Error writing to file.\n");
                    
                    // Reprint the content
                    
                    content = read_content(argv[1]);
                    amount_lines = get_amount_lines(content); //amount of lines starting by 1 then adding

                    refresh_editor(content, argv[1]); // Clear and reprint editor content
            }

            refresh_editor(content, argv[1]); // Refresh if ESC

        }

        else if (selection_menu == 2){
            exit(0);
        }
    }

    free(updated_content);
    free(content);

    return 0;
}
