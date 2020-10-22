#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
int main(){
    printf("pid=%d\n",getpid());
    char *output="input3.csv";
    FILE *out;
    out=fopen(output,"w");
    int change_line=0;
    int num=0;
    char *str=malloc(sizeof(char)*10);
    for(int i=0;i<200000;++i){
        while (change_line<20)
        {
            num=rand()%30;
            if(change_line==0)
                sprintf(str,"%d",num);
            else
                sprintf(str,"|%d",num);
            fputs(str,out);
            ++change_line;
        }
        fputc('\n',out);
        change_line=0;  

    }
    fclose(out);
}