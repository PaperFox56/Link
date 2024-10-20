# Link
Link is a library created as a interface between two or more softwares.
It implement an abstraction of the vanilla socket system for communication between distant 
machines, but with its packet's listener system, it can also be use for thread based softwares.

## How to use the sources
Intended to be used on any platform, the project use the cross-platform library *libsocket* that can 
be found at https://github.com/dermesser/libsocket

To use this library, you shall first get the source code on this repo, you can place the files directly 
in you project or compile it as static library. 
Then get the *libsocket*'s source code in the link above, build and install it.
While building your own project, add the *libsocket*'s static library to the compiler command, and the
folder containing the *libsocket*'s headers as an include folder:

  g++ \<regular options\> -L \<folder containing the static library\> libsocket++.so -I \<folder containing the headers\>

## Using the library
The library come with three headers

*packet.h* contains a structure, *packet*, that encapsulate all packet's fields (read the comments for more informations),
and a class *Packet Listener* working as a queue. This last class is thread safe and can thuus be used to send data 
between threads.

*link.h* contains the network system implemented as the *Link* class
A object instancied from this class can be use as a server or a client with the *startAsServer* and 
*startAsClient* methods. Then, with the *send* method, you can send a whole packet through the link. Note that 
the field *target* of the packet will automatically used to choose the recever.
