#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dirent.h>
#endif

struct directory {
    char *path;
    
    // for iteration.
    int is_directory;
    char full_path[512];
    char name[256];
    
#ifdef _WIN32
    WIN32_FIND_DATA find_data;
    HANDLE handle;
#endif

#ifdef __linux__
    DIR *handle;
#endif
};

#define in_directory(path) \
for (struct directory directory = directory_open(path); directory_next(&directory);) \
if (strcmp(directory.name, ".") != 0 && strcmp(directory.name, "..") != 0)

struct directory directory_open(char *path);
int directory_next(struct directory *directory);
int directory_create(char *path);
