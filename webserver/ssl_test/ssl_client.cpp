#include "ssl_base.hpp"
class SSL_CLIENT : public SSL_BASE{
    private:
        int sockfd = -1;

    public:
        ~SSL_CLIENT(){
            if(sockfd){
                close(sockfd);
            }
        }

    public:
        virtual void init_ctx(){
            ctx = SSL_CTX_new(SSLv23_client_method());
            if(nullptr == ctx){
                error_output(nullptr);
                return;
            }
        }

        void init_sock(){
            struct sockaddr_in dst;

            if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                error_output("client sockfd init failed!");
                return;
            }

            bzero(&dst, sizeof(dst));
            dst.sin_family = AF_INET;
            dst.sin_port = htons(9677);
            dst.sin_addr.s_addr = inet_addr("127.0.0.1");

            if(connect(sockfd, (struct sockaddr*)&dst, sizeof(dst)) != 0){
                error_output("connect failed!");
                return;
            }
        }

        void connect_ssl(){
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, sockfd);
            
            if(-1 == SSL_connect(ssl)){
                error_output(nullptr);
                return;
            }

            show_certs(ssl);
        }

        int ssl_write(char* buf){
            return SSL_write(ssl, buf, strlen(buf));
        }

        int ssl_read(char* buf, buf_size buf_size){
            return SSL_read(ssl, buf, buf_size);
        }

        void shutdown_ssl(){
            SSL_shutdown(ssl);
        }

};