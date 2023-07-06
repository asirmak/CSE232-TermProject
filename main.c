#include <stdio.h>
#include <string.h>
#include "Header.h"

char inputFileName[50];
char outputFileName[50];

int main(int argc, char* argv[])
{
	
	strcat(inputFileName, argv[1]);
	strcat(inputFileName, ".txt");
	strcat(outputFileName, argv[1]);
	strcat(outputFileName, ".asm");
	
    int m_count = read(inputFileName);
    printf("\nCount of macro: %d\n", m_count);
    
    FILE *fp = fopen(outputFileName, "w");
    
    
    for (int i = 0 ; !strstr(field[0], "END") ; i++){
        parse(inputFileName);
        is_macro(argc, argv);
    }
	
    return 0;
}
