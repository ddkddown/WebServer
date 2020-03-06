/*
* 这一层只负责建立tcp连接，发数据，接收数据
*
*/
#pragma once

#include "cheader.hpp"

#include<iostream>
#include<string>
#include<functional>
namespace ddk{
    int ddk_errno = 0;
    
    enum errno_type{
        fd_value_invalid = 1,
        socket_create_error,
        bind_socket_error,
        listen_socket_error,
        accept_client_error,
        connect_server_error,
        send_data_error,
        recv_data_error,
        start_ssl_server_error,
        ssl_ctx_create_error,
        ssl_load_cert_error,
        ssl_load_privkey_error,
        ssl_check_privkey_error,
        ssl_get_client_error
    };

    class Socket_base{
        protected:
            int d_fd, d_port;
            std::string d_ip;
        
        protected:
            template<typename fd_type = int>
            using read_func = int(*)(fd_type* fd, void* buf, int num);

            template<typename fd_type = int>
            using write_func = int(*)(fd_type* fd, void* buf, int num);

        public:
            Socket_base(std::string ip, int port):d_ip(ip),d_port(port),d_fd(-1){}
            ~Socket_base(){
                if(d_fd){
                    close(d_fd);
                }
            }

        public:
            virtual void keep_alive() = 0;
        
        protected:
            template<typename fd_type = int>
            std::string recv_data(int recv_size, int c_fd, std::function<int(fd_type,void*,int)>& func){
                std::string ret_data = "";
                if(0 >= c_fd){
                    ddk_errno = fd_value_invalid;
                    return ret_data;
                }

                int count = 0, recv = 0;
                while(count < recv_size){
                    int buf_len = recv_size-count;
                    char* buf= new char[buf_len];
                    recv = func(c_fd,buf,buf_len);
                    if(-1 == recv){
                        ddk_errno = recv_data_error;
                        ret_data.clear();
                        delete[] buf;
                        return ret_data;
                    }
                    delete[] buf;
                    count += recv;
                    ret_data.append(buf);
                }

                return ret_data;
            }

            template<typename fd_type = int>
            int send_data(std::string& mess, int c_fd, std::function<int(fd_type,void*,int)>& func){
                if(0 >= c_fd){
                    ddk_errno = fd_value_invalid;
                    return -1;
                }

                int count = 0, send = 0, data_len = mess.length();
                const char* c = mess.c_str();
                while(count < data_len){
                    int send_count = (data_len-count);
                    send = func(c_fd,(void*)c,send_count);
                    if(-1 == send){
                        ddk_errno = send_data_error;
                        return -1;
                    }
                    count += send;
                    c += send;
                }
                
                return count;
            }
    };

    template <int listen_num = 10, bool open_keepalive = false>
    class Socket_server : virtual public Socket_base{
        private:
            socklen_t c_size;
            struct sockaddr_in client, server;
        
        public:
            Socket_server(std::string ip, int port):Socket_base(ip,port){ c_size = sizeof(client);}
            Socket_server(const Socket_server&) = delete;
            Socket_server& operator=(const Socket_server&) = delete;
        
        public:
            bool start_server(){
                d_fd = socket(AF_INET, SOCK_STREAM, 0);
                if(0 >= d_fd){
                    ddk_errno = socket_create_error;
                    return false;
                }
                server.sin_family = AF_INET;
                server.sin_port = htons(d_port);
                server.sin_addr.s_addr = inet_addr(d_ip.c_str());
                if(0 > bind(d_fd, (struct sockaddr*)&server, sizeof(server))){
                    ddk_errno = bind_socket_error;
                    return false;
                }

                if(0 > listen(d_fd,listen_num)){
                    ddk_errno = listen_socket_error;
                    return false;
                }

                return true;
            }

            int get_client_fd(){
                auto ret_fd = accept(d_fd, (struct sockaddr*)&client, &c_size);

                if(0 > ret_fd){
                    ddk_errno = accept_client_error;
                }
                
                return ret_fd;
            }

            int close_client(int c_fd){
                close(c_fd);
            }

            std::string recv(int recv_size , int& c_fd, std::function<int(int,void*,int)> func
                                = [](int fd,void* buf,int num)->int{return read(fd,buf,num);}){
                return recv_data(recv_size,c_fd,func);
            }

            int send(std::string& mess, int c_fd, std::function<int(int,void*,int)> func
                                =[](int fd,void* buf,int num)->int{return write(fd,buf,num);}){
                return send_data(mess,c_fd,func);
            }

            virtual void keep_alive(){}
    };

    template<bool open_keepalive = false>
    class Socket_client : virtual public Socket_base{
        private:
            socklen_t c_size;
            struct sockaddr_in server;

        public:
            Socket_client(std::string ip, int port):Socket_base(ip,port){c_size = sizeof(server);}
            bool connect_to_server(){
                d_fd = socket(AF_INET, SOCK_STREAM, 0);
                if(0 >= d_fd){
                    ddk_errno = socket_create_error;
                    return false;
                }

                server.sin_family = AF_INET;
                server.sin_port = htons(d_port);
                server.sin_addr.s_addr = inet_addr(d_ip.c_str());
                if(0 > connect(d_fd, (struct sockaddr*)&server,c_size)){
                    ddk_errno = connect_server_error;
                    return false;
                }
                return true;
            }

            std::string recv(int recv_size, std::function<int(int,void*,int)> func
                             = [](int fd,void* buf,int num)->int{return read(fd,buf,num);}){
                return recv_data(recv_size, d_fd,func);
            }

            int send(std::string& mess, std::function<int(int,void*,int)> func
            =[](int fd,void* buf,int num)->int{return write(fd,buf,num);}){
                return send_data(mess, d_fd,func);
            }
        
            virtual void keep_alive(){}

    };


}
