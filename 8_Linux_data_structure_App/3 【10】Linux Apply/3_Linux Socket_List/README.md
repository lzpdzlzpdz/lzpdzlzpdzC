linux中socket的理解

一、socket

　　一般来说socket有一个别名也叫做套接字。

　　socket起源于Unix，都可以用“打开open –> 读写write/read –> 关闭close”模式来操作。Socket就是该模式的一个实现，socket即是一种特殊的文件，一些socket函数就是对其进行的操作（读/写IO、打开、关闭）。

　　说白了Socket是应用层与TCP/IP协议族通信的中间软件抽象层，它是一组接口。在设计模式中，Socket其实就是一个门面模式，它把复杂的TCP/IP协议族隐藏在Socket接口后面，对用户来说，一组简单的接口就是全部，让Socket去组织数据，以符合指定的协议，而不需要让用户自己去定义什么时候需要指定哪个协议哪个函数。

    其实socket也没有层的概念，它只是一个facade设计模式的应用，让编程变的更简单。是一个软件抽象层。在网络编程中，我们大量用的都是通过socket实现的。

1.1套接字描述符

　　其实就是一个整数，我们最熟悉的句柄是0、1、2三个，0是标准输入，1是标准输出，2是标准错误输出。0、1、2是整数表示的，对应的FILE *结构的表示就是stdin、stdout、stderr

　　套接字API最初是作为UNIX操作系统的一部分而开发的，所以套接字API与系统的其他I/O设备集成在一起。特别是，当应用程序要为因特网通信而创建一个套接字（socket）时，操作系统就返回一个小整数作为描述符（descriptor）来标识这个套接字。然后，应用程序以该描述符作为传递参数，通过调用函数来完成某种操作（例如通过网络传送数据或接收输入的数据）。

　　在许多操作系统中，套接字描述符和其他I/O描述符是集成在一起的，所以应用程序可以对文件进行套接字I/O或I/O读/写操作。

　　当应用程序要创建一个套接字时，操作系统就返回一个小整数作为描述符，应用程序则使用这个描述符来引用该套接字需要I/O请求的应用程序请求操作系统打开一个文件。操作系统就创建一个文件描述符提供给应用程序访问文件。从应用程序的角度看，文件描述符是一个整数，应用程序可以用它来读写文件。下图显示，操作系统如何把文件描述符实现为一个指针数组，这些指针指向内部数据结构。



     对于每个程序系统都有一张单独的表。精确地讲，系统为每个运行的进程维护一张单独的文件描述符表。当进程打开一个文件时，系统把一个指向此文件内部数据结构的指针写入文件描述符表，并把该表的索引值返回给调用者 。应用程序只需记住这个描述符，并在以后操作该文件时使用它。操作系统把该描述符作为索引访问进程描述符表，通过指针找到保存该文件所有的信息的数据结构。

针对套接字的系统数据结构：

   1）、套接字API里有个函数socket，它就是用来创建一个套接字。套接字设计的总体思路是，单个系统调用就可以创建任何套接字，因为套接字是相当笼统的。一旦套接字创建后，应用程序还需要调用其他函数来指定具体细节。例如调用socket将创建一个新的描述符条目：



   2）、虽然套接字的内部数据结构包含很多字段，但是系统创建套接字后，大多数字字段没有填写。应用程序创建套接字后在该套接字可以使用之前，必须调用其他的过程来填充这些字段。

二、基本的socket接口函数



　　服务器端先初始化/创建Socket，然后与端口绑定/绑定地址(bind)，对端口进行监听(listen)，调用accept阻塞/等待连续，等待客户端连接。在这时如果有个客户端初始化一个Socket，然后连接服务器(connect)，如果连接成功，这时客户端与服务器端的连接就建立了。客户端发送数据请求，服务器端接收请求并处理请求，然后把回应数据发送给客户端，客户端读取数据，最后关闭连接，一次交互结束。

