#include "./utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32

HANDLE sHandle;
DWORD dwMode;

int set_vtp_mode(){
    sHandle = GetStdHandle(STD_OUTPUT_HANDLE);  // Set the handle to output
    dwMode = 0; // Set the mode to 0 (normal)
    GetConsoleMode(sHandle, &dwMode); // Get the console handle (stdout) and the mode (0)
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // Set the mode to ENABLE_VIRTUAL_TERMINAL_PROCESSING
    int i = SetConsoleMode(sHandle, dwMode); // Set the console mode to ENABLE_VIRTUAL_TERMINAL_PROCESSING
    return i;
}

#endif

int fget_line(const char* prompt, char* buffer, int buffer_length) {
    if (prompt != NULL) printf("%s", prompt);

    if (fgets(buffer, buffer_length, stdin) == NULL) {
        fprintf(stderr, "Cannot get input.\n");
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return 0;
}

char* read_content(const char* File_path) { // Changed to const char* for better practice
    FILE* f = fopen(File_path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", File_path); // Added the file path to the error message
        return NULL;
    }

    size_t size = 1028; // Initial size
    size_t total_size = 0;
    char* content = malloc(size);
    if (!content) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(f);
        return NULL;
    }

    size_t instance;

    // Look into this

    while ((instance = fread(content + total_size, 1, size - total_size, f)) > 0) { // Read bytes //added size - total_size to know how many bytes to really read per iteration
        total_size += instance; // Update total size
        // Ensure we can double the size safely
        if (total_size + size > size) {
            size *= 2; // Double the size
            char* temp = realloc(content, size);
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed.\n");
                free(content);
                fclose(f);
                return NULL; // Important to return NULL on failure
            }
            content = temp;
        }
    }

    if (ferror(f)) {
        fprintf(stderr, "Error reading file: %s\n", File_path);
        free(content);
        fclose(f);
        return NULL;
    }

    content[total_size] = '\0'; // Null-terminate the string
    fclose(f);
    return content;
}

// SELECTS A NUMBER (Working)
int select_number(int min_line, int max_line){
    bool quit = false;
    int c;
    int i = min_line;

    printf("\r%c %i", 0x10, i); // Display the current selection

    while ((c = _getch()) != 13 && quit != true) {
        switch(c) {
            case 80:
                if (i > min_line) i--; // Prevent selection from going below min_line
                break;
            case 72:
                if (i < max_line) i++; // Prevent selection from going above max_line
                break;
            case 27:
                quit = true;
                return -1;
                break;
            default: 
                continue;
        }

        // Clear the current line before displaying the new selection
        printf("\r"); // Move cursor to the start of the line
        printf("                    "); // Clear the line by printing spaces
        printf("\r%c %i", 0x10, i); // Print the new selection
    }
    return i;
}

void refresh_editor(const char* content, const char* path) {
    // Move cursor to the top-left (assuming that's where the editor content starts)
    printf("\033[H");
    // Clear everything
    printf("\033[J");
    printf("\033[3J"); // This clears what we CANNOT currently see (clears the scroll)
    // Reprint the content with line numbers
    write_content_with_lines(content, path);
}

void print_menu(char* options[], int selected_option, int num_options) {
    printf("\r          ");
    for (int i = 0; i < num_options; i++) {
        if (i == selected_option) {
            printf("\033[1;4;36m[%s]\033[0m        ", options[i]); // Highlight selected option
        } else {
            printf("%s       ", options[i]); // Regular option
        }
    }
    fflush(stdout); // Ensure output is printed immediately
}

int menu(int num_options, ...) {
    // Dynamically allocate memory for the options
    char** options = malloc(num_options * sizeof(char*));
    if (!options) {
        fprintf(stderr, "Memory allocation failed.\n");
        return -1; //indicate an error
    }

    va_list args; // Initialize argument list
    va_start(args, num_options); // Start processing arguments

    // Store each argument in the options array
    for (int i = 0; i < num_options; i++) {
        options[i] = va_arg(args, char*);
    }

    va_end(args); // Clean up the argument list

    int c;
    int selected_option = 0; // Start at the first option

    print_menu(options, selected_option, num_options); // Initial display

    while ((c = _getch()) != 13) { // Wait for Enter key
        switch (c) {
            case 75: // Left arrow key
                if (selected_option > 0) {
                    selected_option--; // Move left
                }
                break;
            case 77: // Right arrow key
                if (selected_option < num_options - 1) {
                    selected_option++; // Move right
                }
                break;
            default:
                continue; // Ignore other keys
        }
        print_menu(options, selected_option, num_options); // Update display
    }
    // Clean pointers
    options = NULL;

    // Clean up dynamically allocated memory
    free(options);

    return selected_option; // Return selected option index
}

