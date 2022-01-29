#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stddef.h>
#include <errno.h>

int (*orig__xstat)(int ver, const char* path, struct stat* stat_buf);

int __xstat(int ver, const char* path, struct stat* stat_buf)
{
    if (!orig__xstat)
    {
       orig__xstat = dlsym(RTLD_NEXT, "__xstat");
    }
    // Get the path to the home directory.
    char *homedir = getenv("HOME");
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw != NULL) 
    {
        if (homedir == NULL)
        {
           homedir = pw->pw_dir;
           
        }
        else
        {
           errno = EACCES;
           return -1;
        }
    }
    if (!strncmp(path, homedir, strlen(homedir)))
    {
       return orig__xstat(ver, path, stat_buf);
    }
    errno = EACCES;
    return -1;
}
