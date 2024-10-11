#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#define MAX_EVENTS 10
#define SOCKET_PATH "/tmp/unix_socket_example"
#define BUFFER_SIZE 1024

int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    return 0;
}

int main() {
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    if (setNonBlocking(sockfd) == -1) {
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, SOCKET_PATH);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockfd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
        perror("epoll_ctl");
        close(sockfd);
        return EXIT_FAILURE;
    }

    struct epoll_event events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    int nfds;

    while (true) {
        nfds = epoll_wait(epfd, events, MAX_EVENTS, -1); // block
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                int connfd;
                while ((connfd = accept(sockfd, NULL, NULL)) != -1) {
                    std::cout << "新连接建立: " << connfd << std::endl;
                    setNonBlocking(connfd);

                    event.events = EPOLLIN | EPOLLOUT | EPOLLET;
                    event.data.fd = connfd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event) == -1) {
                        perror("epoll_ctl: connfd");
                        close(connfd);
                    }
                }
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("accept");
                }
            } else if (events[i].events & EPOLLIN) {
                int fd = events[i].data.fd;
                while (true) {
                    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
                    if (bytes_read > 0) {
                        std::cout << "读取了 " << bytes_read << " 字节: "
                                  << std::string(buffer, bytes_read) << std::endl;
                    } else if (bytes_read == 0) {
                        std::cout << "客户端关闭连接: " << fd << std::endl;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        break;
                    } else { // bytes_read == -1
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // no more data
                            break;
                        }
                        perror("read");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        break;
                    }
                }
            }
        }
    }
    close(sockfd);
    close(epfd);
    unlink(SOCKET_PATH);
    return EXIT_SUCCESS;
}
