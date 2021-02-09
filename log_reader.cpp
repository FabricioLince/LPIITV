#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *lsdir;

    char path[] = "log";

    dir = opendir(path);

    int contador = 0;
    FILE* file;

    /* print all the files and directories within directory */
    while ( ( lsdir = readdir(dir) ) != NULL )
    {
        //printf("%s\n", lsdir->d_name);
        char pathToRemove[strlen(path)+strlen(lsdir->d_name)+4];
        strcpy(pathToRemove, path);
        strcat(pathToRemove, "/");
        strcat(pathToRemove, lsdir->d_name);
        if (file = fopen(pathToRemove, "r"))
        {
            fclose(file);
            contador++;
            printf("Removing %s\n",  pathToRemove);
            remove(pathToRemove);
        }
    }
    printf("Removed %d files...\n", contador);

    closedir(dir);

    return 0;

}
