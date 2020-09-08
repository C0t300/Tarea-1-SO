#include <stdio.h> 
#include <dirent.h> 

int main(void) 
{ 
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("Juegos"); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 
    int cont = 0;
    char* buffer;
    while ((de = readdir(dr)) != NULL){
            buffer = de->d_name;
            char uno;
            uno = buffer[0];
            if(&uno != "."){
                printf("%d - %s\n", cont, buffer);
                cont++;
            }
    } 
  
    closedir(dr);     
    return 0; 
} 