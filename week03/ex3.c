#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 2048
#define MAX_FILENAME 63
#define MAX_FILE_SIZE 1024
#define MAX_FILES 256
#define MAX_DIRECTORIES 256

typedef struct File {
    int id;
    char name[MAX_FILENAME];
    int size;
    char data[MAX_FILE_SIZE];
    struct Directory* directory;
} File;

typedef struct Directory {
    char name[MAX_FILENAME];
    File *files[MAX_FILES];
    struct Directory* directories[MAX_DIRECTORIES];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
} Directory;

void overwrite_to_file(struct File* file, const char* str) {
    strcpy(file->data, str);
    file->size = strlen(str);
}

void append_to_file(struct File* file, const char* str) {
    strcat(file->data, str);
    file->size += strlen(str);
}

void printp_file(struct File* file) {
    printf("%s/%s\n", file->directory->path, file->name);
}

void add_file(struct File* file, struct Directory* dir) {
    dir->files[dir->nf] = file;
    dir->nf++;
    file->directory = dir;
}

void add_dir(struct Directory* dir1, struct Directory* dir2) {
    if (dir1 && dir2) {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
        char tempPath[MAX_PATH];
        if (strcmp(dir2->path, "/")) {
            strcpy(tempPath, dir2->path);
            strcat(tempPath, "/");
            strcat(tempPath, dir1->name);
            strcpy(dir1->path, tempPath);
        } else {
            strcpy(tempPath, "/");
            strcat(tempPath, dir1->name);
            strcpy(dir1->path, tempPath);
        }
    }
}

int main() {
    Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");

    Directory home;
    strcpy(home.name, "home");
    add_dir(&home, &root);

    Directory bin;
    strcpy(bin.name, "bin");
    add_dir(&bin, &root);

    File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");

    File ex3_1;
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");

    File ex3_2;
    ex3_2.id = 3;
    strcpy(ex3_2.name, "ex3_2.c");

    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    add_file(&ex3_1, &home);

    
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    add_file(&ex3_2, &home);

    overwrite_to_file(&bash, "Bourne Again Shell!!");
    add_file(&bash, &bin);

    
    

    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    printf("Path of all files:\n");
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    return 0;
}
