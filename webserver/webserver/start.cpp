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
    
    clients.clear();
}

void Start::accept_clients(){
    LOGI("web server start!");
    
    conn.clientlen = sizeof(conn.clientaddr);
    conn.connfd = app.Accept(conn.listenfd, (struct sockaddr*)&conn.clientaddr,
                             (socklen_t*)&conn.clientlen);
    
    if(conn.connfd < 0){
        LOGE("accept connfd failed! %d",conn.connfd);
        return;
    }
        
    clients.push_back(conn.connfd);
}

void Start::run_poll(){
    while (1) {
        FD_ZERO(&rfd);
        FD_SET(conn.listenfd,&rfd);
        
        int maxfd = conn.listenfd+1;
        if(!clients.empty()){
            LOGI("clients is not empty!");
            for(auto i : clients){
                FD_SET(i,&rfd);
                LOGI("clients is %d",i);
                maxfd = conn.listenfd>i?conn.listenfd+1:i+1;
            }
        }
        
        switch (select(maxfd, &rfd, nullptr, nullptr, nullptr)) {
            case -1:
                LOGI("select error! %s",strerror(errno));
                exit(-1);
                break;
            case 0:
                break;
            default:
                if(FD_ISSET(conn.listenfd,&rfd)){
                    accept_clients();
                }
                
                for(auto i : clients){
                    if(FD_ISSET(i,&rfd)){
                        auto instance = Webserver::getInstance();
                        instance->doit(i);
                        app.Close(i);
                        for (vector<int>::iterator it = clients.begin();
                             it != clients.end();) {
                            if(*it == i){
                                it = clients.erase(it);
                            }else{
                                ++it;
                            }
                        }
                    }
                }
                break;
        }
    }
}

Start::~Start(){
    if(conn.listenfd){
        app.Close(conn.listenfd);
    }
    
    for(auto i : clients){
        app.Close(i);
    }
}
