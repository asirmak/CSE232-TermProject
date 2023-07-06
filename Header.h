#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef HEADER_FILE
#define HEADER_FILE

extern char inputFileName[50];
extern char outputFileName[50];


struct mac {
    char mname[8]; 
    char param[10][4]; 
    char macro[256];
};
struct mac buffer[10];

char field[10][20];

struct pt {
    char mname[8]; //macro name
    int nparams; //number of parameters
    char dummy[10][4]; //max. 10 parameters, each parameter max. 3 characters
    char actual[10][7];
};
struct pt PT;

int isThereIf = 0;



int read(char* filename){
    int m_count = 0;
    char mName[8], macro[256], par1[4], par2[4], par3[4];
    char x[7];
    char str[10] = {};
    FILE *file;

    file = fopen(filename, "r");

    while (fscanf(file,"%s %s %s %s %s", mName, x, par1, par2, par3) != EOF && !(strcmp(mName, "PROG") == 0))
    {
        memset(macro, 0, sizeof(macro));
        memset(str, 0, sizeof(str));

        while(true){
            fscanf(file, "%s", str);
            if(strcmp(str,"#ENDM") == 0){
                break;
            }
            strcat(macro, strcat(str, " "));
        }
        
        strcpy(buffer[m_count].mname, strtok(mName, ":"));
        strcpy(buffer[m_count].param[0], strtok(par1, ","));
        strcpy(buffer[m_count].param[1], strtok(par2, ","));
        strcpy(buffer[m_count].param[2], strtok(par3, ","));
        strcpy(buffer[m_count].macro, macro);

        m_count++;
    }
    fclose(file);
    
    return m_count;
}

void parse(char* filename){

    static int fieldLine = 1;

    FILE *file;
    file = fopen(filename, "r");

    char line[100];

    int isMain = 0;
    char *p = NULL;
    int lineNumber = 1;

    for (int i = 0 ; i<7 ; i++){
        strcpy(field[i],"");
    }

    while ( fgets(line,sizeof(line),file) ){

        if (strstr(line,"PROG")){
            isMain = 1;
            lineNumber = 1;
        }

        if(isMain == 1){

            int fieldIndex = 0;
            p = strtok(line," ");

            while(p != NULL) {

                if (lineNumber == fieldLine) {

                    if ( strstr(p,",") ){

                        //this field contains comma

                        char str2[100] = "";
                        char str3[100] = "";
                        char str4[100] = "";
                        int strIndex = 0;
                        int strLength = (int) strlen(p);

                        for (int i = 0; i < strLength ; i++){

                            if (p[i] == ','){
                                strIndex++;
                            }
                            else if (p[i] == ' '){
                                continue;
                            }
                            else{

                                if (strIndex == 0){
                                    strncat(str2,&p[i],1);
                                }
                                if (strIndex == 1){
                                    strncat(str3,&p[i],1);
                                }
                                if (strIndex == 2){
                                    strncat(str4,&p[i],1);
                                }
                            }
                        }

                        if (strlen(str2) != 0){
                            if ( strstr(str2,"\n") ){
                                strcat(field[fieldIndex], strtok(str2,"\n"));
                            }
                            else{
                                strcat(field[fieldIndex], str2);
                            }
                            fieldIndex++;
                        }

                        if (strlen(str3) != 0){
                            if ( strstr(str3,"\n") ){
                                strcat(field[fieldIndex], strtok(str3,"\n"));
                            }
                            else{
                                strcat(field[fieldIndex], str3);
                            }
                            fieldIndex++;
                        }

                        if (strlen(str4) != 0){
                            if ( strstr(str2,"\n") ){
                                strcat(field[fieldIndex], strtok(str4,"\n"));
                            }
                            else{
                                strcat(field[fieldIndex], str4);
                            }
                            fieldIndex++;
                        }


                    }

                    else if ( strstr(p,"\n") ){

                        //this field contains newline
                        strcat(field[fieldIndex], strtok(p,"\n"));
                        fieldIndex++;

                    }

                    else if ( strstr(p,"(") ){

                        //this field contains if parenthesis
                        strcat(field[fieldIndex], p);

                        char *f = p+1;
                        char left[100] = "";
                        char right[100] = "";

                        strncat(left,f,2);
                        strcpy(field[fieldIndex], left);
                        fieldIndex++;

                        //($2="x")

                        f = f+4;

                        strncat(right,f,strlen(f)-2);
                        strcpy(field[fieldIndex], right);
                        fieldIndex++;

                    }

                    else{

                        strcat(field[fieldIndex], p);
                        fieldIndex++;
                    }

                }

                p = strtok(NULL, " ");
            }

            lineNumber++;
        }
    }

    fieldLine++;
    fclose(file);
}
    
