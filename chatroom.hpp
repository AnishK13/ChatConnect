#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include <iostream>
#include <deque>
#include <set>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <boost/asio.hpp>
#include "message.hpp"

using boost::asio::ip::tcp;

class Participant {
public: 
    virtual void deliver(Message& message) = 0; // virtual polymorphism
};

typedef std::shared_ptr<Participant> ParticipantPointer;

class Room {
public:
    void join(ParticipantPointer participant);
    void leave(ParticipantPointer participant);
    void deliver(Message& message);
private:
    std::deque<Message> messageQueue;
    enum { maxParticipants = 100 };
    std::set<ParticipantPointer> participants;
};

class Session : public Participant, public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket s, Room& room);
    void start();
    void deliver(Message& message) override;
    char* readHeader();
    char* readBody();
    void write();
private:
    tcp::socket clientSocket;
    Room& room;
    Message message;  
    std::deque<Message> messageQueue;
};

#endif 
