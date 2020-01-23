#include <iostream>

#include "webserver.hpp"
#include "serverlog.hpp"
#include "globalvar.h"

int main(int argc, char** argv)
{
    LOGI("webserver program begin!");
    
    if(argc != 2){
        LOGE("usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    conn.port = atoi(argv[1]);
    conn.listenfd = Open_listenfd(conn.port);
    if(conn.listenfd < 0){
        LOGE("open listen fd failed! %d", conn.listenfd);
        exit(1);
    }
    
    while (1) {
        LOGI("web server start!");
        
        conn.clientlen = sizeof(conn.clientaddr);
        conn.connfd = Accept(conn.listenfd, (struct sockaddr*)&conn.clientaddr,
                            (socklen_t*)&conn.clientlen);
        if(conn.connfd < 0){
            LOGE("accept connfd failed! %d",conn.connfd);
            exit(1);
        }
        
        auto instance = Webserver::getInstance();
        instance->doit(conn.connfd);
        Close(conn.connfd);
    }
    
}