// EDITS A LINE 
char* edit_line(int line, char* content) {
    char* line_start = content;
    char* line_end = NULL;
    int curr_line = 0;
    bool last_line = false;

    while (curr_line < line && line_start) {
        line_start = strchr(line_start, '\n');
        if (line_start) line_start++;
        curr_line++;
    }

    if (!line_start) {
        fprintf(stderr, "Invalid line number.\n");
        return content;
    }

    line_end = strchr(line_start, '\n'); // Find the end of the selected line
    if (!line_end){
        line_end = content + strlen(content);
        last_line = true;
    } // If last line, end is the end of the content


    // calculate the length of new content
    size_t prefix_length = line_start - content; // Line start - content = content before new string
    size_t new_line_length; // Length of new string
    char new_line[1024]; // Buffer for new string

    printf("\n\nEnter new content for line %d >> ", line + 1);
    fget_line(NULL, new_line, sizeof(new_line));
    new_line_length = strlen(new_line);

    size_t total_new_content_size = prefix_length + new_line_length + 1 + (content + strlen(content) - line_end);

    /* Total_new_content_size is the result of the prefix length (before new content) + the length of the new content
    * + 1 for the \n or \0 terminator, and content pointer (start of content) + the length of content (size of content) - line end pointer 
    * this will give the value of the end of the new content, so we can copy the contents of new_line to new_content
    */

    char* new_content = (char*)malloc(total_new_content_size);
    if (!new_content) {
        fprintf(stderr, "Memory allocation failed.\n");
        return content;
    }

    // Copy the parts into the new content buffer
    strncpy(new_content, content, prefix_length); // Copy the content before the line
    strcpy(new_content + prefix_length, new_line); // Add the new line content
    new_content[prefix_length + new_line_length] = '\n'; // Add newline after the new line content

    // Check if it is the end of line

    if (last_line == false) strcpy(new_content + prefix_length + new_line_length + 1, line_end + 1); // Add the content after the line
    if (last_line == true) strcpy(new_content + prefix_length + new_line_length + 1, line_end);
    
    // Clean memory
    free(content);

    return new_content;
}

char* add_lines(int line, char* content) {
    if (!content || line < 0) {
        fprintf(stderr, "Invalid input.\n");
        return NULL;
    }

    char* line_start = content;
    char* line_end = NULL;
    char* new_line = "\n";
    int curr_line = 0;

    // Find the start of the specified line
    while (curr_line < line && line_start) {
        line_start = strchr(line_start, '\n');
        if (line_start) {
            line_start++;  // Move past the newline character
        }
        curr_line++;
    }

    if (!line_start) {
        fprintf(stderr, "Invalid line number.\n");
        return NULL;
    }

    line_end = strchr(line_start, '\n'); // Find the end of the selected line
    if (!line_end) {
        line_end = content + strlen(content); // Point to the end of the content
    }

    // Get the number of lines to add from the user
    printf("\n\nSelect how many lines to add (Press Enter to select or ESC to quit, use up and down arrows to change amount):\n[1 to 25]\n\n ");
    int selection_amount_lines = select_number(1, 25);
    
    // Allocate space for the new content
    size_t prefix_length = line_end - content; // Length before the new content
    size_t new_lines_length = selection_amount_lines * strlen(new_line); // Total length of new lines
    size_t total_new_content_size = prefix_length + new_lines_length + (content + strlen(content) - line_end) + 1; // +1 for null terminator
    
    char* new_content = (char*)malloc(total_new_content_size);
    
    if (!new_content) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Copy the parts into the new content buffer
    strncpy(new_content, content, prefix_length); // Copy content before the line
    char* new_content_position = new_content + prefix_length;

    // Add new lines
    for (int i = 0; i < selection_amount_lines; i++) {
        strcpy(new_content_position, new_line); // Use strcpy since new_line is constant
        new_content_position += strlen(new_line); // Move the position pointer forward
    }

    // Copy the remaining content after the specified line
    strcpy(new_content_position, line_end); // Add the content after the line

    // Clean memory
    free(content);

    return new_content; // Return the new content
}

// GETS THE AMOUNT OF LINES (Working)
int get_amount_lines(const char* content) {
    int line_number = 1;

    for (const char* ptr = content; *ptr != '\0'; ++ptr) {
        if (*ptr == '\n') {
            line_number++;
        }
    }

    // Handle any remaining content after the last newline
    return line_number;
}

// PRINTS CONTENTS (Working)
void write_content_with_lines(const char* content, const char* path) {
    printf("\n\n[\033[1;4;36m\\%s\033[0m]-----\n\n", path); // Header
    int line_number = 1;
    const char* line_start = content;

    for (const char* ptr = content; *ptr != '\0'; ++ptr) {
        if (*ptr == '\n') {
            printf("\033[32m%d\033[0m: ", line_number);
            fwrite(line_start, 1, ptr - line_start + 1, stdout); //interesting...
            line_number++;
            line_start = ptr + 1;
        }
    }

    // Handle any remaining content after the last newline
    if (*line_start != '\0') {
        printf("\033[32m%d\033[0m: %s", line_number, line_start);
    }
        printf("\n\n-----\n\n"); // End
}

// We need to create an add_line function to be honest // Done! :3