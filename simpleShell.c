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
    
    // while (1)
    // {
        printf(">> ");

        fgets(input, size , stdin);
        //char str[] = "exit\n";
  

        // if (strcmp(input, str) == 0) {
        //     printf("innn\n");
        //     //break;
        // }

        printf("You entered: %s\n", input);

        
   // }
    return input;
    
}

char * check_command(char ** command){
    if (strcmp(*(command+0), "ls\n") == 0)
    {
        printf("innn ls\n");
        execl("/usr/bin/ls", "/usr/bin/ls",  NULL);
        printf("after ls\n");
    }
    
    if (strcmp(*(command+0), "echo") == 0)
    {
        char * arr[sizeof(command)/sizeof(*(command+0))];
        printf("%ld       %ld", sizeof(command), sizeof(*(command+0)));
        for (int i = 1; i < arr[i] != NULL; i++)
        {
            arr[i] = *(command+i);
        }
        
        // printf("innn echo\n");
        // execl("/usr/bin/echo", "/usr/bin/echo", "Hello I am Sujal",  NULL);
        // printf("\nafter echo\n");
        for (int j = 0; j < arr[j] != NULL; j++)
        {
            printf("%s ", *(arr+j));
        }
        
    }
    //printf("%d \n", strcmp(*(command+0), "sujal"));
    return command[0];
}

int main(int argc, char const *argv[])
{
    while (1)
    {
        char * inp = read_inp();
        if (strcmp(inp, "exit\n") == 0) {
            //printf("innn\n");
            break;
        }
        char ** tok = create_tokens(inp);
        printf(">>>>>>>>>>>>>>>>>   %ld\n", sizeof(tok));
        char *test = check_command(tok);
    }
    
    


    return 0;
}

