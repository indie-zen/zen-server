# Zen App Server

Zen App Server is a high performance application container.  The 1.x version was primarily used as part of an Enterprise Service Bus, but the improvements being made for the 2.0 implemtation makes it a great solution for public web based applications as well as personal / private web based application server.

It is the primary application server used for the [aethos](https://github.com/aethosio) web based OS.

## Multiple scripting languages

Through the use of Zen Scripting, Applications hosted by Zen App Server can be written in C++ as well as any of the scripting languages supported by the Zen Scripting framework, including Lua, Python, JavaScript and others via third party plugins.

## Sandbox

Applications can now run in sandboxes, allowing more granular restrictions on access to network, file system and other resources.  This creates an environment where untrusted applications can execute without exposing the rest of your server to untrusted code.

## Multi-threaded

Sites and applications can now be serviced with more than one thread.  New syncronization objects allow for communication across threads (and even across multiple processes when Zen Server is configured as a cluster)

## Version 2.0

Zen App Server 2.0 is a complete re-write from Zen App Server 1.xx.  It uses the new 2.0 version of Zen Plugin, Zen Scripting, and Zen Enterprise.