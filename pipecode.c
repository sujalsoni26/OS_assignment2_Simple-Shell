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
    if (no_of_pipes == 0){
        strcat(str, *(command+0));
        
        //printf("%d  %d\n", command[0][0], command[0][1]);

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
                    return;
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

        if (command[0][0] == 46 && command[0][1] == 47)
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
                int ret;
                int pid = wait(&ret);

                if(WIFEXITED(ret)) {
                    return;
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


        // for (int i = 0; arr[i] != NULL; i++)
        //     {
        //         printf("%d   %s  \n", i, arr[i]);
        //     }

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
                return;
            } else {
                printf("Abnormal termination of %d\n",pid);
            }

        }
    }
    
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
        //int a = count;
        int q = 0;
        int j = 0;
        int i = 0;

        char temp[1000] = "/usr/bin/";
        char *arr[1000];
        j = 0;
    
        for (i ; command[i][0] != 124; i++){
            if (j == 0)
            {
                //printf("Hellllllooooo\n");
                strcat(temp, *(command+i));
                //printf("str ====  %s\n", temp);
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
        
        printf(">>>>>  1st Child  \n");
       //printf("a ========== %d\n", a);
        for (int u = 0; arr[u] != NULL; u++)
            {
                printf("%d   %s  \n", u, arr[u]);
            }

        int status1 = fork();

        if (status1 < 0)
        {
            printf("fork failed\n");
            exit(0);
        }
        else if (status1 == 0)
        {
            
            for (int e = 0; e < count; e++){
                if (e == 0)
                {
                    close(fd[q][0]);
                    close(fd[q][1]);
                }
            close(fd[0][0]);

            dup2(fd[0][1], STDOUT_FILENO);
            
            close(fd[0][1]);

            execv(temp, arr);
            }
        }
        printf("1111111111111\n");
        i++;
        q++;
        
       
        
        //printf("a before while ========== %d\n", a);
        
        for(int g = 0; g < (no_of_pipes-1); g++){
            printf("Innnnn for loop\n");

            j = 0;
            char temp2[1000] = "/usr/bin/";
            char *arr2[1000];

           // printf("Next command ========== %s\n ,,,,,,,,,,,,,, %s\n,,,,,,,,,,,,,,,,%s\n,,,,,,,,,,,,,,,,%s\n,,,,,,,,,,,,,,,,%s\n,,,,,,,,,,,,,,,,%s\n",*(command+2), *(command+3), *(command+4), *(command+5), *(command+6), *(command+7));

            for (i ; command[i][0] != 124; i++){
                if (j == 0)
                {
                    strcat(temp2, *(command+i));
                    strip(temp2);
                    //printf(" ========  %s\n", temp2);
                    arr2[j] = temp2;
                    j++;
                }
                else{
                    //printf("innnnn elseeee\n");
                    //printf("qqqqqqqqq = %d\n", command[i][0]);
                    arr2[j] = strdup(command[i]);
                    strip(arr2[j]);
                    j++;
                }
                //printf("i =====  %d\n", i);
            }
            //printf("Outside fol loop\n");
            arr2[j] = NULL;


            printf(">>>>>  2nd Child  \n");
            //printf("a ========== %d\n", a);
            for (int u = 0; arr2[u] != NULL; u++)
            {
                printf("%d   %s  \n", u, arr2[u]);
            }
            

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
                        close(fd[q][0]);
                        close(fd[q][1]);
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
            printf("222222222222222\n");
            i++;
            q++;
            
        }
//-------------------------------------------------------------------------
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


            printf(">>>>>  3rd Child  \n");
            //printf("a ========== %d\n", a);
            for (int u = 0; arr3[u] != NULL; u++)
            {
                printf("%d   %s  \n", u, arr3[u]);
            }
            

            int status3 = fork();

            if (status3 < 0)
            {
                printf("fork failed\n");
                exit(0);
            }
            else if (status3 == 0)
            {
                
                //dup2(fd[q][0], STDIN_FILENO);
                dup2(fd[q-1][0], STDIN_FILENO);

                for (int e = 0; e < count; e++){
                    if (e != q)
                    {
                        close(fd[q][0]);
                        close(fd[q][1]);
                    }
                }

                
                // if (q-1 >= count)
                // {
                //     close(fd[q][1]);
                //     close(fd[0][0]);
                // }else{
                //     close(fd[q][1]);
                //     close(fd[q+1][0]);
                // } 
                
                close(fd[q][1]);
                close(fd[q][0]);    
                execv(temp3, arr3);
            }
            printf("333333333333333\n");
            for (int z = 0; z < count; z++) {
                close(fd[z][0]);
                close(fd[z][1]);
            }

            for (int x = 0; x < count; x++) {
                wait(NULL);
            } 
            return;
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
        // printf("PIPES ===   %d  \n", count_pipes(tok));
    }
    return 0;
}