void createPT(){
	int ifCheck = 0;
	
	if(isThereIf){
		strcpy(PT.mname, field[3]);
		char string[10] = "#";
		strcat(string, PT.mname);
		strcpy(PT.mname, string);
		ifCheck = 3;
		isThereIf = 0;
	}
	else strcpy(PT.mname, field[0]);
        

	
    for(int x=0 ; x<PT.nparams ; x++){
        memset(PT.actual[x], 0, sizeof(PT.actual[x]));
    }

    PT.nparams = 0;
    
    for(int i = 0; i<buffer[i].mname[0] != '\0' ; i++){
        if(strcmp(buffer[i].mname, PT.mname) == 0){
            for(int k = 0; buffer[i].param[k][0] != '\0'; k++){
                strcpy(PT.dummy[k], buffer[i].param[k]);
                strcat(PT.dummy[k], "");
                PT.nparams+=1;
            }
        }
    }

    for (int i = 0; i < PT.nparams; i++) {
        int length = strlen(field[i+ifCheck + 1]);
        strncpy(PT.actual[i], field[i+ifCheck + 1], sizeof(PT.actual[i]) - 1); // Reserve space for string and space character
		if(i != PT.nparams-1){
			PT.actual[i][length] = ' '; // Append space character
        	PT.actual[i][length ] = '\n';
        	PT.actual[i][length + 1] = '\0'; // Null-terminate the string
		}
		else{
        	PT.actual[i][length + 1] = '\0'; // Null-terminate the string
		}
	}
    
}

int ff = 0;

void expand(){    
    
    createPT();

    char *token = NULL;
    char *prevToken = NULL;
    bool inLoop = true;
	int dum, ddum;
	
	
	char temp[10][256];	
		
    FILE *file = fopen(outputFileName, "a+");
    for(int i = 0; buffer[i].mname[0] != '\0'; i++){
    	
        if(strcmp(buffer[i].mname, PT.mname) == 0){
        	
			strcpy(temp[ff], buffer[i].macro);
			strcat(temp[ff], " ");
			
            token = strtok(temp[ff++], " ");
            while (token != NULL) {
            		
                prevToken = token;
                token = strtok(NULL, " ");

                if (strlen(prevToken) < 3 || strlen(prevToken) > 4) {
                	
                	for(int fb = 0; PT.dummy[fb][0] != '\0'; fb++){
                		ddum = 0;
                		if(strcmp(prevToken, PT.dummy[fb]) == 0){
                			fprintf(file, "%s ", PT.actual[fb]);  // Dummy code
							ddum = 1;
						}
                		
					}
                
                    if (!ddum && prevToken[0] == '#'){
                    	fprintf(file, "%s ", prevToken);
					}
                    
                    else if(!ddum){
                        fprintf(file, "%s ", prevToken);  // Loop label
                    }
                }
                else {
                    fprintf(file, "%s ", prevToken);   // Actual code
                	   
                    if(token == NULL){
                        break;
        
                    }
                    
                    else if (strlen(token) < 3) {
                	
	                	for(int fb = 0; PT.dummy[fb][0] != '\0'; fb++){
							dum = 0;
							if(strcmp(token, PT.dummy[fb]) == 0){
	                			fprintf(file, "%s", PT.actual[fb]);  // Dummy code
	                			prevToken = token;
                        		token = strtok(NULL, " ");
                        		dum = 1;
                        		break;
							}
	                		
						}
	                    if (!dum && token[0] == '#'){
	                    	fprintf(file, "%s\n", token);
	                    	prevToken = token;
                        	token = strtok(NULL, " ");
						}
	                    
	                    else if (!dum){
	                    	fprintf(file, "%s ", token);  // Loop label
	                        prevToken = token;
	                        token = strtok(NULL, " ");
	                        if(inLoop){
	                            fprintf(file, "\n");
	                            inLoop = false;
	                        }
	                        inLoop = 1;
	                    }
                	}
                    else {
                        fprintf(file, "\n");
                    }        
                }
            }
            
        }
        
    }
	fclose(file);
}


void is_macro(int argc, char* argv[]){
		
	
    char *filename = outputFileName;

    FILE *fp = fopen(filename, "a+");

	if(strcmp(field[0], "#if") == 0){
		
		char fieldTempChar = field[1][1];
		int fieldTempInt = fieldTempChar - '0';
		int result = fieldTempInt;


		if (strcmp(argv[result], field[2]) == 0){
			isThereIf = 1;
			expand();
		}
	}
    else if (strstr(field[0],"#")){

        expand();

    }
    else{
        char addingLine[100] = "";

        for (int i = 0 ; strcmp(field[i] , "") != 0 ; i++){
            strcat(addingLine , field[i]);
            strcat(addingLine , " ");
        }
        int length = strlen(addingLine);
		
		if (length > 0) {
    		addingLine[length - 1] = '\0'; // Set the last character to null
		}	
		
        fprintf(fp, "%s", addingLine);
        fprintf(fp, "\n");
    }

    fclose(fp);
	
}

#endif
