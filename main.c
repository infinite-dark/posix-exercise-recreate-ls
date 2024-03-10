#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

void printType(struct stat *s) {
    mode_t mode = s->st_mode;
    if      (S_ISDIR(mode))
        printf("d");
    else if (S_ISREG(mode))
        printf("-");
    else if (S_ISLNK(mode))
        printf("l");
    else
        printf("?");
}

void printPermissions(struct stat *s) {
    mode_t mode = s->st_mode;
    const char permissions[] = "rwxrwxrwx";
    for (size_t i = 0; i < 9; i++) {
        char c = (mode & (1 << (8-i))) ? permissions[i] : '-';
        printf("%c", c);
    }
    printf("%4lu", s->st_nlink);
}

void printOwners(struct stat *s) {
    struct passwd *p;
    struct group *g;

    p = getpwuid(s->st_uid);
    g = getgrgid(s->st_gid);

    printf("%12s %12s", p->pw_name, g->gr_name);
}

void printSize(struct stat *s) {
    printf("%12ld", s->st_size);
}

void printModificationTime(struct stat *s) {
    printf("%12s", ctime(&(s->st_mtime)));
}

void ls(DIR* directory) {
    unsigned long int total = 0;
    struct dirent* file;
    while (file = readdir(directory)) {
        struct stat s;
        stat(file->d_name, &s);
        total++;

        printType(&s);
        printPermissions(&s);
        printOwners(&s);
        printSize(&s);
        printf("%40s\t", file->d_name);
        printModificationTime(&s);
    }
    printf("total %lu\n", total);
}

int main(int argc, char **argv) {

    DIR* directory = NULL;

    if (argc > 1)
        directory = opendir(argv[1]);
    else
        directory = opendir(".");

    if (directory != NULL)
        ls(directory);
    else
        printf("Error opening the directory.\n");

    closedir(directory);

    return 0;
}
