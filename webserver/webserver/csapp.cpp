//
//  csapp.cpp
//  webserver
//
//  Created by dong da kuan on 2020/1/24.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#include "csapp.hpp"

pid_t Csapp::Fork(){
    pid_t pid;
    if((pid = fork()) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return pid;
}

void Csapp::Execve(const char* filename, char *const *argv, char *const *envp){
    if(execve(filename, argv, envp) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

pid_t Csapp::Wait(int *status){
    pid_t pid;
    
    if((pid = wait(status)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return pid;
}

pid_t Csapp::Waitpid(pid_t pid, int *iptr, int options)
{
    pid_t retpid;
    
    if ((retpid  = waitpid(pid, iptr, options)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return(retpid);
}

void Csapp::Kill(pid_t pid, int signum)
{
    int rc;
    
    if ((rc = kill(pid, signum)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Pause()
{
    (void)pause();
    return;
}

unsigned int Csapp::Sleep(unsigned int secs)
{
    int rc;
    
    if ((rc = sleep(secs)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

unsigned int Csapp::Alarm(unsigned int seconds) {
    return alarm(seconds);
}

void Csapp::Setpgid(pid_t pid, pid_t pgid) {
    int rc;
    
    if ((rc = setpgid(pid, pgid)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return;
}

pid_t Csapp::Getpgrp(void) {
    return getpgrp();
}

Csapp::handler_t* Csapp::Signal(int signum, Csapp::handler_t *handler)
{
    struct sigaction action, old_action;
    
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */
    
    if (sigaction(signum, &action, &old_action) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return (old_action.sa_handler);
}

void Csapp::Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return;
}

void Csapp::Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return;
}

void Csapp::Sigfillset(sigset_t *set)
{
    if (sigfillset(set) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return;
}

void Csapp::Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return;
}

void Csapp::Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return;
}

int Csapp::Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) != true){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

int Csapp::Open(const char *pathname, int flags, mode_t mode)
{
    int rc;
    
    if ((rc = open(pathname, flags, mode))  < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

ssize_t Csapp::Read(int fd, void *buf, size_t count){
    ssize_t rc;
    
    if ((rc = read(fd, buf, count)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

ssize_t Csapp::Write(int fd, const void *buf, size_t count){
    ssize_t rc;
    
    if ((rc = write(fd, buf, count)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

off_t Csapp::Lseek(int fildes, off_t offset, int whence){
    off_t rc;
    
    if ((rc = lseek(fildes, offset, whence)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

void Csapp::Close(int fd){
    int rc;
    
    if ((rc = close(fd)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

int Csapp::Select(int n, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout){
    int rc;
    
    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return rc;
}

int Csapp::Dup2(int fd1, int fd2){
    int rc;
    
    if ((rc = dup2(fd1, fd2)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return rc;
}

void Csapp::Stat(const char *filename, struct stat *buf){
    if (stat(filename, buf) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Fstat(int fd, struct stat *buf){
    if (fstat(fd, buf) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void* Csapp::Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void *ptr;
    
    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1)){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return(ptr);
}

void Csapp::Munmap(void *start, size_t length){
    if(munmap(start, length) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void* Csapp::Malloc(size_t size)
{
    void *p;
    
    if ((p  = malloc(size)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return p;
}

void* Csapp::Realloc(void *ptr, size_t size)
{
    void *p;
    
    if ((p  = realloc(ptr, size)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return p;
}

void* Csapp::Calloc(size_t nmemb, size_t size)
{
    void *p;
    
    if ((p = calloc(nmemb, size)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return p;
}

void Csapp::Free(void *ptr)
{
    free(ptr);
}

void Csapp::Fclose(FILE *fp)
{
    if (fclose(fp) != 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

FILE* Csapp::Fdopen(int fd, const char *type)
{
    FILE *fp;
    
    if ((fp = fdopen(fd, type)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return fp;
}

char* Csapp::Fgets(char *ptr, int n, FILE *stream)
{
    char *rptr;
    
    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream)){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return rptr;
}

FILE* Csapp::Fopen(const char *filename, const char *mode)
{
    FILE *fp;
    
    if ((fp = fopen(filename, mode)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return fp;
}

void Csapp::Fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

size_t Csapp::Fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t n;
    
    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream)){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return n;
}

void Csapp::Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (fwrite(ptr, size, nmemb, stream) < nmemb){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

int Csapp::Socket(int domain, int type, int protocol)
{
    int rc;
    
    if ((rc = socket(domain, type, protocol)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

void Csapp::Setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
    int rc;
    
    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
    int rc;
    
    if ((rc = bind(sockfd, my_addr, addrlen)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Listen(int s, int backlog)
{
    int rc;
    
    if ((rc = listen(s,  backlog)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

int Csapp::Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;
    
    if ((rc = accept(s, addr, addrlen)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    
    return rc;
}

void Csapp::Connect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
    int rc;
    
    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

struct hostent* Csapp::Gethostbyname(const char *name)
{
    struct hostent *p;
    
    if ((p = gethostbyname(name)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return p;
}

struct hostent* Csapp::Gethostbyaddr(const char *addr, int len, int type)
{
    struct hostent *p;
    
    if ((p = gethostbyaddr(addr, len, type)) == NULL){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return p;
}

void Csapp::Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void * (*routine)(void *), void *argp)
{
    int rc;
    
    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Pthread_cancel(pthread_t tid) {
    int rc;
    
    if ((rc = pthread_cancel(tid)) != 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Pthread_join(pthread_t tid, void **thread_return) {
    int rc;
    
    if ((rc = pthread_join(tid, thread_return)) != 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}


void Csapp::Pthread_detach(pthread_t tid) {
    int rc;
    
    if ((rc = pthread_detach(tid)) != 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}


void Csapp::Pthread_exit(void *retval) {
    pthread_exit(retval);
}

pthread_t Csapp::Pthread_self(void) {
    return pthread_self();
}

void Csapp::Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
    pthread_once(once_control, init_function);
}

void Csapp::Sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::P(sem_t *sem)
{
    if (sem_wait(sem) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::V(sem_t *sem)
{
    if (sem_post(sem) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

ssize_t Csapp::rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char*)usrbuf;
    
    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
                return -1;      /* errno set by read() */
        }
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

ssize_t Csapp::rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char*)usrbuf;
    
    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /* Interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            else
                return -1;       /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;
    
    while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
                           sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) /* Interrupted by sig handler return */
                return -1;
        }
        else if (rp->rio_cnt == 0)  /* EOF */
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }
    
    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

void Csapp::rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t Csapp::rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char*)usrbuf;
    
    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;          /* errno set by read() */
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

ssize_t Csapp::rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = (char*)usrbuf;
    
    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        } else
            return -1;      /* Error */
    }
    *bufp = 0;
    return n-1;
}

ssize_t Csapp::Rio_readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;
    
    if ((n = rio_readn(fd, ptr, nbytes)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return n;
}

void Csapp::Rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
}

void Csapp::Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

ssize_t Csapp::Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t rc;
    
    if ((rc = rio_readnb(rp, usrbuf, n)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

ssize_t Csapp::Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;
    
    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}

int Csapp::open_clientfd(char *hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1; /* Check errno for cause of error */
    
    /* Fill in the server's IP address and port */
    if ((hp = gethostbyname(hostname)) == NULL)
        return -2; /* Check h_errno for cause of error */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);
    
    /* Establish a connection with the server */
    if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    return clientfd;
}

int Csapp::open_listenfd(int port)
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
    
    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    
    /* Eliminates "Address already in use" error from bind */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
        return -1;
    /* Listenfd will be an endpoint for all requests to port
     on any IP address for this host */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
        return -1;
    return listenfd;
}

int Csapp::Open_clientfd(char *hostname, int port)
{
    int rc;
    
    if ((rc = open_clientfd(hostname, port)) < 0) {
        if (rc == -1){
            LOGE("open clientfd unix error %s error! %s",__func__,strerror(errno));
            exit(1);
        }
        else{
            LOGE("open clientfd dns error %s error! %s",__func__,strerror(errno));
            exit(1);
        }
    }
    return rc;
}

int Csapp::Open_listenfd(int port)
{
    int rc;
    
    if ((rc = open_listenfd(port)) < 0){
        LOGE("%s error! %s",__func__,strerror(errno));
        exit(1);
    }
    return rc;
}
