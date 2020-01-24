//
//  csapp.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/24.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef csapp_hpp
#define csapp_hpp
#include <iostream>
#include <string>

extern "C"{
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
}

#include "serverlog.hpp"

//默认文件权限 DEF_MODE & ~DEF_UMASK
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024

//别名
using SA = struct sockaddr;

//缓冲区结构
#define RIO_BUFSIZE 8192
typedef struct{
    int rio_fd; //文件描述符
    int rio_cnt; //缓冲区内剩余未读字节
    char* rio_bufptr; //缓冲区内下一个未读字节
    char rio_buf[RIO_BUFSIZE]; //缓冲区
}rio_t;




class Csapp{
    public:
        //进程控制类函数
        pid_t Fork();
        pid_t Wait(int* status);
        pid_t Waitpid(pid_t pid, int* iptr, int options);
        pid_t Getpgrp();
        void Kill(pid_t pid, int signum);
        unsigned int Sleep(unsigned int secs);
        void Pause(void);
        unsigned int Alarm(unsigned int seconds);
        void Setpgid(pid_t pid, pid_t pgid);
        void Execve(const char* filename, char* const argv[], char* const envp[]);
    
        //信号处理类函数
        using handler_t = void (int);
        handler_t* Signal(int signum, handler_t* handler);
        void Sigprocmask(int how, const sigset_t* set, sigset_t* oldset);
        void Sigemptyset(sigset_t* set);
        void Sigfillset(sigset_t* set);
        void Sigaddset(sigset_t* set, int signum);
        void Sigdelset(sigset_t* set, int signum);
        int Sigismember(const sigset_t* set, int signum);
    
        //IO流函数
        FILE* Fopen(const char* filename, const char* mode);
        FILE* Fdopen(int fd, const char* type);
        size_t Fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
        char* Fgets(char* ptr, int n, FILE* stream);
        void Fclose(FILE* fp);
        void Fputs(const char* ptr, FILE* stream);
        void Fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
    
        //IO函数
        int Open(const char *pathname, int flags, mode_t mode);
        ssize_t Read(int fd, void *buf, size_t count);
        ssize_t Write(int fd, const void *buf, size_t count);
        off_t Lseek(int fildes, off_t offset, int whence);
        void Close(int fd);
        int Select(int  n, fd_set *readfds, fd_set *writefds,
                   fd_set *exceptfds, struct timeval *timeout);
        int Dup2(int fd1, int fd2);
        void Stat(const char *filename, struct stat *buf);
        void Fstat(int fd, struct stat *buf);
    
        //动态空间分配函数
        void* Malloc(size_t size);
        void* Realloc(void* ptr, size_t size);
        void* Calloc(size_t nmemb, size_t size);
        void Free(void* ptr);
    
        //内存映射
        void *Mmap(void *addr, size_t len, int prot,
                   int flags, int fd, off_t offset);
        void Munmap(void *start, size_t length);
    
        //socket函数
        int Socket(int domain, int type, int protocol);
        void Setsockopt(int s, int level, int optname, const void* optval, int optlen);
        void Bind(int sockfd, struct sockaddr* my_addr, int addrlen);
        void Listen(int s, int backlog);
        int Accept(int s, struct sockaddr* addr, socklen_t* addrlen);
        void Connect(int sockfd, struct sockaddr* serv_addr, int addrlen);
    
        //DNS函数
        struct hostent* Gethostbyname(const char* name);
        struct hostent *Gethostbyaddr(const char *addr, int len, int type);
    
        //线程控制函数
        void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                            void * (*routine)(void *), void *argp);
        void Pthread_join(pthread_t tid, void **thread_return);
        void Pthread_cancel(pthread_t tid);
        void Pthread_detach(pthread_t tid);
        void Pthread_exit(void *retval);
        void Pthread_once(pthread_once_t *once_control, void (*init_function)());
        pthread_t Pthread_self(void);
    
        //信号量函数
        void Sem_init(sem_t *sem, int pshared, unsigned int value);
        void P(sem_t *sem);
        void V(sem_t *sem);
    
        //Rio函数
        ssize_t rio_readn(int fd, void *usrbuf, size_t n);
        ssize_t rio_writen(int fd, void *usrbuf, size_t n);
        ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
        ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
        ssize_t Rio_readn(int fd, void *ptr, size_t nbytes);
        ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
        ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
        void Rio_writen(int fd, void *usrbuf, size_t n);
        void Rio_readinitb(rio_t *rp, int fd);
        void rio_readinitb(rio_t *rp, int fd);

        int Open_clientfd(char *hostname, int port);
        int Open_listenfd(int port);
    
private:
        int open_clientfd(char *hostname, int portno);
        int open_listenfd(int portno);
    
    
    

    
    
};
#endif /* csapp_hpp */
