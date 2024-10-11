#include "./utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


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
    int c;
    int i = min_line;

    printf("\rSelect a line to edit (Press Enter to select, use up and down arrows to change line):\n"); // Add a prompt for context
    printf("\r%c %i", 0x10, i); // Display the current selection

    while ((c = _getch()) != 13) {
        switch(c) {
            case 80:
                if (i > min_line) i--; // Prevent selection from going below min_line
                break;
            case 72:
                if (i < max_line) i++; // Prevent selection from going above max_line
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

int menu(char* option_1, char* option_2){
    int c;
    int selected_option_ptr = 0;

    printf("\rSelect a menu option (Press Enter to select, use right and left arrows to change option):\n");
    printf("\r*%s          %s", option_1, option_2);

    while((c = _getch()) != 13) {
        switch (c) {
            case 75:
                if (selected_option_ptr > 0){
                    selected_option_ptr--;
                    printf("\r"); // Move cursor to the start of the line
                    printf("                    "); // Clear the line by printing
                    printf("\r*%s          %s", option_1, option_2);
                }
                break;
            case 77:
                if (selected_option_ptr < 1){
                    selected_option_ptr++;
                    printf("\r"); // Move cursor to the start of the line
                    printf("                    "); // Clear the line by printing spaces
                    printf("\r%s          *%s", option_1, option_2);
                }
                break;
            default:
                continue;
        }
    }
    return selected_option_ptr;
}

// EDITS A LINE 
char* edit_line(int line, char* content) {
    char* line_start = content;
    char* line_end = NULL;
    int curr_line = 0;

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
    if (!line_end) line_end = content + strlen(content); // If last line, end is the end of the content


    // calculate the length of new content
    size_t prefix_length = line_start - content;
    size_t new_line_length;
    char new_line[1024];

    printf("\n\nEnter new content for line %d >> ", line + 1);
    fget_line(NULL, new_line, sizeof(new_line));
    new_line_length = strlen(new_line);

    size_t total_new_content_size = prefix_length + new_line_length + 1 + (content + strlen(content) - line_end);

    char* new_content = (char*)malloc(total_new_content_size);
    if (!new_content) {
        fprintf(stderr, "Memory allocation failed.\n");
        return content;
    }

    // Copy the parts into the new content buffer
    strncpy(new_content, content, prefix_length); // Copy the content before the line
    strcpy(new_content + prefix_length, new_line); // Add the new line content
    new_content[prefix_length + new_line_length] = '\n'; // Add newline after the new line content
    strcpy(new_content + prefix_length + new_line_length + 1, line_end); // Add the content after the line

    free(content);

    return new_content;
}

// GETS THE AMOUNT OF LINES (Working)
int get_amount_lines(const char* content){
    int total_lines = 0;
    size_t content_length = strlen(content);

    for (int i = 0; i < content_length; i++) {
        if (content[i] == '\n'){ 
            total_lines++;
        }
    }
    return total_lines;
}

// PRINTS CONTENTS (Working)
void print_content(char* content){
    int i = get_amount_lines(content);
    char* cptr = strtok(content, "\n");
    for (int token = 0; token < i; token++){
        printf("%i - %s\n", token, cptr);
        cptr = strtok(NULL, "\n");
    }
}

void print_content_with_lines(const char* content) {
    int line_number = 1;
    const char* line_start = content;

    for (const char* ptr = content; *ptr != '\0'; ++ptr) {
        if (*ptr == '\n') {
            printf("%d: ", line_number);
            fwrite(line_start, 1, ptr - line_start + 1, stdout); //interesting...
            line_number++;

            line_start = ptr + 1;
        }
    }

    // Handle any remaining content after the last newline
    if (*line_start != '\0') {
        printf("%d: %s", line_number, line_start);
    }
}

// So I thought about checking the index of the string and if it finds a newline, it will print the sum of all
// newlines (if encounters new line = newlineit++) encountered before (+ 1 so we actually have line 1 as 1 and not 0) ant then print the string;
