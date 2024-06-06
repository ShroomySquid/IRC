# IRC -- Internet Relay Protocol

This program is a server made following the IRC protocol. It has the standard features of an IRC server: you can send messages, create and join channels, change permissions, etc.
It is specifically made to work with the Weechat client, but it also work well with netcat.

This project is part of the 42 Quebec cursus and was made with the cooperation of [joeroeg](https://github.com/joeroeg). It is specifically made to run on MacOS.

[General documentation for IRC](https://modern.ircdocs.horse/)

[General documentation for Weechat](https://weechat.org/doc/)

## Run the server

To run the program and therefore host the server, you will need to pass ./ircserv in a terminal window. 
You also need to provide it with a valid port and a valid password. By default IRC servers use the 6667 port.
Example:
```bash
./ircserv 6667 patate
```
To exit the program, simply press CRTL+C. Binding errors may occur if you try to restart too fast the program after quitting it. Simply wait around 10-20 seconds and
you should be able to start again with the same port.

## Join the server

To join the server as a Client, you will need to connect to the server with the correct ip address of the host and the port of the server. 
Example with netcat:
```bash
./nc 10.45.3.192 6667
```

You will then need to enter the password and provide a nickname and a username.
Example:
```bash
PASS password
```
```bash
NICK Jack
```
```bash
USER Jack
```

Once you are registered, you can interact with the server and other users. This server support the JOIN command to both join a create a channel, the TOPIC command 
to check or set the topic of a channel, the PRIVMSG command to send a message to another user or to a channel, the KICK command to kick a user from a channel,
the MODE command to set the modes of a channel, the INVITE command to invite someone to the channel and the PART command to leave a channel.
If you need more details about commands in IRC, you can consult the [IRC documentation](https://modern.ircdocs.horse/).

To join the server as a client with Weechat, Open Weechat in a terminal window and put the following commands:

```bash
/server add [server name] [ip address]/[port] -notls
/set irc.server.[server name].password [password]
/set irc.server.[server name].nicks [nickname]
/set irc.server.[server name].username [username]
/connect [server name]
```

Example:
```bash
/server add irc 10.12.2.8/6667 -notls
/set irc.server.irc.password patate
/set irc.server.irc.nicks Jack
/set irc.server.irc.username Jack
/connect irc
```

If you need more information on how to use weechat, you can consult the [Weechat documentation](https://weechat.org/doc/).

To leave the server, simply run the QUIT command (or /quit in weechat).

## Installation

- Clone the repository in a folder of your choice.

```bash
git@github.com:ShroomySquid/IRC.git
```

- Run the command "make" in the terminal window in the folder to compile the program and then it's done!.
