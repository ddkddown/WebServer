//
//  start.cpp
//  webserver
//
//  Created by dong da kuan on 2020/1/26.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#include "start.hpp"

Start::Start(int port){
    conn.port = port;
    conn.listenfd = app.Open_listenfd(conn.port);
    if(conn.listenfd < 0){
        LOGE("open listen fd failed! %d", conn.listenfd);
        exit(1);
    }
}

void Start::startService(){
    LOGI("web server start!");
    
    conn.clientlen = sizeof(conn.clientaddr);
    while (1) {
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

Start::~Start(){
    if(conn.listenfd){
        app.Close(conn.listenfd);
    }
    
    LOGI("web server stoped!");
}
