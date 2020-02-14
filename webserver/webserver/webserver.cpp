//
//  webserver.cpp
//  webserver
//
//  Created by dong da kuan on 2020/1/22.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#include "webserver.hpp"
#include "serverlog.hpp"

Webserver* Webserver::instance = new Webserver();
class Webserver::interface{
    private:
        Csapp app;
    public:
        void doit(int fd){
            int is_static;//是否为静态请求
            struct stat sbuf;
            char buf[MAXLINE], method[MAXLINE], url[MAXLINE], version[MAXLINE];
            char filename[MAXLINE], cgiargs[MAXLINE];
            rio_t rio;
            
            //读取客户端http请求
            app.Rio_readinitb(&rio, fd);
            app.Rio_readlineb(&rio, buf, MAXLINE);
            LOGD("readline b :%s",buf);
            sscanf(buf, "%s %s %s",method, url, version);
            
            LOGI("buf:%s,method:%s,url:%s,version:%s",buf,method,url,version);
            if(!strcasecmp(method, GET) && !strcasecmp(method, POST)){
                clienterror(fd, filename, "501", "Not Implemented",
                            "Web Server does not implement this method");
                return;
            }
            
            if(strcasecmp(method, GET)){
                read_requesthdrs(&rio, GET,cgiargs);
            }
            if (strcasecmp(method, POST)) {
                read_requesthdrs(&rio, POST, cgiargs);
            }
            
            is_static = parse_url(url, filename, cgiargs);
            if(stat(filename, &sbuf) < 0){
                clienterror(fd, filename, "404", "Not Found",
                            "Web Server couldn't find this file");
                return;
            }
            
            if(is_static){
                //处理静态请求
                if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){
                    clienterror(fd, filename, "403", "Forbidden",
                                "Web Server couldn't read the file");
                    return;
                }
                
                serve_static(fd, filename, sbuf.st_size);
            }else{
                //处理动态请求
                if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){
                    clienterror(fd, filename, "403", "Forbidden",
                                "Web Server couldn't run the CGI program");
                    return;
                }
                
