#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// char * read_inp(void){
//         int i = 0;
//         int size = 2048;
//         char * input = (char *)malloc(sizeof(char) * 2048);
//         char c;

//         // Read input by char by char


//         c = getchar();
//         do
//         {
//             if (c != '\n')
//             {
//                 input[i] = c;

//                 // Reallocate buffer as and when needed
//                 if (i >= size){
//                         size += 64;
//                         input = realloc(input, size);
//                 }

//                 i++;
//                 c = getchar();
//             }
            
//         } while (c != EOF && c!='\n');
        
//         // while (c != EOF && c != '\n'){
//         //         input[i] = c;

//         //         // Reallocate buffer as and when needed
//         //         if (iition >= buf_size){
//         //                 buf_size += 64;
//         //                 command = realloc(command, buf_size);
//         //         }

//         //         iition++;
//         //         c = getchar();
//         // }
//         return input;
// }

char ** create_tokens(char input){
    int no_of_tokens = 1024;
    int i = 0;
    char **tokens = malloc(sizeof(char)* no_of_tokens);
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
    
    while (1)
    {
        printf(">> ");

        fgets(input, size , stdin);
        char str[] = "exit\n";
        // printf("%ld     %d\n",sizeof(input), size);

        // if (sizeof(input) > sizeof(char)*size)
        // {
        //     size += 1024;
        //     input = realloc(input, size);
        // }
       // printf("%ld     %d",sizeof(input), size);
    
        //printf("%d\n",strcmp(input,str));

        if (strcmp(input, str) == 0) {
            printf("innn\n");
            break;
        }

        printf("You entered: %s\n", input);

        create_tokens(input);
    }
    return input;
    
}




int main(int argc, char const *argv[])
{
    // while (1)
    // {
    //     char * ch = read_inp();
    //     // printf("%s\n",ch);
    // }
    read_inp();

    
    
    return 0;
}