2.1socket函数

函数原型：

　　int socket(int protofamily, int type, int protocol);

返回值：

　　//返回sockfd     sockfd是描述符,类似于open函数。

函数功能：

　　socket函数对应于普通文件的打开操作。普通文件的打开操作返回一个文件描述字，而socket()用于创建一个socket描述符（socket descriptor），它唯一标识一个socket。这个socket描述字跟文件描述字一样，后续的操作都有用到它，把它作为参数，通过它来进行一些读写操作。

函数参数：

　　protofamily：即协议域，又称为协议族（family）。常用的协议族有，AF_INET(IPV4)、AF_INET6(IPV6)、AF_LOCAL（或称AF_UNIX，Unix域socket）、AF_ROUTE等等。协议族决定了socket的地址类型，在通信中必须采用对应的地址，如AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合、AF_UNIX决定了要用一个绝对路径名作为地址。

     

　　type：指定socket类型。常用的socket类型有，SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET等等。

     

　　protocol：就是指定协议。常用的协议有，IPPROTO_TCP、IPPTOTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC等，它们分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议

　　注意：并不是上面的type和protocol可以随意组合的，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当protocol为0时，会自动选择type类型对应的默认协议。

当我们调用socket创建一个socket时，返回的socket描述字它存在于协议族（address family，AF_XXX）空间中，但没有一个具体的地址。如果想要给它赋值一个地址，就必须调用bind()函数，否则就当调用connect()、listen()时系统会自动随机分配一个端口。

2.2bind()函数

函数功能：

　　bind()函数把一个地址族中的特定地址赋给socket，也可以说是绑定ip端口和socket。例如对应AF_INET、AF_INET6就是把一个ipv4或ipv6地址和端口号组合赋给socket。

函数原型：

    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

函数参数：

　　1.函数的三个参数分别为：sockfd：即socket描述字，它是通过socket()函数创建了，唯一标识一个socket。bind()函数就是将给这个描述字绑定一个名字。

　　2.addr：一个const struct sockaddr *指针，指向要绑定给sockfd的协议地址。这个地址结构根据地址创建socket时的地址协议族的不同而不同，

　　3.addrlen：对应的是地址的长度。

通用函数类型：

struct sockaddr{
　　sa_family_t  sa_family;
　　char         sa_data[14];
}
如ipv4对应的是：

struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order 2字节*/
    struct in_addr sin_addr;   /* internet address 4字节*/
　　unsigned char sin_zero[8];
};
/* Internet address. */
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};
ipv6对应的是： 

struct sockaddr_in6 { 
    sa_family_t     sin6_family;   /* AF_INET6 */ 
    in_port_t       sin6_port;     /* port number */ 
    uint32_t        sin6_flowinfo; /* IPv6 flow information */ 
    struct in6_addr sin6_addr;     /* IPv6 address */ 
    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */ 
};
struct in6_addr { 
    unsigned char   s6_addr[16];   /* IPv6 address */ 
};
Unix域对应的是： 

#define UNIX_PATH_MAX    108
struct sockaddr_un { 
    sa_family_t sun_family;               /* AF_UNIX */ 
    char        sun_path[UNIX_PATH_MAX];  /* pathname */ 
};
　　通常服务器在启动的时候都会绑定一个众所周知的地址（如ip地址+端口号），用于提供服务，客户就可以通过它来接连服务器；而客户端就不用指定，有系统自动分配一个端口号和自身的ip地址组合。这就是为什么通常服务器端在listen之前会调用bind()，而客户端就不会调用，而是在connect()时由系统随机生成一个。

2.2.1地址转换

　　int_addr_t indet_addr(const char *cp)

　　功能：将字符串形式的IP地址转化为整数型的IP地址（网络字节序）

　　范例：int_addr.saddr=inet_addr("192.168.1.1");

　　char *inet_ntoa(struct in_addr)

