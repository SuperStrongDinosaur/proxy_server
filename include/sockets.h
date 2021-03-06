#ifndef SOCKET_H
#define SOCKET_H
#include <memory.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <list>
//#include "timeout.h"


const uint32_t BUFFER_SIZE = 20000;

struct file_descriptor {
    
    file_descriptor& operator=(const file_descriptor&) = delete;
    file_descriptor(const file_descriptor&) = delete;
    
    file_descriptor();
    file_descriptor(int fd);
    file_descriptor(file_descriptor&&);
    file_descriptor& operator=(file_descriptor&&);
    
    void set_fd(int) noexcept;
    int get_fd() const noexcept;
    int release() noexcept;
    
    ~file_descriptor();
    
protected:
    int fd;
};

struct socket_wrap : public file_descriptor {
    socket_wrap& operator=(const socket_wrap&) = delete;
    socket_wrap(const socket_wrap&) = delete;
    
    socket_wrap() = default;
    socket_wrap(int fd);
    socket_wrap(socket_wrap&&) = default;
    socket_wrap& operator=(socket_wrap&&) = default;
    
    static socket_wrap accept(int);
    
    ptrdiff_t write(std::string const& msg);
    std::string read(size_t buffer_size);
    
    ~socket_wrap();
};

struct server;

/*
 / Client owns server.
 */
struct client {
    client(const client&) = delete;
    client& operator=(client const&) = delete;
    
    client(int);
    
    int get_fd() const noexcept;
    int get_server_fd() const noexcept;
    bool has_server() const noexcept;
    
    void bind(struct server* new_server);
    void unbind();
    
    std::string& get_buffer();
    size_t get_buffer_size() const noexcept;
    void append(std::string&);
    bool has_capcacity() const noexcept;
    
    size_t read(size_t);
    size_t write();
    
    void get_msg();
    void send_msg();
    
    std::string get_host() const noexcept;
    
    ~client();
private:
    std::string buffer;
    socket_wrap socket;
    std::unique_ptr<struct server> server;
};

struct server {
    server(const server&) = delete;
    server& operator=(const server&) = delete;
    
    server(int);
    server(sockaddr);
    
    int get_fd() const noexcept;
    int get_client_fd() const noexcept;
    void bind(struct client*);
    
    void append(std::string&);
    std::string& get_buffer() noexcept;
    bool buffer_empty() const noexcept;
    
    std::string read(size_t);
    size_t write();
    
    void send_msg();
    void get_msg();
    
    void set_host(std::string const&);
    std::string get_host() const noexcept;
    
    void disconnect();
    
    ~server();
private:
    std::string buffer, host;
    socket_wrap socket;
    struct client* client;
};


#endif
