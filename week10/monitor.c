#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

void print_stat_info(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat Info for %s:\n", path);
        printf("File Size: %lld bytes\n", (long long) st.st_size);
        printf("Number of Links: %ld\n", (long) st.st_nlink);
        printf("File inode: %ld\n", (long) st.st_ino);
        printf("File mode: %u\n", (unsigned) st.st_mode);
        printf("\n");
    } else {
        perror("stat");
    }
}

void print_directory_contents(const char *path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, dir->d_name);
                print_stat_info(fullpath);
            }
        }
        closedir(d);
    }
}

int keep_running = 1;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        keep_running = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_watch>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];

    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE | IN_ACCESS | IN_ATTRIB | IN_OPEN);

    signal(SIGINT, signal_handler);

    printf("Monitoring directory: %s\n", argv[1]);
    print_directory_contents(argv[1]);

    while (keep_running) {
        i = 0;
        length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            perror("read");
        }

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", argv[1], event->name);

                if (event->mask & IN_CREATE) {
                    printf("The file %s was created.\n", fullpath);
                } else if (event->mask & IN_DELETE) {
                    printf("The file %s was deleted.\n", fullpath);
                } else if (event->mask & IN_MODIFY) {
                    printf("The file %s was modified.\n", fullpath);
                }

                print_stat_info(fullpath);
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    printf("Final directory contents:\n");
    print_directory_contents(argv[1]);

    return 0;
}
