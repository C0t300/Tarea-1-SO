#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

char* mainGenreGame(FILE *fp){
    char linea[100];
    int cont = 0;
    char* token;
    while (fgets(linea,100,fp) != NULL){
        if (cont==1){
            token = strtok(linea, "\n");
            token = strtok (linea, ", ");
        while( token != NULL ) {
            //printf( " %s\n", token );
            return(token);
            token = strtok(NULL, " ");
        }
        }
        cont++;
    }
    return "0";
}

void moveGame(FILE *fp, DIR *dr, char *gameName, char *genre){

    //char* genre = "Terror"; //cambiar por funcion
    int directoryExists = chdir(genre);
    if(directoryExists == -1){
        mkdir(genre, 0777);
        directoryExists = chdir(genre);
    }
    chdir(".."); // Vuelve al parent por si acaso

    char finalName[256];
    strcpy(finalName, genre);
    strcat(finalName, "/");
    strcat(finalName, gameName);
    rename(gameName, finalName);
    
}

int main(void) 
{ 
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    chdir("Juegos"); //esto usa unistd
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // for readdir() 

    FILE *fp;



    char* juegoName;
    char* genre;
    while ((de = readdir(dr)) != NULL){
            
            juegoName = de->d_name;
            char uno;
            uno = juegoName[0];
            if(uno != '.'){

                fp = fopen(juegoName, "r");
                genre = mainGenreGame(fp);
                moveGame(fp, dr, juegoName, genre);
                fclose(fp);
            }
    } 
  
    fp = fopen("amongas.txt", "r");
    closedir(dr);     
    return 0; 
} 