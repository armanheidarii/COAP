#include <iostream>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <sys/types.h>
#include <string>
#include <vector>

class Socket 
{
public:
    Socket(const std::string& hostname, const int port);

    int connect();

    std::vector<char> receive();

    void send(const std::string& msg);

    void send(const std::vector<char>& msg);

    void close();

private:
    const std::string hostname;
    const int port;
    
    int sockfd;
};