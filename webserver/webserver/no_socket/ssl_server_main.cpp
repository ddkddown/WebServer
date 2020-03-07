#include "ssl_sock.hpp"
using namespace ddk;

int main(){
    std::string cert = "./cert.pem";
    std::string privkey = "./privkey.pem";

    Ssl_server<> test(cert,privkey,"127.0.0.1",7796);
    test.start_ssl_server();
    auto cleint = test.get_ssl_client();
    test.recv(10,cleint.ssl);
    test.close_ssl_client(cleint);
}