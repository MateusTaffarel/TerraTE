#ifndef UTILITIES_H_
#define UTILITIES_H_

int fget_line(char* _Prompt, char* _Buffer, int _Buffer_length);
char* read_content(char* _File_path);
int select_number(int _Min_line, int _Max_line); 
char* edit_line(int _Line, char* _Content);
int get_amount_lines(int size, char* Content);
void print_content(int size, char* Content);

#endif
