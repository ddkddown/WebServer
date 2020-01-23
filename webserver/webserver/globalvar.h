//
//  globalvar.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/22.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef globalvar_h
#define globalvar_h

#include <stdio.h>

struct connect{
    int listenfd;
    int connfd;
    int port;
    int clientlen;
    struct sockaddr_in clientaddr;
};

//存放全局变量

struct connect conn;

//存放全局变量
#endif /* globalvar_hpp */
