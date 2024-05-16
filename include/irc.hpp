/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:58:01 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 15:45:13 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <vector>
#include <queue>
#include <poll.h>
#include <arpa/inet.h>
#include <stdexcept>

#define BUFFER_SIZE 10000

#define MODE_I 1
#define MODE_K 2
#define MODE_L 4
#define MODE_O 8
#define MODE_T 16

class User
{
private:
    std::string nickname, username, realname, host, server, input_buffer, pass;
    bool is_authenticated;
    int fd;
    std::queue<std::string> output_queue;
    User(const User &u);
    User &operator=(const User &c);
public:
    User();
    ~User();
    void set_nickname(const std::string &nickname);
    const std::string get_nickname() const;
    void set_username(const std::string &username);
    const std::string get_username() const;
    void set_realname(const std::string &realname);
    const std::string get_realname() const;
    void set_host(const std::string &host);
    const std::string get_host() const;
    void set_server(const std::string &server);
    const std::string get_server() const;
    void set_pass(const std::string &server);
    const std::string get_pass() const;
    void set_input_buffer(const std::string &input_buffer);
    const std::string get_input_buffer() const;
    void set_is_authenticated(bool is_authenticated);
    bool get_is_authenticated() const;
    void set_fd(int fd);
    int get_fd() const;
    void enqueue_message(const std::string &message);
    const std::string dequeue_message();
    bool has_queued_messages() const;
};

class Channel
{
private:
    std::string topic;
    int mode;
    std::vector<User *> operators, users, invitations;
    Channel(const Channel &c);
    Channel &operator=(const Channel &c);
public:
    Channel();
    ~Channel();
    void set_topic(const std::string &topic);
    const std::string get_topic() const;
    void set_mode(int mode);
    void unset_mode(int mode);
    bool has_mode(int mode) const;
    void add_operator(User *user);
    void remove_operator(const std::string &nickname);
    std::vector<User *>::iterator find_operator(const std::string &nickname);
    bool is_operator(const std::string &nickname);
    void add_user(User *user);
    void remove_user(const std::string &nickname);
    std::vector<User *>::iterator find_user(const std::string &nickname);
    bool is_user(const std::string &nickname);
    void add_invitation(User *user);
    void remove_invitation(const std::string &nickname);
    std::vector<User *>::iterator find_invitation(const std::string &nickname);
    bool is_invitation(const std::string &nickname);
};

class SocketManager;

class Server
{
private:
    std::string server_name, server_version, pass;
    std::vector<User *> registered;
    std::vector<Channel *> channels;
    SocketManager *socket_manager;
    Server();
    Server(const Server &s);
    Server &operator=(const Server &s);
public:
    Server(const std::string &pass);
    ~Server();
    const std::string get_server_name() const;
    const std::string get_server_version() const;
    const std::string get_pass() const;
    void set_socket_manager(SocketManager *socket_manager);
    void add_registered(User *user);
    void remove_registered(const std::string &nickname);
    void remove_registered(int fd);
    std::vector<User *>::iterator find_registered(const std::string &nickname);
    bool is_registered(const std::string &nickname);
    void add_channel(Channel *c);
    void remove_channel(const std::string &topic);
    std::vector<Channel *>::iterator find_channel(const std::string &topic);
    bool is_channel(const std::string &topic);
    std::vector<User *>::iterator find_user_by_fd(int fd);
    void end_user_connection(int fd);
    void print_server_status(const std::string &last_message) const;
};

class Command
{
private:
    std::vector<std::string> arguments;
    std::string message;
    Command();
    Command(const Command &c);
    Command &operator=(const Command &c);
public:
    Command(const std::string &message);
    ~Command();
    void run_command(Server *s, User *u);
    std::vector<std::string> &get_arguments();
};

class SocketManager
{
private:
    std::vector<pollfd> pollfds;
    int port;
    int manager_fd;
    sockaddr_in address;
    Server &server;
    SocketManager();
    SocketManager(const SocketManager &s);
    SocketManager &operator=(const SocketManager &s);
public:
    SocketManager(int port, Server &server);
    ~SocketManager();
    void loop();
    void new_connection(pollfd pfd);
    void end_connection(pollfd pfd);
    void receive_message(pollfd pfd, std::vector<pollfd> &to_close);
    void send_messages(pollfd pfd, std::vector<pollfd> &to_close);
};

class CloseConnection: public std::runtime_error
{
public:
    CloseConnection(const std::string &what);
    ~CloseConnection();
};

class StopServer: public std::runtime_error
{
public:
    StopServer(const std::string &what);
    ~StopServer();
};

std::vector<User *>::iterator find_user_by_nickname(
    std::vector<User *> &v, const std::string &nickname
    );
std::vector<Channel *>::iterator find_channel_by_topic(
    std::vector<Channel *> &c, const std::string &topic
    );

std::string nickname_to_lower(const std::string &nickname);

void down_command(Command *c, Server *s, User *u);
void invite_command(Command *c, Server *s, User *u);
void join_command(Command *c, Server *s, User *u);
void kick_command(Command *c, Server *s, User *u);
void mode_command(Command *c, Server *s, User *u);
void nick_command(Command *c, Server *s, User *u);
void notice_command(Command *c, Server *s, User *u);
void part_command(Command *c, Server *s, User *u);
void pass_command(Command *c, Server *s, User *u);
void privmsg_command(Command *c, Server *s, User *u);
void quit_command(Command *c, Server *s, User *u);
void topic_command(Command *c, Server *s, User *u);
void up_command(Command *c, Server *s, User *u);
void user_command(Command *c, Server *s, User *u);
void who_command(Command *c, Server *s, User *u);

std::string rpl_welcome(Server *s, User *u);
std::string rpl_yourhost(Server *s, User *u);
std::string rpl_created(Server *s, User *u);
std::string rpl_myinfo(Server *s, User *u);
std::string err_unknowncommand(Command *c, Server *s, User *u);
std::string err_nonicknamegiven(Server *s, User *u);
std::string err_erroneusnickname(Command *c, Server *s, User *u);
std::string err_passwdmismatch(Server *s, User *u);
std::string err_nicknameinuse(Command *c, Server *s, User *u);
std::string err_nickcollision(Command *c, Server *s, User *u);
std::string err_needmoreparams(Command *c, Server *s, User *u);
std::string err_alreadyregistered(Command *c, Server *s, User *u);

std::vector<std::string> split(
    const std::string &s, char del, bool include_delimiter);

#endif