　　功能：将整数形式的IP地址转化为字符串形式的IP地址

2.2.2网络字节序

　　网络字节序定义：收到的第一个字节被当作高位看待，这就要求发送端发送的第一个字节应当是高位。而在发送端发送数据时，发送的第一个字节是该数字在内存中起始地址对应的字节。可见多字节数值在发送前，在内存中数值应该以大端法存放。 
　　网络字节序说是大端字节序。

　　小端法(Little-Endian)就是低位字节排放在内存的低地址端即该值的起始地址，高位字节排放在内存的高地址端。 
　　大端法(Big-Endian)就是高位字节排放在内存的低地址端即该值的起始地址，低位字节排放在内存的高地址端。

网络字节序转化：---->不论是数据还是地址只要大于两个字节就必须转换

　　uint32_t htonl(uint32_t hostlong);
　　将32位的数据从主机字节序转换为网络字节序
　　in_addr.saddr = htonl(INADDR_ANY)

　　uint16_t htons(uint16_t hostshort);
　　将16位的数据从主机字节序转换为网络字节序

　　uint32_t ntohl(uint32_t netlong);
　　将32位的数据从网络字节序转换为主机字节序

　　uint16_t ntohs(uint16_t netshort);
　　将16位的数据从网络字节序转换为主机字节序

2.3、listen()、connect()函数

　　如果作为一个服务器，在调用socket()、bind()之后就会调用listen()来监听这个socket，如果客户端这时调用connect()发出连接请求，服务器端就会接收到这个请求。

  int listen(int sockfd, int backlog);
  int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
　　listen函数的第一个参数即为要监听的socket描述字，第二个参数为相应socket可以排队的最大连接个数。socket()函数创建的socket默认是一个主动类型的，listen函数将socket变为被动类型的，等待客户的连接请求。

　　connect函数的第一个参数即为客户端的socket描述字，第二参数为服务器的socket地址，第三个参数为socket地址的长度。客户端通过调用connect函数来建立与TCP服务器的连接。成功返回0，若连接失败则返回-1。

2.4、accept()函数

　　TCP服务器端依次调用socket()、bind()、listen()之后，就会监听指定的socket地址了。TCP客户端依次调用socket()、connect()之后就向TCP服务器发送了一个连接请求。TCP服务器监听到这个请求之后，就会调用accept()函数取接收请求，这样连接就建立好了。之后就可以开始网络I/O操作了，即类同于普通文件的读写I/O操作。

  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); //返回连接connect_fd
参数sockfd
　　参数sockfd就是上面解释中的监听套接字，这个套接字用来监听一个端口，当有一个客户与服务器连接时，它使用这个一个端口号，而此时这个端口号正与这个套接字关联。当然客户不知道套接字这些细节，它只知道一个地址和一个端口号。
参数addr
　　这是一个结果参数，它用来接受一个返回值，这返回值指定客户端的地址，当然这个地址是通过某个地址结构来描述的，用户应该知道这一个什么样的地址结构。如果对客户的地址不感兴趣，那么可以把这个值设置为NULL。
参数len
如同大家所认为的，它也是结果的参数，用来接受上述addr的结构的大小的，它指明addr结构所占有的字节个数。同样的，它也可以被设置为NULL。 
如果accept成功返回，则服务器与客户已经正确建立连接了，此时服务器通过accept返回的套接字来完成与客户的通信。

注意：

　　accept默认会阻塞进程，直到有一个客户连接建立后返回，它返回的是一个新可用的套接字，这个套接字是连接套接字。

此时我们需要区分两种套接字，

　　监听套接字: 监听套接字正如accept的参数sockfd，它是监听套接字，在调用listen函数之后，是服务器开始调用socket()函数生成的，称为监听socket描述字(监听套接字)

    连接套接字：一个套接字会从主动连接的套接字变身为一个监听套接字；而accept函数返回的是已连接socket描述字(一个连接套接字)，它代表着一个网络已经存在的点点连接。

