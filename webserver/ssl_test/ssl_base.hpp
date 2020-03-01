#include "ssl_header.hpp"
class SSL_BASE{
    protected:
        SSL_CTX* ctx;
        SSL* ssl;
    protected:
        void error_output(const char* str){
            if(str){
                cout<<str<<endl;
            }else{
                ERR_print_errors_fp(stdout);
            }
        }

    public:
        SSL_BASE():ctx(nullptr),ssl(nullptr){}
        SSL_BASE(const SSL_BASE& obj) = delete;
        SSL_BASE& operator=(const SSL_BASE& obj) = delete;
        ~SSL_BASE(){
            if(ssl){
                SSL_free(ssl);
            }

            if(ctx){
                SSL_CTX_free(ctx);
            }
        }

    public:
        void init_ssl(){
            SSL_library_init();
            OpenSSL_add_ssl_algorithms();
            SSL_load_error_strings();
        }
        
        void show_certs(){
            X509* cert = nullptr;
            char* line = nullptr;
            cert = SSL_get_peer_certificate(ssl);
            if(nullptr != cert){
                cout<<"数字信息"<<endl;
                line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
                free(line);
                cout<<"证书"<<endl;
                line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
                cout<<"颁发者"<<endl;
                free(line);
                X509_free(cert);
            }
        }

        virtual void init_ctx() = 0;

};