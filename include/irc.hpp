/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:58:01 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/28 12:40:49 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <vector>
#include <queue>
#include <poll.h>
#include <stdexcept>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fstream>

#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 32
#define MAX_NICKNAME_LENGTH 9

#define MODE_I 1
#define MODE_K 2
#define MODE_L 4
#define MODE_T 8

// ./src/user.cpp

class User
{
private:
    std::string nickname, username, realname, host, server, input_buffer, pass;
    bool is_registered;
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
    void set_is_registered(bool is_registered);
    bool get_is_registered() const;
    void set_fd(int fd);
    int get_fd() const;
    void enqueue_message(const std::string &message);
    const std::string dequeue_message();
    bool has_queued_messages() const;
};

std::vector<User *>::iterator find_user_by_nickname(
    std::vector<User *> &v, const std::string &nickname);
std::vector<User *>::iterator find_user_by_fd(std::vector<User *> &v, int fd);

// ./src/channel.cpp

class Channel
{
private:
    std::string name, topic, pass;
    int mode;
    size_t max_users;
    std::vector<User *> operators, users, invitations;
    Channel(const Channel &c);
    Channel &operator=(const Channel &c);
public:
    Channel();
    ~Channel();
    void set_name(const std::string &topic);
    const std::string get_name() const;
    void set_topic(const std::string &topic);
    const std::string get_topic() const;
    void set_max_users(size_t max_users);
    size_t get_max_users() const;
    void set_pass(const std::string &pass);
    const std::string get_pass() const;
    void set_mode(int mode);
    void unset_mode(int mode);
    bool has_mode(int mode) const;
    void add_operator(User *user);
    void remove_operator(const std::string &nickname);
    User *find_operator(const std::string &nickname);
    bool is_operator(const std::string &nickname);
    void add_user(User *user);
    void remove_user(const std::string &nickname);
    User *find_user(const std::string &nickname);
    bool is_user(const std::string &nickname);
    void add_invitation(User *user);
    void remove_invitation(const std::string &nickname);
    User *find_invitation(const std::string &nickname);
    bool is_invitation(const std::string &nickname);
    bool is_full() const;
    std::string get_nicknames();
    void enqueue_message(const std::string &message);
    void enqueue_message(const std::string &message, User *except);
};

std::vector<Channel *>::iterator find_channel_by_name(
    std::vector<Channel *> &c, const std::string &topic);

// ./src/server.cpp

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
    User *find_registered(const std::string &nickname);
    User *find_registered(int fd);
    bool is_registered(const std::string &nickname);
    void add_channel(Channel *c);
    void remove_channel(const std::string &topic);
    Channel *find_channel(const std::string &topic);
    bool is_channel(const std::string &topic);
    void end_user_connection(int fd);
    void print_server_status(const std::string &last_message) const;
    void enqueue_message_in_user_channels(const std::string &msg, User *user);
};

// ./src/command.cpp

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
    const std::string &get_message() const;
    const std::string &argument(size_t position);
    size_t size();
};

std::vector<std::string> split(
    const std::string &s, char del, bool include_delimiter);

// ./src/tracer.cpp

class Tracer
{
private:
    std::ofstream *file;
    bool enabled;
    Tracer(Tracer const &t);
    Tracer &operator=(Tracer const &t);
public:
    Tracer();
    ~Tracer();
    void trace_output(int fd, std::string msg);
    void trace_input(int fd, std::string msg);
    void end_connection(int fd);
};

// ./src/socket_manager.cpp

class SocketManager
{
private:
    std::vector<pollfd> pollfds;
    char *port;
    int manager_fd;
    struct addrinfo hints;
    struct addrinfo *res;
    Server &server;
    Tracer tracer;
    SocketManager();
    SocketManager(const SocketManager &s);
    SocketManager &operator=(const SocketManager &s);
    void new_connection(pollfd pfd);
    void end_connection(pollfd pfd);
    void receive_message(pollfd pfd, std::vector<pollfd> &to_close);
    void send_messages(pollfd pfd, std::vector<pollfd> &to_close);
public:
    SocketManager(char *port, Server &server);
    ~SocketManager();
    void loop();
};

