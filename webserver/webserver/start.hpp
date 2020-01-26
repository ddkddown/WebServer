//
//  start.hpp
//  webserver
//
//  Created by dong da kuan on 2020/1/26.
//  Copyright © 2020年 dong da kuan. All rights reserved.
//

#ifndef start_hpp
#define start_hpp

#include "webserver.hpp"
#include "csapp.hpp"
#include "globalvar.h"

class Start{
    private:
        Csapp app;
        struct connect conn;
    
    private:
        Start& operator=(const Start&);
    
    public:
        Start(int port = 80);
        ~Start();
        void startService();
};

#endif /* start_hpp */
