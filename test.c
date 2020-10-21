#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
int main() 
{ 
    char *in="input2.csv";
    char *out="output.json";
    FILE *input;
    FILE *output;
    input=fopen(in,"r");
    output=fopen(out,"r");
    char *buffer1=malloc(sizeof(char)*250);
    char *buffer2=malloc(sizeof(char)*250);
    int count=0;
    while (!feof(input))
    {
        if( strcmp(buffer1,buffer2)!=0)
        {
            printf("%s",buffer1);
            printf("%s\n",buffer2);
        }
        else
        {
            ++count;
        }
        
        fgets(buffer1,250,input);
        fgets(buffer2,250,output);

        
    }
    if(count>=100000)
        printf("successful\n");
    else
    {
        
        printf("error %d\n",100000-count);
    }
    
    
} 