#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
int numStringInThread=0;
FILE *output=NULL;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct threadContainer
{
    int array[20];
    char buffer[250];
    /* json */
};
typedef struct threadContainer threadContainer;

void *child(void *arg){
    //thread保存data避免待會被free memory
    threadContainer *data=(threadContainer *)arg;
    //printf("%s",data[0].buffer);
    char *saveptr=NULL;
    for(int i=0;i<numStringInThread;++i){
       
        char *string=data[i].buffer;
        //printf("%s",json[i].buffer); 
        char *token=strtok_r(string,"|",&saveptr);
         
        int k=0;
        
        while (token!=NULL)
        {
            data[i].array[k]=atoi(token);
            //printf("%s|",token);
            token=strtok_r(NULL,"|",&saveptr);
            ++k;
        }
        //printf("\n");
    }

    //unlock
    pthread_exit(NULL);
}
void spliteByMultiThread(threadContainer *json,int array_num,int thread_num){

    
}

int main(int argc,char *argv[]){
    printf("pid=%d\n",getpid());

    char *output_file="output.json";
    char *input_file="input.csv";
    
    //int res=writeTojson(output_file);
    FILE *input;
    input=fopen(input_file,"r");//per thread handle string num
    output=fopen(output_file,"w");
    int array_num=20000;
    int read_count=0;
    int thread_count=0;
    
    int thread_num=atoi(argv[1]);//number of thread     
    //for initiate the threadContainer numofString
    numStringInThread=array_num/thread_num;
    pthread_t *myThread=malloc(sizeof(pthread_t)*thread_num);
    threadContainer json[thread_num][numStringInThread];
    //printf("successful!");
    while (!feof(input))
    {
        
        fgets(json[thread_count][read_count].buffer,250,input);
        ++read_count;
        if(read_count>=numStringInThread){
                pthread_create(&myThread[thread_count],NULL,child,(void*)&json[thread_count]);
                thread_count++;


                //做的同時繼續讀下一個
                read_count=0;
        }
        //如果thread滿了要等所有thread完畢才能再讀
        if(thread_count>=thread_num){
            for(int i=0;i<thread_num;++i){
                pthread_join(myThread[i],NULL);
                for(int j=0;j<numStringInThread;++j){
                    //fputs(json[i].buffer,output);
                    for(int k=0;k<20;++k){
                    
                        char *tmp=malloc(sizeof(char)*20);
                        if(k<19){
                            sprintf(tmp,"%d|",json[i][j].array[k]);
                            fputs(tmp,output);
                        }
                        else{
                            sprintf(tmp,"%d\n",json[i][j].array[k]);
                            fputs(tmp,output);
                        }
                        free(tmp);
                    }
                    //printf("\n");
                }
                //全部做完準備寫入檔案
                thread_count=0;
            }
        }

        
    }
    if(!feof(input))
        printf("not end!");
    else
        printf("end %d",feof(input));

    
}