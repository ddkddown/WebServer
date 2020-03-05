#include "sock.hpp"
using namespace ddk;
int main(){
    std::string ip = "127.0.0.1";
    Socket_server<>a(ip,7796);
    a.start_server();
    auto c_fd = a.get_client_fd();
    std::cout<<a.recv(10,c_fd);
}