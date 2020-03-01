#include "ssl_client.hpp"
#include <iostream>
#include <memory>
using namespace std;

int main(){
    shared_ptr<SSL_CLIENT> test = make_shared<SSL_CLIENT>();
    test->init_sock();
    test->init_ctx();
    test->connect_ssl();
    char buf[] = {"1234456"};
    test->ssl_write(buf);
    test->shutdown_ssl();
}