　　一个服务器通常通常仅仅只创建一个监听socket描述字，它在该服务器的生命周期内一直存在。内核为每个由服务器进程接受的客户连接创建了一个已连接socket描述字，当服务器完成了对某个客户的服务，相应的已连接socket描述字就被关闭。

　　连接套接字socketfd_new 并没有占用新的端口与客户端通信，依然使用的是与监听套接字socketfd一样的端口号

2.5、recv()/send()函数

　　当然也可以使用其他函数来实现数据传送，比如read和write。

2.5.1send函数

 ssize_t send(int sockfd, const void *buf, size_t len, int flags);
　　不论是客户还是服务器应用程序都用send函数来向TCP连接的另一端发送数据。

　　客户程序一般用send函数向服务器发送请求，而服务器则通常用send函数来向客户程序发送应答。

　　该函数的第一个参数指定发送端套接字描述符；

　　第二个参数指明一个存放应用程序要发送数据的缓冲区；

　　第三个参数指明实际要发送的数据的字节数；

　　第四个参数一般置0。

　　这里只描述同步Socket的send函数的执行流程。当调用该函数时，send先比较待发送数据的长度len和套接字s的发送缓冲的 长度，如果len大于s的发送缓冲区的长度，该函数返回SOCKET_ERROR；如果len小于或者等于s的发送缓冲区的长度，那么send先检查协议是否正在发送s的发送缓冲中的数据，如果是就等待协议把数据发送完，如果协议还没有开始发送s的发送缓冲中的数据或者s的发送缓冲中没有数据，那么 send就比较s的发送缓冲区的剩余空间和len，如果len大于剩余空间大小send就一直等待协议把s的发送缓冲中的数据发送完，如果len小于剩余空间大小send就仅仅把buf中的数据copy到剩余空间里（注意并不是send把s的发送缓冲中的数据传到连接的另一端的，而是协议传的，send仅仅是把buf中的数据copy到s的发送缓冲区的剩余空间里）。如果send函数copy数据成功，就返回实际copy的字节数，如果send在copy数据时出现错误，那么send就返回SOCKET_ERROR；如果send在等待协议传送数据时网络断开的话，那么send函数也返回SOCKET_ERROR。

　　要注意send函数把buf中的数据成功copy到s的发送缓冲的剩余空间里后它就返回了，但是此时这些数据并不一定马上被传到连接的另一端。如果协议在后续的传送过程中出现网络错误的话，那么下一个Socket函数就会返回SOCKET_ERROR。（每一个除send外的Socket函数在执行的最开始总要先等待套接字的发送缓冲中的数据被协议传送完毕才能继续，如果在等待时出现网络错误，那么该Socket函数就返回 SOCKET_ERROR）

2.5.2recv函数学习

　　int recv( SOCKET s,     char FAR *buf,      int len,     int flags     );   

　不论是客户还是服务器应用程序都用recv函数从TCP连接的另一端接收数据。
　该函数的第一个参数指定接收端套接字描述符；
　第二个参数指明一个缓冲区，该缓冲区用来存放recv函数接收到的数据；
　第三个参数指明buf的长度；
　第四个参数一般置0。
　　这里只描述同步Socket的recv函数的执行流程。当应用程序调用recv函数时，recv先等待s的发送缓冲中的数据被协议传送完毕，如果协议在传送s的发送缓冲中的数据时出现网络错误，那么recv函数返回SOCKET_ERROR，如果s的发送缓冲中没有数据或者数据被协议成功发送完毕后，recv先检查套接字s的接收缓冲区，如果s接收缓冲区中没有数据或者协议正在接收数据，那么recv就一直等待，只到协议把数据接收完毕。当协议把数据接收完毕，recv函数就把s的接收缓冲中的数据copy到buf中（注意协议接收到的数据可能大于buf的长度，所以 在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的），recv函数返回其实际copy的字节数。如果recv在copy时出错，那么它返回SOCKET_ERROR；如果recv函数在等待协议接收数据时网络中断了，那么它返回0。

