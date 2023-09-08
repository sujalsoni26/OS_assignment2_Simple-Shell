#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

static char *process_pid[1000][5];
static char **history_list;
int history_number = 0;
int process_no =0;

void strip(char* str){
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

char ** create_tokens(char *input){
    int no_of_tokens = 1024;
    int i = 0;
    char **tokens = malloc(sizeof(char*)* no_of_tokens);
    if(tokens==NULL){
        printf("Memory not allocated to tokens.");
    }
    char *token = strtok(input, " ");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " ");
    }    
    tokens[i] = NULL;
   
    return tokens;
}

void creating_historylist(char* input){
    if (history_list == NULL){
        history_list=(char*)malloc(sizeof(char)*2048);

        //checking if memory is allocated to history
        if(history_list == NULL){
            printf("Memory not allocated to history.");
        }
    }

    //if you hit only enter it will not be stored
    if(strcmp(input,"\n")){
        char *copy;
        copy = strdup(input);
        history_list[history_number]=copy;
        history_number++;
    }
}

char * read_inp(){
    int size = 2048;
    char * input = (char *)malloc(sizeof(char) * 2048);
    if (input == NULL){
        printf("Memory not allocated to input");
    }
    fgets(input, size , stdin);
    creating_historylist(input);
    // printf("You entered: %s\n", input);
    return input;   
}

void check_command(char ** command){
    char str[1000] = "/usr/bin/";
    strcat(str, *(command+0));
    pid_t pid;
    clock_t start;
    clock_t end; 
    double duration;
    char dur[50]; 
    process_pid[process_no][0]=history_list[process_no];
    
    if (strcmp(*(command+0), "ls\n") == 0){
        start = clock();
        process_pid[process_no][1]=ctime(&start);
        int status = fork();
        
        //checking for fork failure
        if (status < 0)
        {
            printf("fork failed\n");
            exit(0);
        }

        //child process for ls to execute
        else if(status == 0){
            // printf("Process id:  %d\n", getpid());
    
            
            execl("/usr/bin/ls", "/usr/bin/ls",  NULL);
            printf("This statement should not be printed.\n");
        }

        //parent process for the shell to continue running
        else  {
            int ret;
            int pid = wait(&ret);

            if(WIFEXITED(ret)) {
                pid = getpid();
                process_pid[process_no][2]=pid;
                end = clock();
                process_pid[process_no][3] =ctime(&end);
                duration = (double)(end-start)/CLOCKS_PER_SEC;
                snprintf(dur,sizeof(dur),"%f",duration);
                process_pid[process_no][4]=dur;
                // printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
            } else {
                printf("Abnormal termination of %d\n",pid);
            }
        }  
    }

    if (strcmp(*(command+0), "history\n") == 0){
        for(int k=0;k<history_number;k++){
            printf("%s",history_list[k]);
        }
    }

    //creating arr to be passed in execv

    start = clock();
    process_pid[process_no][1]=ctime(&start);
    char *arr[1000];
    int j=0;
    for (int i = 0; command[i] != NULL; i++){
        if (j == 0)
        {
            arr[j] = str;
            j++;
        }
        else{
            arr[j] = strdup(command[i]);
            strip(arr[j]);
            j++;
        }
    }
    arr[j] = NULL;

    int status = fork();
    //checking for fork failure
    if (status < 0)
    {
        printf("fork failed\n");
        exit(0);
    }

    //child process  to execute    
    else  if(status == 0){
        execv(str, arr);
    }  

    //parent process for the shell to continue running
    else {
        int ret;
        int pid = wait(&ret);

        if(WIFEXITED(ret)) {
            pid = getpid();
            process_pid[process_no][2]=pid;
            end = clock();
            process_pid[process_no][3] =ctime(&end);
            duration = (double)(end-start)/CLOCKS_PER_SEC;
            snprintf(dur,sizeof(dur),"%f",duration);
            process_pid[process_no][4]=dur;
            //printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
        } else {
            printf("Abnormal termination of %d\n",pid);
        }

    }
    process_no++;
   
}

void terminate(){
    for (int j=0;j<process_no;j++){
        printf("Command: %s",process_pid[j][0]);
        printf("Start time: %s",process_pid[j][1]);
        printf("Process id: %d\n",process_pid[j][2]);
        printf("End time: %s",process_pid[j][3]);
        printf("Duration: %s\n",process_pid[j][4]);
        printf("\n");
    }
    return;
} 

int main(int argc, char const *argv[]){
    char cwd[500];
    printf("Shell Initialised\n");
    if (getcwd(cwd,sizeof(cwd))==NULL){
        printf("Error in locating your directory.");
    }
    else{
        printf("You are in %s directory\n",cwd);
    }
    while (1)
    {   
        printf("iiitd@myshell :~$ ");
        char * inp = read_inp();
        if (strcmp(inp, "exit\n") == 0) {
            //printf("The shell has exited.\n");
            terminate();
            break;
            exit(0);
        }
        char **tok = create_tokens(inp);
        if(tok!=NULL){
            check_command(tok);
        }
    }
    return 0;
}
