#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
int column_count=0;
int numStringInThread=0;
struct threadContainer
{
    int array[21];
    char buffer[250];
    /* json */
};
typedef struct threadContainer threadContainer;
threadContainer *json;
void *child(void *arg){
    int *idx_address=(int *)arg;
    int idx=*idx_address;
    //printf("%d %d\n",idx,numStringInThread);
    int k=0;
    char *saveptr=NULL;
    //printf("thread %d j address=%p\n",idx,&k);
    for(int i=idx;i<idx+numStringInThread;++i){
       
        char *string=json[i].buffer;
        //printf("%s",json[i].buffer); 
        char *token=strtok_r(string,"|",&saveptr);
         
        k=0;
        
        while (token!=NULL)
        {
            json[i].array[k]=atoi(token);
            //printf("%s|",token);
            token=strtok_r(NULL,"|",&saveptr);
            ++k;
        }
        //printf("\n");
    }
    pthread_exit(NULL);
}
void spliteByMultiThread(threadContainer *json,int array_num,int thread_num){
    int *start_idx=malloc(sizeof(int)*thread_num);
    int now_idx=0;
    for(int i=0;i<thread_num;++i){
        now_idx=i*numStringInThread;
        start_idx[i]=now_idx;
        
    }
    pthread_t *myThread=malloc(sizeof(pthread_t)*thread_num);
    void *ret=0;
    for(int i=0;i<thread_num;++i){
        //printf("%d %d\n",json[0].start_index,numStringInThread);
        pthread_create(&myThread[i],NULL,child,(void*)&start_idx[i]);       
        //printf("next thread!!!\n");
        
    }
    for(int i=0;i<thread_num;++i){

        pthread_join(myThread[i],&ret);

        //printf("thread %d finish\n",i);
    }
    //printf("\n");
    free(myThread);
    
}
int main(int argc,char *argv[]){
    printf("pid=%d\n",getpid());

    char *output_file="output.json";
    char *input_file="input2.csv";
    //int res=writeTojson(output_file);
    FILE *input;
    FILE *output;
    int array_num=50000;
    int count=0;
    int thread_num=atoi(argv[1]);//number of thread 
    
    //for initiate the threadContainer numofString
    numStringInThread=array_num/thread_num;
    json=malloc(sizeof(threadContainer)*array_num);

    input=fopen(input_file,"r");//per thread handle string num
    output=fopen(output_file,"w");
    
    
    fwrite("[\n",1,sizeof(char)*2,output);
    while (!feof(input))
    {
        
        fgets(json[count].buffer,250,input);
        ++count;
        if(count>=array_num){
            //here to use thread to strtok string
            //printf("successful\n");
            spliteByMultiThread(json,array_num,thread_num);
            for(int i=0;i<array_num;++i){
                 //fputs(json[i].buffer,output);
                fwrite("    {\n",1,sizeof(char)*6,output);
                for(int j=0;j<20;++j){
                    
                    char *tmp=malloc(sizeof(char)*50);
                    if(j<19){
                        sprintf(tmp,"       ""col_%d"":%d,\n",j+1,json[i].array[j]);
                        fwrite(tmp,1,strlen(tmp),output);
                    }
                    else{
                        sprintf(tmp,"       ""col_%d"":%d\n",j+1,json[i].array[j]);
                        fwrite(tmp,1,strlen(tmp),output);
                    }
                    free(tmp);
                }
                fwrite("    },\n",1,sizeof(char)*7,output);
                //printf("\n");
            }


            //here to write to json file
            //writeTojson(output_file,json,array_num);



            count=0;


        }
        
    }
    fwrite("]",1,sizeof(char),output);
    
}