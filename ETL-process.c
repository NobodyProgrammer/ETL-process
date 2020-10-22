#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
int column_count=0;
struct jsonContainer
{
    int index;
    int array[20];
    char buffer[250];
    /* data */
};
typedef struct jsonContainer jsonContainer;


void *child(void *arg){
    jsonContainer *data=(jsonContainer *)arg;
    //printf("%s\n",data->buffer);
    char *string=data->buffer;
    char *token=strtok(string,"|");
    int i=0;
    while (token!=NULL)
    {
        data->array[i]=atoi(token);
        token=strtok(NULL,"|");
        //printf("%d ",data->array[i]);
        ++i;
    }
    // if(data->index==5)
    //     sleep(2);
    //printf("thread %d finish\n",data->index);
    
    pthread_exit(NULL);
}
void spliteByMultiThread(jsonContainer *json,int buffer_index,int thread_num){
    pthread_t *myThread=malloc(sizeof(pthread_t)*thread_num);
    int idx=buffer_index;//this is the thread index for the correct order of structure
    for(int i=0;i<thread_num;++i){
        json[i].index=i;
        pthread_create(&myThread[i],NULL,child,(void*)&json[idx++]);
        //printf("next thread!!!\n");
        
    }
    for(int i=0;i<thread_num;++i){

        pthread_join(myThread[i],NULL);
        //printf("thread %d finish\n",i);
    }
    
}
void writeTojson(char *output_name,jsonContainer *data,int array_num){
    FILE *output;
    output=fopen(output_name,"w");
    for(int i=0;i<array_num;++i){
        for(int j=0;j<20;++j)
            printf("%d ",data[i].array[j]);
        printf("\n");
    }
    
}
int main(int argc,char *argv[]){
    printf("pid=%d\n",getpid());

    char *output_file="output.json";
    char *input_file="input2.csv";
    FILE *input;
    int array_num=200000;
    int count=0;
    jsonContainer *json=malloc(sizeof(jsonContainer)*array_num);
    int thread_num=atoi(argv[1]);
    input=fopen(input_file,"r");
    while (!feof(input))
    {
        
        fgets(json[count].buffer,250,input);
        //printf("%s",json[count].buffer);
        ++count;
        if(count>=array_num){
            //here to use thread to strtok string
            for(int i=0;i<array_num;i+=thread_num){
                //index is 
                spliteByMultiThread(json,i,thread_num);
                //printf("finish\n");
            }


            //here to write to json file
            //writeTojson(output_file,json,array_num);



            count=0;


            //break;
        }
    }
    if(!feof(input))
        printf("not end!");
    else
        printf("end %d",feof(input));

    
}