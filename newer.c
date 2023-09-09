#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

static char **history_list;
int history_number = 0;

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

int count_pipes(char **command){
    int count = 0;

    for (int i = 0; command[i] != NULL; i++){
        if (command[i][0] == 124)
        {
            count++;
        }
    }
    return count;
}

void check_command(char ** command){
    int no_of_pipes = count_pipes(command);
    char str[1000] = "/usr/bin/";

    //normal commands like ls,ls /home,cat abx.txt will be executed here
    if (no_of_pipes == 0){
        strcat(str, *(command+0));
        if (strcmp(*(command+0), "ls\n") == 0){
            int status = fork();

            //checking for fork failure
            if (status < 0)
            {
                printf("fork failed\n");
                exit(0);
            }
            //child process for ls to execute
            else if(status == 0){           
                execl("/usr/bin/ls", "/usr/bin/ls",  NULL);
                printf("This statement should not be printed.\n");
            }
            //parent process for the shell to continue running
            else  {
                // printf("Parent process is waiting\n");
                int ret;
                int pid = wait(&ret);

                if(WIFEXITED(ret)) {
                    // printf("parent process ends\n");
                    return;
                } else {
                    printf("Abnormal termination of %d\n",pid);
                }
            }  
        }


        else if (strcmp(*(command+0), "history\n") == 0){
            for(int k=0;k<history_number;k++){
                printf("%s",history_list[k]);
            }
        }

        else if (command[0][0] == 46 && command[0][1] == 47)
        {
            //creating array of args.
            char strr[1000] = "";
            strcat(strr, *(command+0));
            strip(strr);

            char *arr[1000];
            int j=0;
            for (int i = 0; command[i] != NULL; i++){
                if (j == 0)
                {
                    arr[j] = strr;
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
                execvp(strr, arr);
                return;
            }  

            //parent process for the shell to continue running
            else {

                // printf("Parent process is waiting\n");
                int ret;
                int pid = wait(&ret);

                if(WIFEXITED(ret)) {
                    // printf("parent process ends\n");
                    return;
                } else {
                    printf("Abnormal termination of %d\n",pid);
                }
            }        
        }
        
        else{

            //creating arr to be passed in execv
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
                // printf("Parent process is waiting\n");
                int ret;
                int pid = wait(&ret);

                if(WIFEXITED(ret)) {
                    // printf("Prent process ends\n");
                    return;
                } else {
                    printf("Abnormal termination of %d\n",pid);
                }

            }
        }  
    }
    
    //commands like cat abc.txt | wc -l will be executed here
    else{
        int count = no_of_pipes;
        int fd[count][2];
        for (int p = 0; p < count; p++)
        {
            if (pipe(fd[p]) < 0)
            {
                printf("Pipe Failed\n");
                return;
            } 
        }
        int q = 0;
        int j = 0;
        int i = 0;

        char temp[1000] = "/usr/bin/";
        char *arr[1000];
        j = 0;
    
        for (i ; command[i][0] != 124; i++){
            if (j == 0)
            {
                strcat(temp, *(command+i));
                strip(temp);
                arr[j] = temp;
                j++;
            }
            else{
                arr[j] = strdup(command[i]);
                strip(arr[j]);
                j++;
            }


        }
        arr[j] = NULL;
        int status1 = fork();

        if (status1 < 0)
        {
            printf("fork failed\n");
            exit(0);
        }
        else if (status1 == 0)
        {
            
            for (int e = 0; e < count; e++){
                if (e != 0)
                {
                    close(fd[e][0]);
                    close(fd[e][1]);
                }
            close(fd[0][0]);

            dup2(fd[0][1], STDOUT_FILENO);
            
            close(fd[0][1]);

            execv(temp, arr);
            }
        }
        i++;
        q++;
        
        // printf("Process 1 ends\n");
        
        for(int g = 0; g < (no_of_pipes-1); g++){
            j = 0;
            char temp2[1000] = "/usr/bin/";
            char *arr2[1000];

            for (i ; command[i][0] != 124; i++){
                if (j == 0)
                {
                    strcat(temp2, *(command+i));
                    strip(temp2);
                    arr2[j] = temp2;
                    j++;
                }
                else{
                    arr2[j] = strdup(command[i]);
                    strip(arr2[j]);
                    j++;
                }
            }
            arr2[j] = NULL;
            
            int status2 = fork();

            if (status2 < 0)
            {
                printf("fork failed\n");
                exit(0);
            }
            else if (status2 == 0)
            {
                for (int e = 0; e < count; e++){
                    if (e == q || e == (q-1))
                    {
                        continue;
                    }else{
                        close(fd[e][0]);
                        close(fd[e][1]);
                    }
                }
                 
                close(fd[q][0]);
                close(fd[q-1][1]);

                dup2(fd[q-1][0], STDIN_FILENO);
                dup2(fd[q][1], STDOUT_FILENO);

                close(fd[q][1]);
                close(fd[q-1][0]);
                    
                execv(temp2, arr2);
            }
            i++;
            q++;
            // printf("Process 2 ends\n");
            
        }

            j = 0;
            q--;
            char temp3[1000] = "/usr/bin/";
            char *arr3[1000];

            for (i ; command[i] != NULL; i++){
                if (j == 0)
                {
                    strcat(temp3, *(command+i));
                    strip(temp3);
                    arr3[j] = temp3;
                    j++;
                }
                else{
                    arr3[j] = strdup(command[i]);
                    strip(arr3[j]);
                    j++;
                }
            }
            arr3[j] = NULL;

            int status3 = fork();

            if (status3 < 0)
            {
                printf("fork failed\n");
                exit(0);
            }
            else if (status3 == 0)
            {
                dup2(fd[q][0], STDIN_FILENO);

                for (int e = 0; e < count; e++){
                    if (e != q)
                    {
                        close(fd[e][0]);
                        close(fd[e][1]);
                    }
                }
                
                close(fd[q][1]);
                close(fd[q][0]);    
                execv(temp3, arr3);
            }

            // printf("Process 3 ends\n");


            for (int z = 0; z < count; z++) {
                close(fd[z][0]);
                close(fd[z][1]);
            }

            for (int x = 0; x < count+1; x++) {
                //printf("Parent process is waiting for %d child process\n", x);
                wait(NULL);
            } 
            // printf("Parent Process ends\n");

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
        printf("iiitd@myshell:%s $ ", cwd);
        char * inp = read_inp();
        if (strcmp(inp, "exit\n") == 0) {
            //printf("The shell has exited.\n");
            break;
        }
        char ** tok = create_tokens(inp);
        check_command(tok);
    }
    return 0;
}
