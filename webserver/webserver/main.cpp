#include <iostream>
#include <memory>
#include <boost/smart_ptr.hpp>
#include "start.hpp"

using namespace std;
int main(int argc, char** argv)
{
    LOGI("webserver program begin!");
    
    int port = 7788;
    if(argc != 2){
        LOGI("usage: %s <port>\n", argv[0]);
    }else{
        port = atoi(argv[1]);
    }
    auto deleter = [](Start* s){LOGI("web server stoped!");delete s;};
    unique_ptr<Start, decltype(deleter)> Server(new Start(port), deleter);
    Server->run_poll();
}

