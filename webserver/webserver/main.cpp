#include <iostream>

#include "webserver.hpp"
#include "serverlog.hpp"
#include "globalvar.h"
#include "csapp.hpp"

int main(int argc, char** argv)
{
    LOGI("webserver program begin!");
    
    int port = 80;
    if(argc != 2){
        LOGI("usage: %s <port>\n", argv[0]);
    }else{
        port = atoi(argv[1]);
    }
    
    Csapp app;
    conn.port = port;
    conn.listenfd = app.Open_listenfd(conn.port);
    if(conn.listenfd < 0){
        LOGE("open listen fd failed! %d", conn.listenfd);
        exit(1);
    }
    
    while (1) {
        LOGI("web server start!");
        
        conn.clientlen = sizeof(conn.clientaddr);
        conn.connfd = app.Accept(conn.listenfd, (struct sockaddr*)&conn.clientaddr,
                            (socklen_t*)&conn.clientlen);
        if(conn.connfd < 0){
            LOGE("accept connfd failed! %d",conn.connfd);
            exit(1);
        }
        
        auto instance = Webserver::getInstance();
        instance->doit(conn.connfd);
        app.Close(conn.connfd);
    }
    
}

