# Limiting the Scope of ls
I chose to override the __xstat() function as it can deal with both directory & file accesses, while opendir() & readdir() only deal with the former.
```
$ /bin/ls /etc
backups  cache	crash  lib  local  lock  log  mail  metrics  opt  run  snap  spool  tmp
$ gcc -o ls-limited.so -fPIC -shared -D_GNU_SOURCE home_stat.c -ldl
$ $ LD_PRELOAD=`pwd`/ls-limited.so ./ls-modified /var
./ls-modified: cannot access '/var': Permission denied
```