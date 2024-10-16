#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STRING_LEN 500

int main(int noArgs,char **args){

    if(noArgs != 4) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    const char *path = args[1];
    FILE *fileToRead = fopen(path,"r+");

    if (fileToRead == NULL) {
        printf("no fue posible abrir el archivo %s\n",path);
        return 1;
    }

    remove("impares.txt");
    remove("pares.txt");

    FILE *paresFiles = fopen("impares.txt","wa");
    FILE *imparesFile = fopen("pares.txt","wa");

    if (paresFiles == NULL || imparesFile == NULL) {
        perror("no fue posible los archivos destino");
        return 1;
    }

    int index = 1;

    char line[STRING_LEN];
    while(fgets(line,STRING_LEN,fileToRead)) {
        if(index % 2 == 0) {
            // pares
            fwrite(line,strlen(line),1,paresFiles);
        } else {
            // impars
            fwrite(line,strlen(line),1,imparesFile);
        }
        index++;
    }

    fclose(fileToRead);

    char *g1 = args[2];
    char *g2 = args[3];

    char formatedOne[1024];
    char formatedTwo[1024];

    sprintf(formatedOne,"cat impares.txt | grep %s | sort",g1);
    sprintf(formatedTwo,"cat impares.txt | grep %s | sort",g2);

    printf("\n\n\n ***** IMPARES con argumento %s \n\n\n",g1);
    system(formatedOne);
    printf("\n\n\n ***** PARES con argumento %s\n\n\n",g2);
    system(formatedTwo);
    return 0;
}

