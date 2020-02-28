#include <iostream>

extern "C"{
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <cstring>
}

using namespace std;

int create_socket(int port){
    int s;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        cout<<"Unable to create socket";
    }

    if(bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        cout<<"unable to bind";
    }

    if(listen(s,1) < 0){
        cout<<"unable to listen";
    }

    return s;
}

void init_openssl(){
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl(){
    EVP_cleanup();
}

SSL_CTX* create_context(){
    const SSL_METHOD* method;
    SSL_CTX* ctx;
    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if(!ctx){
        cout<<"unable to create ssl context";
        exit(1);
    }
    
    return ctx;
}

int main(){
    int sock;
    SSL_CTX* ctx;
    init_openssl();
    ctx = create_context();
    
    sock = create_socket(4400);

    while(1){
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL* ssl;
        const char reply[] = "test\n";
        int client = accept(sock,(struct sockaddr*)&addr, &len);
        if(client < 0){
            cout<<"unable to accept";
            exit(1);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if(SSL_accept(ssl) <= 0){
            cout<<"ssl accept failed!";
        }else{
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }
    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}
