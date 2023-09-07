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
    char str[1000] = "/usr/bin/";
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
        }  
    }

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
        if(status == 0){
            execv(str, arr);
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
