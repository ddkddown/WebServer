#include "sock.hpp"
#include "ssl_header.hpp"
namespace ddk{
    class Ssl_base{
        protected:
            struct d_ssl{
                SSL* ssl;
                int fd;
            };
        protected:
            SSL_CTX* ctx;
        public:
            Ssl_base():ctx(nullptr){ 
                SSL_library_init();
                OpenSSL_add_all_algorithms();
                SSL_load_error_strings();
            }
            Ssl_base(const Ssl_base& obj) = delete;
            Ssl_base& operator=(const Ssl_base& obj) = delete;
            ~Ssl_base(){
                if(ctx){
                    SSL_CTX_free(ctx);
                }
            }
    };

    template<int listen_num = 10, bool open_keepalive = false>
    class Ssl_server : protected Socket_server<listen_num, open_keepalive>, protected Ssl_base{
        private:
            std::string d_cert;
            std::string d_privkey;
        public:
            Ssl_server(std::string cert, std::string privkey, std::string ip, int port):
                Socket_server<listen_num, open_keepalive>(ip,port),d_cert(cert),d_privkey(privkey){}
        public:
            bool start_ssl_server(){
                if(!this->start_server()){
                    ddk_errno = start_ssl_server_error;
                    return false;
                }

                ctx = SSL_CTX_new(SSLv23_server_method());
                if(nullptr == ctx){
                    ddk_errno = ssl_ctx_create_error;
                    return false;
                }

                if(0 >= SSL_CTX_use_certificate_file(ctx, d_cert.c_str(), SSL_FILETYPE_PEM)){
                    ddk_errno = ssl_load_cert_error;
                    return false;
                }

                if(0 >= SSL_CTX_use_PrivateKey_file(ctx, d_privkey.c_str(), SSL_FILETYPE_PEM)){
                    ddk_errno = ssl_load_privkey_error;
                    return false;
                }

                if(!SSL_CTX_check_private_key(ctx)){
                    ddk_errno = ssl_check_privkey_error;
                    return false;
                }
            }

            d_ssl get_ssl_client(){
                int c_fd = this->get_client_fd();
                auto ssl = SSL_new(ctx);
                SSL_set_fd(ssl, c_fd);
                if(-1 == SSL_accept(ssl)){
                    ddk_errno = ssl_get_client_error;
                    close(c_fd);
                    SSL_free(ssl);
                    return nullptr;
                }
                d_ssl ret = {ssl,c_fd};
                return ret;
            }

            void close_ssl_client(d_ssl& obj){
                SSL_shutdown(obj.ssl);
                SSL_free(obj.ssl);
                close(obj.fd);
            }

            std::string recv(int recv_size, SSL* ssl, std::function<int(SSL*,void*,int)> func
                        = [](SSL* ssl,void* buf, int num)->int{return SSL_read(ssl,buf,num);}){
                if(nullptr == ssl){
                    ddk_errno = fd_value_invalid;
                    return "";
                }
                return this->write_data(recv_size,ssl,func);
            }
            int send(std::string& mess, SSL* ssl, std::function<int(SSL*,void*,int)>func
                        =[](SSL* ssl,void* buf,int num)->int{return SSL_write(ssl,buf,num);}){
                if(nullptr == ssl){
                    ddk_errno = fd_value_invalid;
                    return -1;
                }
                return this->send_data(mess,ssl,func);
            }
    };

    template<bool open_keepalive = false>
    class Ssl_client : protected Socket_client<open_keepalive>, protected Ssl_base{
        private:
            SSL* ssl;
        public:
            Ssl_client(std::string ip, int port):Socket_client<open_keepalive>(ip,port),ssl(nullptr){}
            ~SSL_client(){
                if(ssl){
                    SSL_shutdown(ssl);
                    SSL_free(ssl);
                }
            }
        public:
            bool connect_to_server(){
                if(!this->connect_to_server()){
                    ddk_errno = connect_server_error;
                    return false;
                }

                ctx = SSL_CTX_new(SSLv23_client_method());
                if(nullptr == ctx){
                    ddk_errno = ssl_ctx_create_error;
                    return false;
                }

                ssl = SSL_new(ctx);
                SSL_set_fd(ssl,this->d_fd);

                if(-1 == SSL_connect(ssl)){
                    ddk_errno = ssl_connect_server_error;
                    return false;
                }
            }
        
            std::string recv(int recv_size, std::function<int(SSL*,void*,int)> func
                        = [](SSL* ssl,void* buf, int num)->int{return SSL_read(ssl,buf,num);}){
                if(nullptr == ssl){
                    ddk_errno = fd_value_invalid;
                    return "";
                }

                return this->write_data(recv_size,ssl,func);
            }

            int send(std::string& mess, std::function<int(SSL*,void*,int)>func
                        =[](SSL* ssl,void* buf,int num)->int{return SSL_write(ssl,buf,num);}){
                if(nullptr == ssl){
                    ddk_errno = fd_value_invalid;
                    return -1;
                }

                return this->send_data(mess,ssl,func);
            }

            void close_connect(){
                SSL_shutdown(ssl);
                SSL_free(ssl);
                ssl = nullptr;
            }
    };
}