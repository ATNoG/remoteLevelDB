#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "JSONObject.hpp"

#include <string>
#include <vector>
#include <map>

class Message
{
    public:
        Message(const std::string& type);
        Message(const json::JSONObject& json);
        Message(const Message& message);

        virtual json::JSONObject toJSON() const;
        std::string toString() const;
        std::string type() const;
        virtual Message* clone(const json::JSONObject& json) const;

        bool operator==(const Message &other) const;

        virtual ~Message();
    private:
        std::string *vtype;
};

std::ostream& operator<<(std::ostream& out, const Message& message);

class MessageReply: public Message
{
    public:
        MessageReply(const std::string& type);
        MessageReply(const std::string& type, bool success);
        MessageReply(const json::JSONObject& json);
        MessageReply(const MessageReply& message);

        virtual json::JSONObject toJSON() const;
        bool success() const;

        virtual ~MessageReply();
    private:
        bool vsuccess;
};

class MessageGetRequest: public Message
{
    public:
        MessageGetRequest();
        MessageGetRequest(const std::string& key);
        MessageGetRequest(const json::JSONObject& json);

        std::string key() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageGetRequest();
    private:
        std::string *vkey;

};

class MessageGetReply: public MessageReply
{
    public:
        MessageGetReply();
        MessageGetReply(const std::string& value);
        MessageGetReply(const json::JSONObject& json);

        std::string value() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageGetReply();
    private:
        std::string *vvalue;

};

class MessageSetRequest: public Message
{
    public:
        MessageSetRequest();
        MessageSetRequest(const std::string& key, const std::string& value);
        MessageSetRequest(const json::JSONObject& json);
        MessageSetRequest(const MessageSetRequest &message);

        std::string key() const;
        std::string value() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageSetRequest();
    private:
        std::string *vkey, *vvalue;

};

class MessageSetReply: public MessageReply
{
    public:
        MessageSetReply();
        MessageSetReply(const bool success);
        MessageSetReply(const json::JSONObject& json);
        MessageSetReply(const MessageSetReply &message);

        Message* clone(const json::JSONObject& json) const;
};

class MessageDeleteRequest: public Message
{
    public:
        MessageDeleteRequest();
        MessageDeleteRequest(const std::string& key);
        MessageDeleteRequest(const json::JSONObject& json);
        MessageDeleteRequest(const MessageDeleteRequest &message);

        std::string key() const;
        std::string value() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageDeleteRequest();
    private:
        std::string *vkey;

};

class MessageDeleteReply: public MessageReply
{
    public:
        MessageDeleteReply();
        MessageDeleteReply(const bool success);
        MessageDeleteReply(const json::JSONObject& json);
        MessageDeleteReply(const MessageDeleteReply &message);

        Message* clone(const json::JSONObject& json) const;
};

class MessageGetAllRequest: public Message
{
    public:
        MessageGetAllRequest();
        MessageGetAllRequest(const std::vector<std::string> keys);
        MessageGetAllRequest(const json::JSONObject& json);
        MessageGetAllRequest(const MessageGetAllRequest& message);

        std::vector<std::string> keys() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageGetAllRequest();
    private:
        std::vector<std::string> *vkeys;
};

class MessageGetAllReply: public MessageReply
{
    public:
        MessageGetAllReply();
        MessageGetAllReply(const std::vector<std::string> keys, const std::vector<std::string> values);
        MessageGetAllReply(const json::JSONObject& json);
        MessageGetAllReply(const MessageGetAllReply& message);

        
        std::vector<std::string> keys() const;
        std::vector<std::string> values() const;
        json::JSONObject toJSON() const;
        Message* clone(const json::JSONObject& json) const;

        ~MessageGetAllReply();
    private:
        std::vector<std::string> *vvalues, *vkeys;
};

class MessageCleanRequest: public Message
{
    public:
        MessageCleanRequest();
        MessageCleanRequest(const json::JSONObject& json);
        MessageCleanRequest(const MessageCleanRequest &message);

        Message* clone(const json::JSONObject& json) const;
};

class MessageCleanReply: public MessageReply
{
    public:
        MessageCleanReply();
        MessageCleanReply(const bool success);
        MessageCleanReply(const json::JSONObject& json);
        MessageCleanReply(const MessageCleanReply &message);

        Message* clone(const json::JSONObject& json) const;
};

class MessageFactory
{
    public:
        MessageFactory();

        Message* parse(const std::string& text);

        ~MessageFactory();
    
    private:
        std::map<std::string, Message*> msgs;
};

#endif