2.6、close()函数

　　在服务器与客户端建立连接之后，会进行一些读写操作，完成了读写操作就要关闭相应的socket描述字，好比操作完打开的文件要调用fclose关闭打开的文件。

#include <unistd.h>
int close(int fd);
　　close一个TCP socket的缺省行为时把该socket标记为以关闭，然后立即返回到调用进程。该描述字不能再由调用进程使用，也就是说不能再作为read或write的第一个参数。

注意：close操作只是使相应socket描述字的引用计数-1，只有当引用计数为0的时候，才会触发TCP客户端向服务器发送终止连接请求。

https://www.cnblogs.com/wmx-learn/p/5312259.html



传输层学习之六（SCTP）
https://blog.csdn.net/goodluckwhh/article/details/10648633

随着IP网络业务的发展，尤其是VoIP的发展，TCP出现了很多局限性，例如对于VoIP信令及异步基于事务应用的处理。因此，IEFT的信令传输工作组(SIGTRAN)提出了一种面向多媒体通信的流控制传输协议(SCTP)，用于在IP网络上传输PSTN信令消息，即通常所说的SS7overIP。它把SS7信令网络的一些可靠特性引入到了IP。SCTP是一种可靠的传输协议。SCTP提供的服务很像TCP，但同时又结合了UDP的一些优点。它提供了可靠、高效、有序的数据传输服务。相比之下TCP是面向字节的，而SCTP是针对成帧的消息（这点是类似于UDP的）。SCTP又是一个面向连接的传输协议，其连接被称为association即耦联，SCTP的“耦联”比TCP连接更为广泛：TCP的连接的每一端只有一个源地址和一个目的地址，而SCTP耦联的每一端能为另一端提供一组传输地址。
SCTP可以看做是TCP的增强协议，因为其可靠传输和拥塞控制机制基本都来自于TCP，但是它又对TCP进行了增强和完善，使得信令传输具有更高的可靠性。与TCP相比，SCTP最大的改变是增加了对多宿主（Multi-homing）、多流（Multi-streaming）以及部分有序（partialordering）的支持。一、SCTP报文格式
SCTP头部格式如图所示：
一个SCTP包含了一个公共的报文头（CommonHeader）和若干数据块（Chunk），每个数据块中既可以包含控制信息，也可以包含用户数据。除了INIT、INITACK和SHUTDOWNCOMPLETE数据块外，其他类型的多个数据块可以捆绑在一个SCTP报文中，以满足对MTU大小的要求。当然，这些数据块也可以不与其他数据块捆绑在一个报文中。如果一个用户消息不能放在一个SCTP报文中，这个消息可以被分成若干个数据块。
公共头的各个部分及其含义：源端口号和目地端口号：用于STCP的多路复用和多路分解，即标识发端和收端应用进程。验证标签：耦联建立时，本端为这个偶联生成一个随机标识。偶联建立过程中，双方会交换这个TAG，到了数据传递时，发送端必须在公共报文头中带上对端的这个TAG，以备校验。校验码：用于数据完整性校验。发送端产生，接收端验证。
数据块的各个部分及其含义：块类型：块类型定义在块值（ChunkValue）中消息所属的类型。如果接收端点不能识别块类型时，块类型最高位2bit用于标识需要进行的各种操作。此时最高两位含义如下：
停止处理并丢弃此SCTP报文，不再处理该SCTP报文中的其他消息块。停止处理并丢弃此SCTP报文，不再处理该SCTP报文中的其他消息块，并且在“ERROR”或“INITACK”中向发起端点返回不能识别的参数。跳过此数据块并继续执行。跳过此数据块并继续执行，并且在“ERROR”或“INITACK”中向发起端点返回不能识别的参数。数据块标志位：块标志位用法由块类型决定。块长度：块长度包括块类型（ChunkType）、块标记（ChunkFlags）、块长度（ChunkLength）和块值（ChunkValue），长度使用二进制表示。块值：该块包含的数据内容。二、STCP的多路复用和解复用
SCTP的多路复用和解复用采用了和TCP类似的机制，STCP也使用源IP+源端口+目地IP+目地端口的四元组来标识一个socket。但是需要注意的是SCTP支持mutihome，因而源IP和目地IP都不是唯一的，而允许是连接建立过程中通告的多个IP地址中的任意一个。
在STCP工作时，无论发送端还是接收端，都必须首先建立连接，在连接建立完成后，socket即拥有了它所需的四元组的信息，之后的收发都经过socket进行。三、SCTP的状态迁移图
SCTP的状态迁移图如下所示：
该状态机详细显示了SCTP的状态以及它们之间的迁移过程。四、SCTP的连接管理
1.建立连接
不同于TCP，SCTP通过四次握手来完成连接的建立：连接发起者（一般为客户端）SCTP发送一个INIT消息（初始化）。该消息包括了连接发起者的IP地址清单、初始序列号、用于标识本耦联中所有报文的起始标记、客户请求的外出流的数目以及客户能够支持的外来流的数目对端（服务器）发送一个INITACK消息确认连接发起者的INIT消息，其中含有服务器的IP地址清单、初始序列号、起始标记、服务器请求的外出流的数目、服务器能够支持的外来流的数目以及一个状态cookie，状态cookie包含服务器用于确信本耦联有效所需的所有状态，cookie是经过数字签名的，因而可以确保其有效性客户以一个COOKIEECHO消息返回服务器的状态cookie，除COOKIEECHO外，该消息可能在同一个报文中捆绑一个用户数据服务器以一个COOKIEACK消息确认客户返回的cookie是正确的，到此时该耦联就建立成功了。该消息也可能在同一个报文中捆绑一个用户数据。
其图示如下： 
与TCP的连接建立过程相比：SCTP经过四次握手才能建立一个耦联。INIT除了包括了一个类似于TCP的ISN的一个初始序列号之外，还包括了一个验证标记Ta。Ta是一个验证标记，此后对端所有返回的数据中都必须包含这个验证标记INITACK中的Tz验证标记类似于INIT消息中的Ta标记，连接发起者之后的所有数据都必须包含Tz这个验证标记。INIT接收端还在作为响应的INITACK中提供一个cookieC，包含设置本SCTP耦联所需的所有状态，用于防止“Dos”攻击。在一次SCTP四次握手中，INIT消息的接收端不必保存任何状态信息或者分配任何资源，这样就可防范SYNFlooding等DoS攻击。它在发送INIT-ACK消息时，采用了一种机制——“状态Cookie”，该Cookie具有发送端要建立自己状态所需的全部信息。
用于建立连接的INIT ACK只能在COOKIE WATI状态收到，在其它状态收到该报文时都会直接丢弃，类似的，COOKIE ACK只能在COOKIE ECHOED状态接收。
在常规的握手中，主动发起方的本地tag在发起握手时产生，主动发起方的对端tag在收到INIT ACK时产生。而连接的被动方的本地tag和对端tag都在收到INIT时产生，但是最终要到收到了COOKIE ECO后才确定并保存下来。
SCTP产生一个状态Cookie的过程如下：使用收到的INIT和发出的INIT-ACK块中的信息创建一个关联的TCB（传输控制块）。在TCB中，将当前日期设为创建日期，将协议参数“有效Cookie时间”设为生存期间。根据TCB，收集重建TCB所需的最小信息子集，将该子集和密钥产生一个MAC（信息认证编码）。结合上述最小信息子集和MAC产生状态Cookie。在发送完INITACK（包含状态Cookie参数）后，发送方必须删除TCB以及任何与新关联有关的本地资源。
INIT和INIT-ACK都必须包含建立初始状态所需的参数：一组IP地址，保证可靠传输的初始序列号，每个被接收的SCTP报文中必须含有的验证标签，每一端请求发出的流数目和每一端能支持接收的流数目。交换完这些消息之后，INIT的发送端以COOKIE-ECHO消息的方式发送回状态Cookie。接收端根据所接收到的COOKIE-ECHO中的状态Cookie，完整地重建自己的状态，并回送COOKIE-ACK来确认关联已建立。因此对于SCTP，即使接收再多的INIT消息,接收端也没有任何资源的消耗：它既不分配任何系统资源，也不保存此次新关联的状态，它只是把相应重建状态所用的状态Cookie作为参数，包含在每一个回送的INIT-ACK消息中，最后该状态Cookie会被COOKIE-ECHO消息发送回来。
类似于TCP，SCTP也多由客户端执行主动打开，而服务器执行被动打开。
2.终止连接
与TCP不同，SCTP使用三次握手来关闭一个耦联。而且SCTP不支持TCP所支持的“半关闭”状态。典型的SCTP关闭一个耦联的过程如下：应用程序发出关闭请求，SCTP耦联进入SHUTDOWN-PENDING状态，并且不再接收应用程序的数据，只发送队列中还未发送的数据，再队列中没有待发送数据后，发送SHUTWODN并进入SHUTDOWN-SENT状态。这一方被称为主动关闭。执行被动关闭的一方在接收到主动关闭一方的SHUTWODN消息时，进入SHUTDOWN-RECEIVED状态，此时执行被动关闭一方不再接受上层应用的数据，只发送队列中剩余的数据。在发送队列中的数据被发送完后，执行被动关闭一方发送SHUTDOWN-ACK并进入SHUTDOWN-ACK-SENT状态。执行主动关闭一方收到SHUTDOWN-ACK后就发送SHUTDOWN-COMPLETE，并进入CLOSE状态。执行主动关闭一端接收到SHUTDOWN-COMPLETE后就进入close状态。
其图示如下：
和TCP不同，由于SCTP的报文中带有验证标记，因而不会出现将具有相同的四元组上的旧的耦联的报文当做新耦联的报文的情形，因而SCTP关闭时不存在TIME_WAIT状态。3.同时打开
RFC规定，如果SCTP在COOKIE-WAIT或者COOKIE-ECHOED状态接收到INIT报文。则：INIT报文的接收者产生一个INIT-ACK，该INIT-ACK使用的本端参数和自己发送的那个INIT报文的相同执行状态COOKIE的计算过程，产生一个状态COOKIE不允许修改SCTP的状态状态COOKIE相关的TCB不能删除不关闭T1-init定时器
如果SCTP在非COOKIE-WAIT状态接收到了INIT-ACK，则丢弃它。
则同时打开可能如下图所示（是5.2.4的case D）：
 
