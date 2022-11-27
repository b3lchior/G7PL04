#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <string.h>

#define BUFFER_SIZE 1024


int main(int argc, char* argv[]) {
    char* filename; 
    char* zip_argument_list[1024] = {"zip", "ebooks.zip"};
    for ( int i = 1; i < argc ; i++){
        filename = (char *) malloc(sizeof(argv[i]) +1);
        strcpy(filename, argv[i]); 
        filename[strlen(filename) - 4 ] = '\0';
        strcat(filename ,".epub"); 
        zip_argument_list[i+1] = filename;
    

        pid_t pid = fork();
        if (pid == -1){
            perror("fork failure");
            return EXIT_FAILURE;
        }
        else if (pid == 0){
          
            
            
            char* epub_argument_list[1024] = {"pandoc", argv[i] , "-o" , filename , NULL} ; 
          
            if (execvp("pandoc" , epub_argument_list ) == -1){
                perror("execvp failure");
                return EXIT_FAILURE;
            }
        }
        else {
            printf("[%i] converting %s ...\n", pid, filename);
            if(waitpid(pid , NULL , 0) == -1){
                perror("wait failure");
                return EXIT_FAILURE;
            }
        }
    
    
    
    }
    
    zip_argument_list[argc + 1] = NULL;
    if (execvp("zip", zip_argument_list) == -1) {
        perror("zip failure");
        return EXIT_FAILURE;
    }
    free(filename);
    return EXIT_SUCCESS;
}