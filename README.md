# 42_IRC
Internet Relay Chat Server in C++. ChatGPT API included (ChatGPT Bot)

-> Internet Relay Chat (IRC) is a form of real-time text messaging communication that was one of the earliest forms of online chat. It was created by Jarkko Oikarinen in August 1989 while he was working at the University of Oulu in Finland. IRC has played a significant role in the early development of the internet. Channels and private messages are supported.

> Implemented some basic commands like: PASS, NICK, USER, JOIN, TOPIC, PRIVMSG, INVITE, KICK, MODE (i, t, k, o), QUIT, PART + CHATGPT

> READ about [RFC1459](https://datatracker.ietf.org/doc/html/rfc1459) for more information about the IRC protocol.

### How to run the server 

```make```

```./irc_server <port> <password>```


### How to connect with a client 🔗

```nc localhost <port>```

OR

```sudo apt install konversation```

```konversation```

## GPT Bot with Konversation IRC Client 🤖💬

<img src="https://github.com/jblackiex/42_IRC/blob/main/gptbot.gif" alt="gptbot" width="auto" height="auto" border="2"/>
