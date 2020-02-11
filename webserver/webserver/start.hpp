//
//  start.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/26.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef start_hpp
#define start_hpp

#include <vector>
#include "webserver.hpp"
#include "csapp.hpp"

extern "C"{
#include <sys/select.h>
#include "globalvar.h"
}

class Start{
    private:
        Csapp app;
        struct connect conn;
        struct fd_set rfd;
        std::vector<int> clients;
    
    private:
        Start& operator=(const Start&);
        void accept_clients();
    
    public:
        Start(int port = 80);
        ~Start();
        void run_poll();
};

#endif /* start_hpp */
