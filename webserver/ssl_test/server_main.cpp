#include "ssl_server.hpp"
#include <iostream>
#include <memory>
using namespace std;

int main(){
    shared_ptr<SSL_SERVER> test = make_shared<SSL_SERVER>();
    test->init_sock();
    test->init_ctx();
    test->start_accept();
}
