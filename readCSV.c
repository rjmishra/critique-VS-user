/*
Author: Ranjan Mishra
Date: 20th Aug 2015
Program: CSV data file reading
Acknowledgment: 
*/


#include<stdio.h> 
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_ROW_SIZE 2048


/* Reading the data from the file in CSV format i.e. comma seperated format
*  The number of arguments may differ, this function will take variable arguments
*  but first argument will be location of the file to be read  
*  and it return a two dimensional array, which contains data
*/

unsigned int i=0, j=0;

typedef struct field{
	char *fieldName;
	char **content;
} FIELD;






/* The no_of_char will return the no of occrence of pattern in the given string as buffer
 * size will be size of the length between one pattern to next pattern
*/
 
int no_of_char(char pattern,  char *buffer){
	int occurences=0, i=0;
	while(buffer[i]){
		if(buffer[i]==pattern){
			occurences++;
		}	
		i++;
	}
	return occurences;
}
	
char** readLine(char *buffer, int noline, char pattern){
	char *buff, **lines, temp;
	int i=0;
	lines=(char**) malloc(noline*sizeof(char*));
	if(lines==NULL){
		perror("Not enough spaces");
		exit(-1);
	}
	buff=buffer;
	for(i=0;i<noline;i++){
		lines[i]=buff;
		while(*buff!=pattern) buff++;
		temp=*buff;	
		*buff='\0';
		lines[i]=strdup(lines[i]);
		*buff=temp;		
		buff++;
	}
	return lines; 
}

char** readData(char *buffer, int noline, char pattern){
	char *buff, **lines, temp;
	int i=0;

	lines=(char**) malloc(noline*sizeof(char*));
	if(lines==NULL){
		perror("Not enough spaces");
		exit(-1);
	}
	buff=buffer;
	for(i=0;i<noline;i++){
		lines[i]=buff;
		while(*buff!=pattern) buff++;
		temp=*buff;		
		*buff='\0';
		lines[i]=strdup(lines[i]);
		*buff=temp;		
		buff++;
	}
	return lines; 
}


char* readWhole(const char *fileName){
	struct stat stats;
	int result;
	char *buffer;
	FILE *inputFile; 	
	result=stat(fileName, &stats);
	if(result!=0){
		//printf("Result is: %d", result);
		perror("\nNot able to get the stats,");
		exit(-1);
	}
	inputFile=fopen(fileName,"r");
	if(inputFile==NULL){
		perror("Error in opening file!");
		exit(-1);
	}

	buffer=malloc((int)(stats.st_size)*sizeof(char)+1);
	if(buffer==NULL){
		perror("\nNot able to allocate the space!");
		exit(-1);
	}
	
	result=fread(buffer, stats.st_size, stats.st_size, inputFile);
	//printf("%s", buffer);
	//i=stats.st_size;
	fclose(inputFile);
	return buffer;  
}

FIELD** read_CSV(const char *file, int* cols, int* rows){
	char *inBuf;
	char **lines;
	char **data;
	int row, col;
	int i,j;
	FIELD **table;
	inBuf=readWhole(file);
	//printf("%s", inBuf);
	*rows=row=no_of_char('\n',inBuf);
	lines=readLine(inBuf, row, '\n');
	*cols=col=no_of_char(',', lines[0])+1;
	table=(FIELD**) malloc(col*sizeof(FIELD*));
	data=readData(lines[0], col, ',');
	if(table==NULL){
		exit(0);
	}
	for(i=0;i<col;i++){
		table[i]=(FIELD*) malloc(sizeof(FIELD));
		if(table[i]==NULL){
		exit(0);
		}
	}
	//printf("\n Data is present for below columns: ");
	printf("\n");
	for(i=0;i<col;i++){
		table[i]->fieldName=data[i];
		//printf("%s\t", table[i]->fieldName);
	}
	printf("\n");

	for(j=0;j<col;j++){
		table[j]->content=(char**)malloc(sizeof(char*));
		for(i=0;i<row;i++){
			table[j]->content[i]=(char*)malloc(100*sizeof(char));
		}
	}

	for(i=1;i<row;i++){
		data=readData(lines[i], col, ',');
		for(j=0;j<col;j++){
			table[j]->content[i-1]=data[j];
		}
	}
			
	return table;
}

	


int main(){
	FIELD **table;
	int rows=0, cols=0;
	int i=0;
	table=read_CSV("other.csv", &cols, &rows);
	for(j=0;j<cols;j++){
			printf("%s\t", table[j]->fieldName);
		}
	for(i=0;i<rows;i++){
		printf("\n");
		for(j=0;j<cols;j++){
			printf("%s\t", table[j]->content[i]);
		} 
	}
	printf("\n");
	free(table);
	return 0;

}



