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

extern "C"{
#include "csapp.h"
}


class Webserver{
    private:
        Webserver(){};
        Webserver(const Webserver&);
        Webserver& operator=(const Webserver&);
    
        static Webserver* instance;
    public:
        static Webserver* getInstance(){
            return instance;
        }
    
    public:
    
        /**
        @brief 处理一个HTTP事务
        @param fd 客户端的socket连接
        @return 
        **/
        void doit(int fd);
    
        /**
        @brief 用于读取请求报头中的信息，现在仅仅作为读取并忽略
        @param rp 读取数据的结构体
        @return
         **/
        void read_requesthdrs(rio_t* rp);
    
        /**
         @brief 将url解析为一个文件名和一个可选的CGI参数字符串
         @param url请求地址 filename请求文件名 cgiargs动态请求的参数
         @return 返回值1代表静态请求，返回值0代表动态请求
         **/
        int parse_url(char* url, char* filename, char* cgiargs);
    
        /**
         @brief 回复一个静态请求
         @param fd客户端socket连接，filename请求文件，filesize文件大小
         @return
         **/
        void serve_static(int fd, char* filename, int filesize);
    
        /**
         @brief 判断请求文件类型
         @param filename请求文件, filetype请求文件类型
         **/
        void get_filetype(char* filename, char* filetype);
    
        /**
         @brief 回复一个动态请求
         @param fd客户端socket连接， filename请求执行文件，cgiargs参数
         @return
         **/
        void serve_dynamic(int fd, char* filename, char* cgiargs);
    
        /**
         @brief 返回一个HTTP响应到客户端
         @param fd 客户端的socket连接， cause请求的文件， errnum消息类型， shortmsg概要，longmsg解释
         @return
         **/
        void clienterror(int fd, char* cause, char* errnum,
                         char* shortmsg, char* longmsg);
    
};

#endif /* webserver_hpp */
