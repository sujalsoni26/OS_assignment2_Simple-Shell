#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    for (int j = 0; j <i ; j++)
    {
        printf("%s \n", *(tokens+j));
    }
    
    return tokens;
}

char* read_inp(){
    int size = 2048;
    char * input = (char *)malloc(sizeof(char) * 2048);
    fgets(input, size , stdin);
    // printf("You entered: %s\n", input);
    return input;   
}

char * check_command(char ** command){
    if (strcmp(*(command+0), "ls\n") == 0)
    {
        printf("in ls.");
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
            char arg1[100];
            
            if (command[1]!=NULL){
                for (int i = 1; command[i] != NULL; i++){
                    printf("here");
                    strcpy(arg1 + strlen(arg1), command[i]);
                    strcat(arg1," ");
                    printf("%s",arg1);
                }
                execl("/usr/bin/ls","/usr/bin/ls", arg1 , NULL);
            }
            
            execl("/usr/bin/ls", "/usr/bin/ls",  NULL);
            printf("This statement should not be printed.\n");
        }

        //parent process for the shell to continue running
        else {
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
    
    if (strcmp(*(command+0), "echo") == 0)
    {
        char * arr[1000];
        for (int i = 1; arr[i] != NULL; i++)
        {
            arr[i] = *(command+i);
        }
        
        printf("innn echo\n");
        execl("/usr/bin/echo", "/usr/bin/echo", *arr, NULL);
        printf("\nafter echo\n");        
    }
    return command[0];
}

int main(int argc, char const *argv[])
{
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
        char *test = check_command(tok);
    }
    return 0;
}
