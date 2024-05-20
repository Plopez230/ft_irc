<h1 align="center">
        FT_IRC
</h1>

<p align="center">
	<b><i>42 CURSUS FT_IRC PROJECT</i></b><br>
</p>


## 💡 About the project

Welcome to the ft_irc project! This project implements a basic IRC server that 
supports several fundamental IRC commands. It's a great starting point for 
understanding how IRC works and for building more complex IRC-related 
applications.

## Features
Commands Overview

- **PASS \<password>**: Set a connection password (if required by the server).
- **NICK \<nickname>**: Choose or change your nickname.
- **USER \<username> \<hostname> \<servername> \<realname>**: Provide your 
username and other details.
- **JOIN <#channel>**: Join a channel.
- **PART <#channel>**: Leave a channel.
- **KICK <#channel> \<user>**: Remove a user from a channel.
- **MODE <#channel> \<mode>**: Change channel modes (e.g., +o to give operator 
status).
- **TOPIC <#channel> \<topic>**: Set the topic of a channel.
- **QUIT [message]**: Disconnect from the server with an optional message.
- **PRIVMSG \<recipient> \<message>**: Send a private message to a user or 
channel.
- **INVITE \<user> <#channel>**: Invite a user to a channel.

## Running the Server
The program accepts two parameters: the first is the port it will listen on,
and the second is the server password.
```
./ircserv <port> <password>
```

## Connecting to the Server
You can connect to the server using any IRC client, such as HexChat, irssi, or
mIRC.

## License
This project is licensed under the MIT License. See the LICENSE file for
details.

## Authors
- [Juan Ariza](https://github.com/jariza-o)
- [Pablo Lopez](https://github.com/Plopez230)
