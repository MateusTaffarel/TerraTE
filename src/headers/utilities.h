#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>

// TYPINGS (not used currently)

typedef char* str;
typedef int* vec;

typedef float FLOAT32;
typedef double FLOAT64;

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint64_t QWORD;


/**
 * Sets the console mode to Virtual Terminal Processing.
  * @return {int} If the function succeeds, the return value is nonzero.
 */
int set_vtp_mode();

/**
 * Prompts the user for a line of input and reads it into a buffer.
 *
 * @param {const char*} prompt - A string to display as a prompt. If NULL, no prompt is displayed.
 * @param {char*} buffer - The buffer to store the input.
 * @param {int} buffer_length - The length of the buffer.
 * @return {int} Returns 0 on success, 1 on error.
 */
int fget_line(const char* prompt, char* buffer, int buffer_length);

/**
 * Reads the entire content of a file into a dynamically allocated string.
 *
 * @param {const char*} File_path - The path to the file to be read.
 * @return {char*} Returns a pointer to the content on success, or NULL on error.
 */
char* read_content(const char* File_path);

/**
 * Prompts the user to select a line number using keyboard input.
 *
 * @param {int} min_line - The minimum line number that can be selected.
 * @param {int} max_line - The maximum line number that can be selected.
 * @return {int} Returns the selected line number.
 */
int select_number(int min_line, int max_line);

/**
 * Refreshes the screen.
 * @param {const char*} content - The content to be refreshed.
 * @param {const char*} path - The current file path.
 */
void refresh_editor(const char* content, const char* path);

/**
 * Prints the menu.
 *
 * @param {char*} options - The options to be used.
 * @param {int} selected_option - The selected option.
  * @param {int} num_options - The amount of options.
 */
void print_menu(char* options[], int selected_option, int num_options);

/**
 * Prompts the user to select an option using keyboard input.
 *
 * @param {int} num_options - The amount of options that can be selected.
 * @return {int} Returns the selected option.
 */
int menu(int num_options, ...);

/**
 * Edits a specific line in the content and returns the updated content.
 *
 * @param {int} line - The line number to edit (0-indexed).
 * @param {char*} content - The original content as a string.
 * @return {char*} Returns the updated content, or the original content on error.
 */
char* edit_line(int line, char* content);

/* Adds lines from a specific point and returns the updated content.
 *
 * @param {int} line - The line number to edit (0-indexed).
 * @param {int} lines_add - The amount of lines to add.
 * @return {char*} Returns the updated content, or the original content on error.
 */
char* add_lines(int line, char* content);

/**
 * Counts the number of lines in the content.
 *
 * @param {const char*} Content - The content string to analyze.
 * @return {int} Returns the total number of lines.
 */
int get_amount_lines(const char* Content);

/**
 * Prints the content line by line with line numbers.
 *
 * @param {char*} Content - The content string to print.
 */
void print_content(char* Content);

/**
 * Prints the content with line numbers using @fwrite for efficient output.
 *
 * @param {const char*} content - The content string to print.
 */
void print_content_with_lines(const char* content, const char* path);

#endif
