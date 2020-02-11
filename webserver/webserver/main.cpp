#include <iostream>

#include "start.hpp"

int main(int argc, char** argv)
{
    LOGI("webserver program begin!");
    
    int port = 7788;
    if(argc != 2){
        LOGI("usage: %s <port>\n", argv[0]);
    }else{
        port = atoi(argv[1]);
    }
    Start program(port);
    program.run_poll();
    
}

