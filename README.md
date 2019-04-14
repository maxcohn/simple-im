# Group IM client and server built with C

## What is this?
I had never worked with sockets in C and I figured that it would be a
good learning experience for learning more about basic networking.
Prior to this project, I had only used sockets directly once, and that
was in Java. I figured that if I could do it in C, I'd be able to do
it in any other languages I cared to.

This is completely for practice and example. This isn't intended to be
used for any real application, but instead a neat exercise in sockets
and threading.

# Pieces
## Server

1. The server established a socket which clients can connect to.

2. Upon connecting, a new thread is spawned to receive data from the user.

3. Whenever a new message is received, all other users are notified.

## Client

1. The client connects to the server's socket.

2. After connecting, a new thread is spawned to receive data from the server.

3. Whenever a new message is received, it is printed to the display.

# TODO

* Organize code
    * Should user_t struct code belong in it's own file?
    * Should thread code belong in it's own file?
* Document all code
* Handle users disconnecting
* Make sure no memory/thread leaks