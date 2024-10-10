#include "../../header/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// GETS LINES (Working)
int fget_line(char* _Prompt, char* _Buffer, int _Buffer_length){
    if (_Prompt != NULL){
        printf("%s", _Prompt);
    }

    char* f = fgets(_Buffer, _Buffer_length, stdin);

    if (f == NULL){
        fprintf(stderr, "Cannot get input.");
        return 1;
    }

    size_t len = strlen(_Buffer);

    if (_Buffer[len-1] == '\n'){
        _Buffer[len-1] = '\0';
    }

    return 0;
}

// READS CONTENT OF A FILE (Working)
char* read_content(char* _File_path){ //maybe remake this function later only to exercise
    FILE* f = fopen(_File_path, "r");
    if (!f){
        fprintf(stderr, "Cannot open file.");
        return NULL;
    }

    size_t size = 1028; // two kilobytes
    size_t total_size = 0;
    char* content = malloc(size);
    if (!content){
        fprintf(stderr, "Memory allocation failed.");
        fclose(f);
        return NULL;
    }

    size_t instance;
    while ((instance = fread(content + total_size, 1, size, f)) > 0){ //while reading bytes
        total_size += instance; // add how many bytes read to total size
        if (total_size + size > size){
            size *= 2; //double the size
            char* temp = realloc(content, size);
            if (!temp){
                fprintf(stderr, "Memory reallocation failed.");
                free(content);
                fclose(f);
            }
            content = temp;
        }
    }
    
    if(ferror(f)){
        fprintf(stderr, "Error reading file");
        free(content);
        fclose(f);
        return NULL;
    }

    content[total_size] = '\0';
    fclose(f);
    return content;
}

int select_number(int _Min_line, int _Max_line){
    int c;
    int i = _Min_line;
    printf("\r%c %i", 0x10, i);
    while ((c = getch()) != 13){
        switch(c){
            case 72:
                if (i < _Max_line) i++;
                break;
            case 80:
                if (i > _Min_line) i--;
                break;
            default: 
                continue;
        }
        printf("\r%c %i ", 0x10, i);
    }
    return i;
}

char* edit_line(int _Line, char* _Content){
    int size = 1024; // 1 kilobyte
    char* temp = malloc(size);
    
    if (temp < sizeof(_Content)){            
        size *= 2;
        char* temp = realloc(temp, size);
    }
    
    for (int i = 0; i < _Line; i++){
        _Content = strchr(_Content, '\n') + 1;
    }
    char* line_end = strchr(_Content, '\n');
   
    if (line_end != NULL){
        strcpy(temp, line_end);
    }

    fget_line("\n\nEnter sub-text >> ", _Content, sizeof(_Content));
    strcat(_Content, temp);
    return _Content;
}

int get_amount_lines(int size, char* Content){
    int total_lines = 0;
    for (int i = 0; i < size; i++){
        if (Content[i] == '\n'){ 
            total_lines++;
        };
    }
    return total_lines;
}

void print_content(int size, char* Content){
    int i = get_amount_lines(size, Content);
    char* cptr = strtok(Content, "\n");
    for (int token = 1; token < i; token++){
        printf("%i - ", token);
        printf("%s", cptr);
        cptr = strtok(NULL, "\n");  
    }
}

// So I thought about checking the index of the string and if it finds a newline, it will print the sum of all
// newlines (if encounters new line = newlineit++) encountered before (+ 1 so we actually have line 1 as 1 and not 0) ant then print the string;
