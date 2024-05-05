# 42_IRC
Internet Relay Chat Server in C++. ChatGPT API included (ChatGPT Bot)

-> Internet Relay Chat (IRC) is a form of real-time text messaging communication that was one of the earliest forms of online chat. It was created by Jarkko Oikarinen in August 1989 while he was working at the University of Oulu in Finland. IRC has played a significant role in the early development of the internet. Channels and private messages are supported.

> We implemented some basic commands like: PASS, NICK, USER, JOIN, TOPIC, PRIVMSG, INVITE, KICK, MODE (i, t, k, o), QUIT, PART + CHATGPT

> READ about [RFC1459](https://datatracker.ietf.org/doc/html/rfc1459) for more information about the IRC protocol.

### How to run the server 

```make```

```./irc_server <port> <password>```


### How to connect with a client 🔗

```nc localhost <port>```

OR

```sudo apt install konversation```

```konversation```

## ChatGPT Bot with Konversation IRC Client 🤖💬

<iframe width="560" height="315" src="https://www.youtube.com/embed/h29gZz1peB0" frameborder="1" allowfullscreen></iframe>


Built by [@nbordoni](https://github.com/jblackiex), [@lnascari](https://github.com/lnascari) and [@gpaoline](https://github.com/lnascari)