根据INIT-ACK的相关处理规则，只要在COOKIE-WAIT状态收到了INIT-ACK，则SCTP就会发送COOKIE-ECHO并进入COOKIE-ECHOED状态。因而在上图的“同时打开”场景下，SCTP会走到图中所示的状态，之后的处理则会有所不同。在这种场景中，收到COOKIE-ECHO后会从其中恢复TCB信息，并将其和在发送INIT时产生的那个没有被删除的TCB的信息进行比较，如果本端和对端的tag信息都相同（在这种场景中，这是我们期望的），则会直接进入ESTABLISHED状态，其它情形的处理可参考RFC 2960的5.2.4一节，该节给出了详细的处理规则。
更复杂一点的同时打开可能如下所示（图画的不是特别好，在ubuntu下画的。5.2.4的case A很简单，当连接建立后一端down掉，然后用一致的参数尝试建立连接就是该情形。）：

4.同时关闭
极少数情况下，耦联的双发可能同时执行主动关闭，即同时进入发送SHUTWODN并进入SHUTDOWN-SENT状态。在这种情况下关闭的流程为：两端都发送SHUTWODN并进入SHUTDOWN-SENT状态两端都收到对方的SHUTDOWN消息，并发送SHUTDOWN-ACK，然后进入SHUTDOWN-ACK-SENT状态两端都收到对方的SHUTDOWN-ACK，并发送SHUTDOWN-COMPLETE，然后就进入close状态五、和TCP的区别
SCTP可以看做是对TCP进行了增强的传输层协议，和TCP的不同包括：
支持多宿主:SCTP的每一端都可以提供多个IP地址，对于对端提供的每个IP地址，SCTP都会通过HEARD_BEAT机制来检测其可达性，当其中一个IP地址由于网络故障而不可达时，SCTP会将报文送到一个可达的地址。通过这种方式，SCTP提高了可靠性，可以容忍网络级错误。支持多种传输模式:除了支持TCP的严格有序传输完，SCTP还支持部分有序传输和无序传输（像UDP）。支持多流:TCP以字节流的方式工作，通过TCP连接传输的数据以字节流的方式在TCP连接的两端之间流动。但是在SCTP中，流用来指示需要按顺序递交到上层协议的用户消息的序列，在同一个流中的消息需要按照其顺序进行递交。严格地 说，“流”就是一个SCTP 偶联中，从一个端点到另一个端点的单向逻辑通道。一 个偶联是由多个单向的流组成的。各个流之间相对独立，使用流ID 进行标识，每个 流可以单独发送数据而不受其他流的影响。路径管理:SCTP 路径管理功能主要负责从远端提供的一组传输地址中选择目的传输地址，它根据SCTP用户的指令和当前可达的目的地来选择目的地址。在SCTP建立耦联时，会向对端通告本端可用的所有地址，并且会获取对端的所有可用地址。随后SCTP会通过HEART_BEAT定时检测对端所有地址的可达性。选择性确认（SACK）:在TCP中其确认机制的基础是累积确认，SACK是选项，不一定启用，但是SCTP中是必须使用的。SCTP使用SACK反馈给发送端的是丢失的并且要求重传的消息序号。防范拒绝服务（DoS）攻击:SYN Flooding攻击是DoS的一种方式，TCP的实际实现中通过扩展实现了对它的防御，但是在SCTP协议中已经增加了对它的防御。方法是在耦联初始化阶段实施一种安全的“Cookie”机制。根据已发现的路径MTU（最大传输单元）大小进行用户数据分片: TCP根据MSS确定发送给对端的最大字节长度，MSS可能来自于路径MTU发现也可能不是来自于路径MTU发现。但是对于SCTP，为了确保发送到下层的SCTP数据包与路径MTU一致进而避免分片，SCTP对用户消息分片。在接收端，分片被重组后传给上层SCTP用户。验证标签:SCTP 报文的公共分组头包含一个验证标签（VerificATIon Tag）。 验证标签的值由偶联两端在偶联启动时选择。如果收到的分组中如果没有期望的验证标签值，接收端将丢弃这个分组，以阻止攻击和失效的SCTP 分组。这个特性也使得SCTP可以区分SCTP报文段是属于本耦联的还是本耦联所用的四元组的上一个实例的。消息块绑定:类似于TCP当用户数据很短时，数据传输的效率会很低。为了应对该问题，SCTP会将几个用户数据绑定在一个SCTP 报文里面传输，以提高带宽的利用率。用户可以选择是否启用该功能，但是当出现拥塞/重传时即便用户选择不使用该功能，该功能也仍会被使用。相比之下TCP需要打开TCP_CORK 选项的支持才能获得类似的功能。

---------------------

本文来自 goodluckwhh 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/goodluckwhh/article/details/10648633?utm_source=copy 