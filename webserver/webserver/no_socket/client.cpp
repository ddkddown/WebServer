#include "sock.hpp"
using namespace ddk;

int main(){
    std::string ip = "127.0.0.1";
    int port = 7796;
    Socket_client<> c(ip,port);
    c.connect_to_server();
    std::string mess = "12345";
    c.send(mess); 
}