                serve_dynamic(fd, filename, cgiargs);
            }
        }
    
        void read_requesthdrs(rio_t* rp,char* method,char* cgiargs){
            char buf[MAXLINE];
        
            if(method == GET){
                //get请求直接把所有请求头读走
                app.Rio_readlineb(rp, buf, MAXLINE);
                while (strcmp(buf, "\r\n")) {
                    app.Rio_readlineb(rp, buf, MAXLINE);
                    LOGD("%s", buf);
                }
            }
        
            if(method == POST){
                //post请求直接把所有请求头读走，再把post数据保存
                app.Rio_readlineb(rp, buf, MAXLINE);
                while (strcmp("\r\n", buf) != 0) {
                    app.Rio_readlineb(rp, buf, MAXLINE);
                    LOGD("%s",buf);
                }
            
                //读取post的请求体
                app.Rio_readlineb(rp, buf, MAXLINE);
                strncpy(cgiargs, buf, MAXLINE);
            }
        
            return;
        }
    
        int parse_url(char* url, char* filename, char* cgiargs){
            char* ptr;
            if(!strstr(url, "cgi-bin")){
                //静态请求
            
                strncpy(cgiargs, "", MAXLINE);
                strncpy(filename, ".", MAXLINE);
                strcat(filename, url);
        
                if(url[strlen(url)-1] == '/'){
                    //如果静态请求最后是'/',返回默认主页
                    strcat(filename, "home.html");
                }
            
                LOGD("static : %s", filename);
                return 1;
            }
            else{
                //动态请求
                ptr = index(url, '?');
                if (ptr) {
                    strncpy(cgiargs, ptr+1, MAXLINE);
                    *ptr = '\0';
                }
                else{
                    strncpy(cgiargs, "", MAXLINE);
                }
            
                strncpy(filename, ".", MAXLINE);
                strcat(filename, url);
            
                LOGD("dynamic : %s", filename);
                return 0;
            }
        }
    
        void serve_static(int fd, char* filename, int filesize){
            int srcfd;
            char* srcp, filetype[MAXLINE], buf[MAXBUF];
            
            //发送http回应头部
            get_filetype(filename, filetype);
            sprintf(buf, "HTTP/1.0 200 OK\r\n");
            sprintf(buf, "%sServer: Web Server\r\n", buf);
            sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
            sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
            app.Rio_writen(fd, buf, strlen(buf));
            
            //发送http回应body
            srcfd = app.Open(filename, O_RDONLY, 0);
            srcp = (char*)app.Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
            app.Close(srcfd);
            app.Rio_writen(fd, srcp, filesize);
            app.Munmap(srcp, filesize);
        }
    
        void get_filetype(char* filename, char* filetype){
            if (strstr(filename, ".html")) {
                strncpy(filetype, "text/html", MAXLINE);
            }
            else if(strstr(filename, ".gif")){
                strncpy(filetype, "image/gif", MAXLINE);
            }
            else if(strstr(filename, ".jpg")){
                strncpy(filetype, "image/jpeg", MAXLINE);
            }
            else{
                strncpy(filetype, "text/plain", MAXLINE);
            }
        }
    
        void serve_dynamic(int fd, char* filename, char* cgiargs){
            char buf[MAXLINE], *emptylist[] = { NULL };
        
            //返回回复报文头
            sprintf(buf, "HTTP/1.0 200 OK\r\n");
            app.Rio_writen(fd, buf, strlen(buf));
            sprintf(buf, "Server: Web Server\r\n");
            app.Rio_writen(fd, buf, strlen(buf));
        
            if (app.Fork() == 0) {
                //设置cgi环境变量
                setenv("QUERY_STRING", cgiargs, 1);
                app.Dup2(fd, STDOUT_FILENO);
                app.Execve(filename, emptylist, environ);
            }
            
            app.Wait(NULL);
        }
    
        void clienterror(int fd, char* cause, char* errnum,
                         char* shortmsg, char* longmsg){
            char buf[MAXLINE], body[MAXBUF];
            
            //创建http回复消息的body
            sprintf(body, "<html><title>Tiny Error</title>");
            sprintf(body, "%s<body bgcolor=""ffffff"">\r\n",body);
            sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
            sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
            sprintf(body, "%s<hr><em> The Web Server </em>\r\n", body);
            LOGD("%s", body);
            //输出http回应报文
            sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
            LOGD("%s",buf);
            app.Rio_writen(fd, buf, strlen(buf));
            sprintf(buf, "Content-type: text/html\r\n");
            LOGD("%s",buf);
            app.Rio_writen(fd, buf, strlen(buf));
            sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
            LOGD("%s",buf);
            app.Rio_writen(fd, buf, strlen(buf));
            app.Rio_writen(fd, body, strlen(body));
        }
};

Webserver::Webserver(){
    inter = std::make_shared<interface>();
}

void Webserver::doit(int fd){
    inter->doit(fd);
}

void Webserver::clienterror(int fd, char *cause, char *errnum,
                            char *shortmsg, char *longmsg){
    inter->clienterror(fd, cause, errnum, shortmsg, longmsg);
}

void Webserver::read_requesthdrs(rio_t *rp,char* method,char* cgiargs){
    inter->read_requesthdrs(rp, method, cgiargs);
}

int Webserver::parse_url(char *url, char *filename, char *cgiargs){
    return inter->parse_url(url, filename, cgiargs);
}

void Webserver::serve_static(int fd, char *filename, int filesize){
    inter->serve_static(fd, filename, filesize);
}

void Webserver::get_filetype(char *filename, char *filetype){
    inter->get_filetype(filename, filetype);
}

void Webserver::serve_dynamic(int fd, char *filename, char *cgiargs){
    inter->serve_dynamic(fd, filename, cgiargs);
}
