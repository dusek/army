#ifndef ARMY_CORE_SYSCALLS_H_
#define ARMY_CORE_SYSCALLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define stdin 0
#define stdout 1
#define stderr 2

int open(const char *path, int mode);
void close(int fd);
int read(int fd, char *buf, int bytes);
int write(int fd, const char *buf, int bytes);
void exit(void);

#ifdef __cplusplus
}
#endif


#endif

