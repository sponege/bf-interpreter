/*
 * Written by Jordan Perry
 * https://jappl.es/
 * https://github.com/sponege
 */

#include <stdio.h>
#include <unistd.h> // for sleep
#include <stdlib.h>
#include <string.h>
#include <time.h>   // for random

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: %s [file]\n", argv[0]);
    return 0;
  }

  FILE *file;
  int fs; // file size
  file = fopen(argv[1], "r");
  
  if (file == NULL) {
    puts("Error opening file, this is either because the file doesn't exist or you don't have permission to read the requested file.");
    return 1; // error opening file
  }

  fseek(file, 0L, SEEK_END);
  fs = ftell(file);
  rewind(file);

  if (fs == -1) {
    puts("That's a directory lol");
    return 1; // a directory was provided instead of a file
  }

  char *code = malloc(fs * sizeof(char)); // string with file contents
  fread(code, sizeof(char), fs, file); // read file into string

  // Filter string to only include bf-supported characters.
  const char valid_chars[] = "><+-.,[]";
  char *code_index = code;
  int code_length = 0;
  for (int scan_index = 0; scan_index < fs; scan_index++) {
    if (strchr(valid_chars, code[scan_index])) { // check if character is valid
      *code_index = code[scan_index];
      code_index++; // go to next part of string
      code_length++;
    }
  }
  *code_index = 0; // terminate string with a null character

  unsigned char *mem = malloc(30000 * sizeof(char)); // 30 thousand cells, cell size is 8 bits
  int ip = 0; // ip is short for "instruction pointer"
  int dp = 0; // dp is short for "data pointer"
  unsigned long i = 0; // instruction executed

  while (ip < code_length) {
    if (code[ip] == '>') { // increment data pointer
      dp++;
      if (dp >= 30000) {
        dp = 0;
      }
    } else if (code[ip] == '<') { // decrement data pointer
      dp--;
      // printf("before: %d ", dp);
      if (dp < 0) {
        dp = 30000 - 1;
      }
      // printf("after: %d ", dp);
    } else if (code[ip] == '+') { // 
      mem[dp]++; // overflow is what C already does with byte-size locations.
    } else if (code[ip] == '-') { // decrement byte at data pointer
      mem[dp]--; // underflow is what C already does with byte-size locations.
    } else if (code[ip] == '.') { // print byte at data pointer
      putchar(mem[dp]);
    } else if (code[ip] == ',') { // accept one byte as input, store input in pointer
      char inputChar; // I'm guessing that because this was the last character that was declared, you can enter any length of string you want because the rest of the memory is garbage that can be used for this purpose.
      scanf("%c", &inputChar); // Somehow, it works for multiple characters. Cool.
      mem[dp] = inputChar;
    } else if (code[ip] == '[') { // if byte at dp is 0, jump to corresponding closing bracket
      if (mem[dp] == 0) {
        int bS = 0; // brackets seen
        while (bS >= 0) {
          ip++; // increment instruction pointer
          if (code[ip] == '[') {
            bS++;
          } else if (code[ip] == ']') {
            bS--;
          }
        }
      }
    } else if (code[ip] == ']') { // if byte at dp is not 0, jump to corresponding opening bracket
      if (mem[dp] != 0) {
        int bS = 0; // brackets seen
        while (bS >= 0) {
          ip--; // decrement instruction pointer
          if (code[ip] == ']') {
            bS++;
          } else if (code[ip] == '[') {
            bS--;
          }
        }
      }
    }

    i++;
    ip++; // increment instruction pointer
  }
  printf("\n%lu instructions executed", i);
  return 0; // program had no errors
}
