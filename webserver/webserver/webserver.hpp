//
//  webserver.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/22.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef webserver_hpp
#define webserver_hpp

#include <iostream>

#include "csapp.hpp"

extern int h_errno;
extern char **environ;

#define GET "GET"
#define POST "POST"
class Server{
    public:
        virtual ~Server(){};
    public:
        /**
         @brief 处理一个HTTP事务
         @param fd 客户端的socket连接
         **/
        virtual void doit(int fd) = 0;
    
        /**
        @brief 用于读取请求报头中的信息，现在仅仅作为读取并忽略
        @param rp 读取数据的结构体
         **/
        virtual void read_requesthdrs(rio_t* rp,char* method,char* cgiargs) = 0;
    
        /**
         @brief 将url解析为一个文件名和一个可选的CGI参数字符串
         @param url 请求地址 filename 请求文件名 cgiargs 动态请求的参数
         @return 返回值1代表静态请求，返回值0代表动态请求
         **/
        virtual int parse_url(char* url, char* filename, char* cgiargs) = 0;
    
        /**
         @brief 回复一个静态请求
         @param fd 客户端socket连接，filename 请求文件，filesize 文件大小
         **/
        virtual void serve_static(int fd, char* filename, int filesize) = 0;
    
        /**
         @brief 判断请求文件类型
         @param filename 请求文件, filetype 请求文件类型
         **/
        virtual void get_filetype(char* filename, char* filetype) = 0;
    
        /**
         @brief 回复一个动态请求
         @param fd 客户端socket连接， filename 请求执行文件，cgiargs 参数
         **/
        virtual void serve_dynamic(int fd, char* filename, char* cgiargs) = 0;
    
        /**
         @brief 返回一个HTTP响应到客户端
         @param fd 客户端的socket连接， cause 请求的文件， errnum 消息类型， shortmsg 概要，longmsg 解释
         **/
        virtual void clienterror(int fd, char* cause, char* errnum,
                                 char* shortmsg, char* longmsg) = 0;
};

class Webserver : virtual public Server{
    private:
        Webserver();
        Webserver(const Webserver&);
        Webserver& operator=(const Webserver&);
        
        static Webserver* instance;
        class interface;
        decltype(std::make_shared<interface>()) inter;
    public:
        static Webserver* getInstance(){
            return instance;
        }
    
    public:
        virtual void doit(int fd);
        virtual void read_requesthdrs(rio_t* rp,char* method,char* cgiargs);
        virtual int parse_url(char* url, char* filename, char* cgiargs);
        virtual void serve_static(int fd, char* filename, int filesize);
        virtual void get_filetype(char* filename, char* filetype);
        virtual void serve_dynamic(int fd, char* filename, char* cgiargs);
        virtual void clienterror(int fd, char* cause, char* errnum,
                         char* shortmsg, char* longmsg);
    
};

#endif /* webserver_hpp */
