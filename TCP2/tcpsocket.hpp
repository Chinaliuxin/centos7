/*=============================================================== 

*   描    述：封装TcpSocket类，向外提供更加轻便的tcp套接字接口 
*       1. 创建套接字           Socket()
*       2. 绑定地址信息         Bind(std::string &ip, uint16_t port)
*       3. 服务端开始监听       Listen(int backlog = 5)
*       4. 服务端获取已完成连接的客户端新socket 
*          Accept(TcpSocket &clisock,std::string &cli_ip, uint16_t port)
*       5. 接收数据             Recv(std::string &buf)
*       6. 发送数据             Send(std::string &buf)
*       7. 关闭套接字           Close()
*       3. 客户端向服务端发起连接请求 
*           Connect(std::string &srv_ip, uint16_t srv_port);
================================================================*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q) == false){return -1;}

class TcpSocket
{
    public:
        TcpSocket(){
        }
        ~TcpSocket(){
        }
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            int opt = 1;
            setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, 
                    (void*)&opt, sizeof(int));
            return true;
        }
        void SetNonBlock() {
            //int fcntl(int fd, int cmd, ... /* arg */ );
            int flag = fcntl(_sockfd, F_GETFL, 0);
            fcntl(_sockfd, F_SETFL, flag|O_NONBLOCK);
        }
        bool Bind(const std::string &ip, const uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());

            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        bool Listen(int backlog = 5) {
            //int listen(int sockfd, int backlog)
            //  sockfd: 套接字描述符
            //  backlog：最大并发连接数-决定内核中已完成连接队列节点个数
            //  backlog决定的不是服务端能接收的客户端最大上限
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        } 
        bool Accept(TcpSocket &cli, std::string &cliip, uint16_t &port){
            //int accept(int sockfd, struct sockaddr *addr, 
            //socklen_t *addrlen);
            //sockfd:   套接字描述符
            //addr：    新建连接的客户端地址信息
            //addrlen： 新建客户端的地址信息长度
            //返回值：返回新建客户端socket的描述符
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int sockfd = accept(_sockfd, (struct sockaddr*)&addr, &len);
            if (sockfd < 0) {
                perror("accept error");
                return false;
            }
            cli.SetFd(sockfd);
            cliip = inet_ntoa(addr.sin_addr);
            port = ntohs(addr.sin_port);
            return true;
        }
        void SetFd(int sockfd){
            _sockfd = sockfd;
        }
        int GetFd() {
            return _sockfd;
        }
        bool Connect(std::string &srv_ip, uint16_t srv_port) {
            //int connect(int sockfd, const struct sockaddr *addr,
            //socklen_t addrlen);
            //sockfd:   套接字描述符
            //addr：    服务端地址信息
            //addrlen： 地址信息长度
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(srv_port);
            addr.sin_addr.s_addr = inet_addr(srv_ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);

            int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("connect error");
                return false;
            }
            return true;
        }
        bool Recv(std::string &buf) {
            //ssize_t recv(int sockfd, void *buf, size_t len, int flags)
            //sockfd:   服务端为新客户端新建的socket描述符
            //flags：   0-默认阻塞接收，没有数据就一直等待
            //      MSG_PEEK    接收数据，但是数据并不从接收缓冲区移除
            //      常用于探测性数据接收
            //返回值：实际接收字节长度；出错返回-1；若连接断开则返回0
            //recv默认是阻塞的，意味着没有数据则一直等，不会返回0；
            //返回0只有一种情况，就是连接断开，不能再继续通信了
            while(1) {
                char tmp[5];
                int ret = recv(_sockfd, tmp, 5, 0);
                if (ret < 0) {
                    if (errno == EAGAIN) {
                        break;
                    }
                    perror("recv error");
                    return false;
                }else if (ret == 0) {
                    printf("peer shutdown\n");
                    return false;
                }
                tmp[ret] = '\0';
                buf +=  tmp;
            }
            return true;
        }
        bool Send(std::string &buf) {
            //ssize_t send(int sockfd, void *buf, size_t len, int flags)
            int ret = send(_sockfd, buf.c_str(), buf.size(), 0);
            if (ret < 0) {
                perror("send error");
                return false;
            }
            return true;
        }
        bool Close() {
            close(_sockfd);
            return true;
        }
    private:
        int _sockfd;
};
