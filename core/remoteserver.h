#ifndef REMOTESERVER_H
#define REMOTESERVER_H
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

class RemoteServer : public boost::noncopyable
{
public:
    RemoteServer();

    static RemoteServer &instance()
    {
        static auto inst = std::unique_ptr<RemoteServer>( new RemoteServer() );
        return *inst;
    }


};

#endif // REMOTESERVER_H
