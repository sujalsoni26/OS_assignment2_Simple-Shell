#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void strip(char* str){
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

char ** create_tokens(char *input){
    int no_of_tokens = 1024;
    int i = 0;
    char **tokens = malloc(sizeof(char*)* no_of_tokens);
    char *token = strtok(input, " ");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " ");
    }    
    tokens[i] = NULL;

    // for (int j = 0; j <i ; j++)
    // {
    //     printf("%s \n", *(tokens+j));
    // }
    
    return tokens;
}

char * read_inp(){
    int size = 2048;
    char * input = (char *)malloc(sizeof(char) * 2048);
    fgets(input, size , stdin);
    // printf("You entered: %s\n", input);
    return input;   
}

void check_command(char ** command){
    if (strcmp(*(command+0), "ls\n") == 0){
        // printf("in ls.");
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
                // printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
            } else {
                printf("Abnormal termination of %d\n",pid);
            }

            // printf("inside parent\n");
            // sleep(2);
        }  
    }

    if (strcmp(*(command+0), "ls") == 0){
        // printf("in ls arg1\n");
        int status =fork();
        //checking for fork failure
        if (status < 0)
        {
            printf("fork failed\n");
            exit(0);
        }

        //child process for ls to execute
        else if(status == 0){
            char arg1[100];
            if (command[1]!=NULL){
                for (int i = 1; command[i] != NULL; i++){
                    strcpy(arg1, command[i]);
                    strip(arg1);
                    strcat(arg1,"");
                    printf("%s",arg1);
                }
                execl("/usr/bin/ls","/usr/bin/ls", arg1 , NULL);
                printf("This statement should not be printed.\n");
            }
        }
        
        //parent process
        else {
            int ret;
            int pid = wait(&ret);

            if(WIFEXITED(ret)) {
                // printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
            } else {
                printf("Abnormal termination of %d\n",pid);
            }
        }
    }
    
    
    if (strcmp(*(command+0), "echo") == 0){
        char arr[1000];
        //printf("%ld       %ld", sizeof(command), sizeof(*(command+0)));

        int status = fork();
        if(status == 0){
            for (int i = 1; command[i] != NULL; i++){
                strcpy(arr + strlen(arr), command[i]);
                strcat(arr, " ");
        }

            printf(">>>>>>>>>>>>>>      %s \n", arr);
            //printf("innn echo\n");
            execl("/usr/bin/echo", "/usr/bin/echo", arr, NULL);
            //printf("\nafter echo\n");
        }
        else if (status < 0)
        {
            printf("fork failed\n");
            exit(0);
        } else {
            int ret;
            int pid = wait(&ret);

            if(WIFEXITED(ret)) {
                //printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
            } else {
                printf("Abnormal termination of %d\n",pid);
            }
            //printf("inside parent\n");
        }
    }

    if (strcmp(*(command+0), "wc") == 0){
        //printf("%d   ", strcmp(*(command+0), "wc"));
        char *arr[1000];
        //printf("%ld       %ld", sizeof(command), sizeof(*(command+0)));

        int j=0;
        for (int i = 0; command[i] != NULL; i++){
            if (j == 0)
            {
                arr[j] = "/usr/bin/wc";
                j++;
            }
            else{
                arr[j] = strdup(command[i]);
                strip(arr[j]);
                j++;
            }
        }
        arr[j] = NULL;

        // for (int i = 0; arr[i] != NULL; i++)
        // {
        //     printf("%d   %s  ", i, arr[i]);
        // }

        int status = fork();
        if(status == 0){
            // int j=0;
            // for (int i = 1; command[i] != NULL; i++){
            //     strcpy(arr[j], command[i]);
            //     j++;
            execv("/usr/bin/wc", arr);
        }      
        else if (status < 0)
        {
            printf("fork failed\n");
            exit(0);
        } else {
            int ret;
            int pid = wait(&ret);

            if(WIFEXITED(ret)) {
                //printf("%d Exit =%d\n",pid,WEXITSTATUS(ret));
            } else {
                printf("Abnormal termination of %d\n",pid);
            }

            //printf("inside parent\n");
        }
    }




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
            break;
        }
        char ** tok = create_tokens(inp);
        check_command(tok);
    }
    return 0;
}
