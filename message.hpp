#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <string>
#include <cstring>

/*
           BODY
______________________________
|         |                   |
| header  |     message       |
|_________|___________________|

*/

class Message {
public:
    static constexpr int messageSize = 512; // max value of the message
    static constexpr int headerSize = 4;    // the starting 4 bytes represent size of actual message

    Message() : bodyLength(0) {}

    Message(const std::string& message) {
        if (message.size() > messageSize) {
            bodyLength = messageSize;
        } else {
            bodyLength = message.size();
        }

        encodeHeader(); // this initializes the header, by giving value to first 4 bytes
        std::memcpy(data + headerSize, message.c_str(), bodyLength); // this adds the remaining data after first 4 bytes
    }

    void encodeHeader() {
        char new_header[headerSize + 1] = "";
        snprintf(new_header, sizeof(new_header), "%4d", static_cast<int>(bodyLength));
        std::memcpy(data, new_header, headerSize);
    }

    bool decodeHeader() {
        char new_header[headerSize + 1] = "";
        std::strncpy(new_header, data, headerSize);
        new_header[headerSize] = '\0';

        int headerValue = std::atoi(new_header);
        if (headerValue < 0 || headerValue > messageSize) {
            bodyLength = 0;
            return false;
        }
        bodyLength = headerValue;
        return true;
    }

    void printMessage() {
        std::cout << "Message received: " << getBody() << std::endl;
    }

    std::string getData() const {
        int length = headerSize + bodyLength;
        return std::string(data, length);
    }

    std::string getBody() const {
        return std::string(data + headerSize, bodyLength);
    }

    size_t getBodyLength() const {
        return bodyLength;
    }

private:
    char data[headerSize + messageSize]; // character array to store header and actual message
    size_t bodyLength;                   // size of actual messgae
};

#endif 