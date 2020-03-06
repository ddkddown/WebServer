#include "sock.hpp"
#include "ssl_header.hpp"
namespace ddk{
    class Ssl_base{
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
                if(!start_server()){
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

            SSL* get_ssl_client(){
                int c_fd = get_client_fd();
                auto ssl = SSL_new(ctx);
                if(-1 == SSL_accept(ssl)){
                    ddk_errno = ssl_get_client_error;

                }
            }

    };
}