#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct juegos {
        char nombre[256];
        int nGenre;
        } juego;

int nGenres(FILE *fp){
    char linea[100];
    int cont = 0;
    char* token;
    int ngenres = 0;
    while (fgets(linea,100,fp) != NULL){
        if (cont==1){
            //printf("%s\n", linea);
            token = strtok(linea, ", ");
            
        while( token != NULL ){
            //printf("%s\n", token);
            token = strtok(NULL, " ");
            ngenres+=1;
        }
        }
        cont+=1;
    }
    return ngenres;
}

char* mainGenreGame(FILE *fp){
    char linea[100];
    int cont = 0;
    char* token;
    while (fgets(linea,100,fp) != NULL){
        if (cont==1){
            token = strtok(linea, "\n");
            token = strtok (token, ",");
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

int moveGame(FILE *fp, DIR *dr, char *gameName, char *genre){

    //char* genre = "Terror"; //cambiar por funcion
    int created = 0;
    int directoryExists = chdir(genre);
    if(directoryExists == -1){
        mkdir(genre, 0777);
        directoryExists = chdir(genre);
        created = 1;
    }
    chdir(".."); // Vuelve al parent por si acaso

    char finalName[256];
    strcpy(finalName, genre);
    strcat(finalName, "/"); 
    strcat(finalName, gameName); // MOBA/Lol.txt
    rename(gameName, finalName);
    return created;
    
}

char** genreNames(DIR *dr, int cantGenres){

    struct dirent *de;

    char* genreName;
    char** arrayGenres;
    arrayGenres = malloc(sizeof(char*) * cantGenres);

    int cont = 0;

    char* buffer;

    while ((de = readdir(dr)) != NULL){

        buffer = de->d_name;
        char uno;
        uno = buffer[0];
        if(uno != '.'){
            genreName = malloc(sizeof(char) * 256); // FREE
            strcpy(genreName, buffer);
            arrayGenres[cont] = genreName;
            cont++;
        }
        
    } 

    return arrayGenres;
}

void freeArrayStrings(char** array, int cant){

    int i;
    for(i = 0; i < cant; i++){
        free(array[i]);
    }
    free(array);

}

int nGames(DIR *dr){

    struct dirent *de;
    int amount = 0;
    while ((de = readdir(dr)) != NULL){
            
            if(de->d_type == DT_REG){
                amount++;
            }
    } 
    rewinddir(dr); // Resetea el punteo dir
    return amount;
}

char** gameNamesArray(DIR *dr, int nGames){ // Esto esta malo
    
    char* juegoName;
    struct dirent *de;
    char* jName;
    int i = 0;
    char** arrayNames = malloc(sizeof(char*) * nGames);
    while ((de = readdir(dr)) != NULL){
            if(de->d_type == DT_REG){
                juegoName = de->d_name;
                juegoName = strtok(juegoName, ".txt");
                jName = malloc(sizeof(char) * 256); // FREE
                strcpy(jName, juegoName);
                arrayNames[i] = jName;
                i++;
            }
    } 
    rewinddir(dr); // Resetea el punteo dir

    return arrayNames;
}

int* nGenreGameArray(DIR *dr, int nGames){ //Free solo
    
    FILE *fp;
    struct dirent *de;
    int i = 0;
    int* arrayCant = malloc(sizeof(int) * nGames);
    char* gameName;
    
    while ((de = readdir(dr)) != NULL){
            if(de->d_type == DT_REG){
                gameName = de->d_name;
                fp = fopen(gameName, "r");
                arrayCant[i] = nGenres(fp);
                i++;
            }
    } 
    rewinddir(dr); // Resetea el punteo dir
    return arrayCant;
}

char** juegosOrdenados(char** nombres, int* generos, int cantJuegos){
    
    int maximo = 0;
    int i;
    for(i = 0; i < cantJuegos; i++){
        if (maximo < generos[i]){
            maximo = generos[i];
        }
    }
    

    char** arrayJuegos = malloc(sizeof(char*) * cantJuegos);
    //juegoName = malloc(sizeof(int)); // FREE
    
    int cont = maximo;
    int indice = 0;
    // Bien las variables hasta aca
    
    while(cont >= 0){
        
        for(i = 0; i < cantJuegos; i++){
            if (cont == generos[i]){
                char* jName = malloc(sizeof(char) * 256);
                strcpy(jName, nombres[i]);
                arrayJuegos[indice] = jName;
                indice++;
            }
        }
        cont--;
    }

    return arrayJuegos;
    
}

void showGenre(char* genre){
    chdir(genre);
    DIR *dr = opendir("."); 
    int nJuegos = nGames(dr);
    char** gameNames = gameNamesArray(dr, nJuegos); // Aca bien
    int* cantJuegosArray = nGenreGameArray(dr, nJuegos); // aca bien
    char** orderedGames = juegosOrdenados(gameNames, cantJuegosArray, nJuegos); // aca mal :c
    freeArrayStrings(gameNames, nJuegos);
    free(cantJuegosArray);

    int i;
    for(i = 0; i< nJuegos; i++){
        printf("%d - %s\n", i+1, orderedGames[i]);
    }

    

    chdir(".."); //Vuelve al parent
}

void runConsole(DIR *dr, int cantGenres){

    char** arrayGenres;
    arrayGenres = genreNames(dr, cantGenres);

    int run = 1;
    int input;
    while(run){
        int i;
        printf("Generos\n");
        for(i = 0; i < cantGenres; i++){
            printf("%d.- %s\n", i+1,arrayGenres[i]);

        }
        printf("0.- Salir\n");
        scanf("%d", &input);

        if(input == 0){
            printf("Saliendo,\n");
            break;
        }
        else if(input > cantGenres){
            printf("Input incorrecto.\n");
        }
        else if(input < 0){
            printf("Input incorrecto.\n");
        }
        else{
            showGenre(arrayGenres[input-1]); // Aca bien
        }

        
        
        //Leemos las carpetas
        // 1.- carpeta
        // 2.- carpeta
        // 3.- carpeta
    }
}


int main(void) {

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
    int cantGenres = 0;
    while ((de = readdir(dr)) != NULL){
            
            juegoName = de->d_name;
            char uno;
            uno = juegoName[0];
            if(uno != '.'){

                fp = fopen(juegoName, "r");
                genre = mainGenreGame(fp);
                cantGenres = cantGenres + moveGame(fp, dr, juegoName, genre);

                fclose(fp);
            }
    } 


    rewinddir(dr); // Resetea el punteo dir

    
    runConsole(dr, cantGenres);


    


    return 0; 
} 