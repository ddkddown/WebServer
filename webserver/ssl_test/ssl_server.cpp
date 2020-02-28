#include "ssl_base.hpp"
#define LISNUM 10
class SSL_SERVER : public SSL_BASE{
    private:
        int sockfd = -1;
    public:
        ~SSL_SERVER(){
            if(sockfd){
                close(sockfd);
            }

            if(ctx){
                SSL_CTX_free(ctx);
            }

            if(ssl){
                SSL_free(ssl);
            }
        }
    public:
        virtual void init_ctx(){
            ctx = SSL_CTX_new(SSLv23_server_method());
            if(nullptr == ctx){
                error_output(nullptr);
                return;
            }

            if(SSL_CTX_use_certificate_file(ctx, "./cert.pem", SSL_FILETYPE_PEM) <= 0){
                error_output(nullptr);
                return;
            }

            if(SSL_CTX_use_PrivateKey_file(ctx, "./privkey.pem", SSL_FILETYPE_PEM) <= 0){
                error_output(nullptr);
                return;
            }

            if(!SSL_CTX_check_private_key(ctx)){
                error_output(nullptr);
                return;
            }
        }

        void init_sock(){
            struct sockaddr_in server_addr;
            if(-1 == (sockfd = socket(PF_INET, SOCK_STREAM, 0))){
                error_output("socket create failed!");
                return;
            }
            
            bzero(&server_addr, sizeof(server_addr));
            server_addr.sin_family = PF_INET;
            server_addr.sin_port = htons(9677);
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

            if(-1 == bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))){
                error_output("bind failed!");
                return;
            }

            if(-1 == listen(sockfd, LISNUM)){
                error_output("listen failed!");
                return;
            }
        }

        void start_accept(){
            char buf[1024] = {0};
            struct sockaddr_in client_addr;
            int newfd = -1;
            if(-1 == (newfd = accept(sockfd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr)))){
                error_output("accept failed!");
                return;
            }

            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, newfd);
            if(-1 == SSL_accept(ssl)){
                error_output("accept failed!");
                close(newfd);
                return;
            }
            
            SSL_read(ssl, buf, 1024);
            cout<<buf<<endl;
            SSL_shutdown(ssl);
            SSL_free(ssl);
            ssl = nullptr;
            close(newfd);
            close(sockfd);
        }
};