class CloseConnection: public std::runtime_error
{
public:
    CloseConnection(const std::string &what);
};

class StopServer: public std::runtime_error
{
public:
    StopServer(const std::string &what);
};

// ./src/validation.cpp

std::string to_lower(const std::string &nickname);
bool is_valid_channel_name(const std::string &channel_name);
bool is_valid_nickname(const std::string &nickname);
void register_user(Server *s, User *u);

// ./src/commands/invite.cpp

void invite_command(Command *c, Server *s, User *u);

// ./src/commands/join.cpp

void join_command(Command *c, Server *s, User *u);

// ./src/commands/kick.cpp

void kick_command(Command *c, Server *s, User *u);

// ./src/commands/mode.cpp

void mode_command(Command *c, Server *s, User *u);

// ./src/commands/nick.cpp

void nick_command(Command *c, Server *s, User *u);

// ./src/commands/part.cpp

void part_command(Command *c, Server *s, User *u);

// ./src/commands/pass.cpp

void pass_command(Command *c, Server *s, User *u);

// ./src/commands/privmsg.cpp

void privmsg_command(Command *c, Server *s, User *u);

// ./src/commands/quit.cpp

void quit_command(Command *c, Server *s, User *u);

// ./src/commands/topic.cpp

void topic_command(Command *c, Server *s, User *u);

// ./src/commands/user.cpp

void user_command(Command *c, Server *s, User *u);

// ./src/replies.cpp

std::string user_jid(User *u);
std::string rpl_welcome(Server *s, User *u);
std::string rpl_yourhost(Server *s, User *u);
std::string rpl_created(Server *s, User *u);
std::string rpl_myinfo(Server *s, User *u);
std::string rpl_topic(Server *s, Channel *c, User *u);
std::string err_unknowncommand(Command *c, Server *s, User *u);
std::string err_nonicknamegiven(Server *s, User *u);
std::string err_erroneusnickname(Command *c, Server *s, User *u);
std::string err_passwdmismatch(Server *s, User *u);
std::string err_nicknameinuse(Command *c, Server *s, User *u);
std::string err_nickcollision(Command *c, Server *s, User *u);
std::string err_needmoreparams(Command *c, Server *s, User *u);
std::string err_alreadyregistered(Server *s, User *u);
std::string err_inviteonlychan(Server *s, Channel *c, User *u);
std::string err_badchannelkey(Server *s, Channel *c, User *u);
std::string err_channelisfull(Server *s, Channel *c, User *u);
std::string err_nosuchchannel(Server *s, const std::string &c, User *u);
std::string rpl_namreply(Server *s, Channel *c, User *u);
std::string rpl_endofnames(Server *s, Channel *c, User *u);
std::string err_notonchannel(Server *s, User *u, const std::string &ch);
std::string err_unknownmode(Server *s, User *u, char mode);
std::string rpl_channelmodeis(Server *s, Channel *c, User *u,
    const std::string &mode);
std::string err_keyset(Server *s, Channel *c, User *u);
std::string err_nosuchnick(Server *s, User *u, const std::string &nickname);
std::string err_chanoprivsneeded(Server *s, Channel *c, User *u);
std::string command_reply(Command *c, User *u);
std::string command_reply(Command *c, User *u, const std::string &args);
std::string err_notexttosend(Server *s, User *u);
std::string err_norecipient(Command *c, Server *s, User *u);
std::string rpl_inviting(Server *s, Channel *c, User *u, User *user);
std::string err_useronchannel(Server *s, Channel *c, User *u, User *user);
std::string rpl_notopic(Server *s, Channel *c, User *u);
std::string err_umodeunknownflag(Server *s, User *u);
std::string err_notregistered(Server *s, User *u);

#endif
