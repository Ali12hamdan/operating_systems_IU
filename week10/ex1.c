#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void find_all_hlinks(const char *source, const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat source_stat, entry_stat;
    char fullpath[PATH_MAX];

    if (stat(source, &source_stat) != 0) {
        perror("stat");
        return;
    }

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (stat(fullpath, &entry_stat) == 0) {
            if (S_ISREG(entry_stat.st_mode) && entry_stat.st_ino == source_stat.st_ino) {
                char resolved_path[PATH_MAX];
                realpath(fullpath, resolved_path);
                printf("Hard link: %s (inode: %ld)\n", resolved_path, (long)entry_stat.st_ino);
            }
        }
    }

    closedir(dir);
}

void unlink_all(const char *source, const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat source_stat, entry_stat;
    char fullpath[PATH_MAX];
    int first = 1;

    if (stat(source, &source_stat) != 0) {
        perror("stat");
        return;
    }

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (stat(fullpath, &entry_stat) == 0) {
            if (!first && S_ISREG(entry_stat.st_mode) && entry_stat.st_ino == source_stat.st_ino) {
                unlink(fullpath);
            } else {
                first = 0;
            }
        }
    }

    closedir(dir);
}

void create_sym_link(const char *source, const char *link) {
    if (symlink(source, link) != 0) {
        perror("symlink");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char path[PATH_MAX];
    realpath(argv[1], path);

    // Create myfile1.txt and its hard links
    char file1[PATH_MAX], link1[PATH_MAX], link2[PATH_MAX];
    snprintf(file1, sizeof(file1), "%s/myfile1.txt", path);
    snprintf(link1, sizeof(link1), "%s/myfile11.txt", path);
    snprintf(link2, sizeof(link2), "%s/myfile12.txt", path);

    FILE *fp = fopen(file1, "w");
    if (fp) {
        fprintf(fp, "Hello world.");
        fclose(fp);
    }

    link(file1, link1);
    link(file1, link2);

    find_all_hlinks(file1, path);

    // Move myfile1.txt
    char new_location[PATH_MAX];
    snprintf(new_location, sizeof(new_location), "/tmp/myfile1.txt");
    rename(file1, new_location);

    // Modify myfile11.txt
    fp = fopen(link1, "w");
    if (fp) {
        fprintf(fp, "Modified content.");
        fclose(fp);
    }

    // Create a symbolic link
    char sym_link[PATH_MAX];
    snprintf(sym_link, sizeof(sym_link), "%s/myfile13.txt", path);
    create_sym_link(new_location, sym_link);

    // Modify the file at /tmp/myfile1.txt
    fp = fopen(new_location, "w");
    if (fp) {
        fprintf(fp, "Another modification.");
        fclose(fp);
    }

    // Remove duplicates of hard links
    unlink_all(link1, path);

    return 0;
}
