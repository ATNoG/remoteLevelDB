#include "Message.hpp"

#define GET_REQUEST "get request"
#define GET_REPLY "get reply"
#define SET_REQUEST "set request"
#define SET_REPLY "set reply"
#define GET_ALL_REQUEST "getall request"
#define GET_ALL_REPLY   "getall reply"
#define DELETE_REQUEST "delete request"
#define DELETE_REPLY "delete reply"
#define CLEAN_REQUEST "clean request"
#define CLEAN_REPLY "clean reply"

Message::Message(const std::string& type)
{
    vtype = new std::string(type);
}

Message::Message(const json::JSONObject& json)
{
    vtype = new std::string(json.get("type"));
}

Message::Message(const Message& message)
{
    vtype = new std::string(message.type());
}

json::JSONObject Message::toJSON() const
{
    json::JSONObject json;
    json.put("type", *(this->vtype));
    return json;
}

std::string Message::toString() const
{
    std::ostringstream stream;
    stream<<this->toJSON();
    std::string rv = stream.str();

    return rv;
}

std::string Message::type() const
{
    std::string rv(*(this->vtype));

    return rv;
}

Message* Message::clone(const json::JSONObject& json) const
{
    std::cerr<<"MESSAGE clone"<<std::endl;
    Message* msg = new Message(json);

    return msg;
}

bool Message::operator==(const Message &other) const
{
    bool rv = false;

    if(*vtype==other.type())
    {
        rv = true;
    }

    return rv;
}

Message::~Message()
{
    delete vtype;
}

std::ostream& operator<<(std::ostream& out, const Message& message)
{   
    return out<<message.toString();
}

/***
 *  Class: Message Reply
 *
 */
MessageReply::MessageReply(const std::string& type): Message(type)
{
    vsuccess = false;
}

MessageReply::MessageReply(const std::string& type, bool success): Message(type)
{
    vsuccess = success;
}

MessageReply::MessageReply(const json::JSONObject& json): Message(json)
{
    vsuccess = json.getBoolean("success");
}

MessageReply::MessageReply(const MessageReply& message): Message(message)
{
    vsuccess = message.success();
}

json::JSONObject MessageReply::toJSON() const
{
    json::JSONObject json = Message::toJSON();
    json.put("success", vsuccess);
    return json;
}
bool MessageReply::success() const
{
    return vsuccess;
}

MessageReply::~MessageReply()
{ 
}

/***
 * Class: Message Get Request
 *
 */
MessageGetRequest::MessageGetRequest():Message(GET_REQUEST)
{
    vkey=NULL;
}

MessageGetRequest::MessageGetRequest(const std::string& key):Message(GET_REQUEST)
{
    vkey = new std::string(key);
}

MessageGetRequest::MessageGetRequest(const json::JSONObject& json):Message(GET_REQUEST)
{
    vkey = new std::string(json.get("key"));
}

std::string MessageGetRequest::key() const
{
    std::string rv(*vkey);

    return rv;
}

json::JSONObject MessageGetRequest::toJSON() const
{
    json::JSONObject json = Message::toJSON();
    json.put("key", *vkey);
    return json;
}

Message* MessageGetRequest::clone(const json::JSONObject& json) const
{
    MessageGetRequest* msg = new MessageGetRequest(json);
    return msg;
}

MessageGetRequest::~MessageGetRequest()
{
    if(vkey != NULL)
    {
        delete vkey;
    }
} 

MessageGetReply::MessageGetReply():MessageReply(GET_REPLY)
{
    vvalue=NULL;
}

MessageGetReply::MessageGetReply(const std::string& value):MessageReply(GET_REPLY, true)
{
    vvalue = new std::string(value);
}

MessageGetReply::MessageGetReply(const json::JSONObject& json):MessageReply(json)
{
    vvalue = new std::string(json.get("value"));
}

std::string MessageGetReply::value() const
{
    std::string rv(*vvalue);
    return rv;
}

json::JSONObject MessageGetReply::toJSON() const
{
    json::JSONObject json = MessageReply::toJSON();
    json.put("value", *vvalue);
    return json;
}

Message* MessageGetReply::clone(const json::JSONObject& json) const
{
    MessageGetReply* msg = new MessageGetReply(json);
    return msg;
}

MessageGetReply::~MessageGetReply()
{
    if(vvalue != NULL)
    {
        delete vvalue;
    }
}

/***
 * Class: Message Set Request
 *
 */
MessageSetRequest::MessageSetRequest():Message(SET_REQUEST)
{
    vkey=NULL;
    vvalue=NULL;
}

MessageSetRequest::MessageSetRequest(const std::string& key, const std::string& value):Message(SET_REQUEST)
{
    vkey = new std::string(key);
    vvalue = new std::string(value);
}

MessageSetRequest::MessageSetRequest(const json::JSONObject& json):Message(SET_REQUEST)
{
    vkey = new std::string(json.get("key"));
    vvalue = new std::string(json.get("value"));
}

MessageSetRequest::MessageSetRequest(const MessageSetRequest &message):Message(SET_REQUEST)
{
    vkey = new std::string(message.key());
    vvalue = new std::string(message.value());
}

std::string MessageSetRequest::key() const
{
    std::string rv(*vkey);
    return rv;
}

std::string MessageSetRequest::value() const
{
    std::string rv(*vvalue);
    return rv;
}

json::JSONObject MessageSetRequest::toJSON() const
{
    json::JSONObject json = Message::toJSON();
    json.put("key", *vkey);
    json.put("value", *vvalue);
    return json;
}

Message* MessageSetRequest::clone(const json::JSONObject& json) const
{
    MessageSetRequest* msg = new MessageSetRequest(json);
    return msg;
}

MessageSetRequest::~MessageSetRequest()
{
    if(vkey != NULL)
    {
        delete vkey;
    }

    if(vvalue != NULL)
    {
        delete vvalue;
    }
} 

MessageSetReply::MessageSetReply():MessageReply(SET_REPLY)
{
}

MessageSetReply::MessageSetReply(const bool success):MessageReply(SET_REPLY, success)
{
}

MessageSetReply::MessageSetReply(const json::JSONObject& json):MessageReply(json)
{
}

MessageSetReply::MessageSetReply(const MessageSetReply &message):MessageReply(message.type(), message.success())
{
}

Message* MessageSetReply::clone(const json::JSONObject& json) const
{
    MessageSetReply* msg = new MessageSetReply(json);

    return msg;
}

/***
 *  Class: Message Get All Request
 *
 */
MessageGetAllRequest::MessageGetAllRequest():Message(GET_ALL_REQUEST)
{
    vkeys=NULL;
}

MessageGetAllRequest::MessageGetAllRequest(const std::vector<std::string> keys):Message(GET_ALL_REQUEST)
{
    vkeys = new std::vector<std::string>(keys);
}

MessageGetAllRequest::MessageGetAllRequest(const json::JSONObject& json):Message(GET_ALL_REQUEST)
{
    vkeys = new std::vector<std::string>(json.getArray("keys"));
}

MessageGetAllRequest::MessageGetAllRequest(const MessageGetAllRequest& message):Message(GET_ALL_REQUEST)
{
    vkeys = new std::vector<std::string>(*message.vkeys);
}

std::vector<std::string> MessageGetAllRequest::keys() const
{
    std::vector<std::string> rv(*vkeys);
    return rv;
}

json::JSONObject MessageGetAllRequest::toJSON() const
{
    json::JSONObject json = Message::toJSON();
    json.put("keys", *vkeys);
    return json;
}

Message* MessageGetAllRequest::clone(const json::JSONObject& json) const
{
    MessageGetAllRequest* msg = new MessageGetAllRequest(json);
    return msg;
}

MessageGetAllRequest::~MessageGetAllRequest()
{
    if(vkeys != NULL)
    {
        delete vkeys;
    }
}

/***
 *  Class: Message Get All Reply
 *
 */
MessageGetAllReply::MessageGetAllReply():MessageReply(GET_ALL_REPLY)
{
    vvalues=NULL;
}

MessageGetAllReply::MessageGetAllReply(const std::vector<std::string> keys, const std::vector<std::string> values):MessageReply(GET_ALL_REPLY, true)
{
    vkeys = new std::vector<std::string>(keys);
    vvalues = new std::vector<std::string>(values);
}

MessageGetAllReply::MessageGetAllReply(const json::JSONObject& json):MessageReply(GET_ALL_REPLY, true)
{
    vkeys = new std::vector<std::string>(json.getArray("keys"));
    vvalues = new std::vector<std::string>(json.getArray("values"));
}

MessageGetAllReply::MessageGetAllReply(const MessageGetAllReply& message):MessageReply(GET_ALL_REPLY, true)
{
    vkeys = new std::vector<std::string>(*message.vkeys);
    vvalues = new std::vector<std::string>(*message.vvalues);
}

std::vector<std::string> MessageGetAllReply::values() const
{
    std::vector<std::string> rv(*vvalues);
    return rv;
}

std::vector<std::string> MessageGetAllReply::keys() const
{
    std::vector<std::string> rv(*vkeys);
    return rv;
}

json::JSONObject MessageGetAllReply::toJSON() const
{
    json::JSONObject json = MessageReply::toJSON();
    json.put("keys", *vkeys);
    json.put("values", *vvalues);
    return json;
}

Message* MessageGetAllReply::clone(const json::JSONObject& json) const
{
    MessageGetAllReply* msg = new MessageGetAllReply(json);
    return msg;
}

MessageGetAllReply::~MessageGetAllReply()
{
    if(vvalues != NULL)
    {
        delete vvalues;
    }

    if(vkeys != NULL)
    {
        delete vkeys;
    }
}

/***
 * Class: Message Delete Request
 *
 */
MessageDeleteRequest::MessageDeleteRequest():Message(DELETE_REQUEST)
{
    vkey=NULL;
}

MessageDeleteRequest::MessageDeleteRequest(const std::string& key):Message(DELETE_REQUEST)
{
    vkey = new std::string(key);
}

MessageDeleteRequest::MessageDeleteRequest(const json::JSONObject& json):Message(DELETE_REQUEST)
{
    vkey = new std::string(json.get("key"));
}

MessageDeleteRequest::MessageDeleteRequest(const MessageDeleteRequest &message):Message(DELETE_REQUEST)
{
    vkey = new std::string(message.key());
}

std::string MessageDeleteRequest::key() const
{
    std::string rv(*vkey);
    return rv;
}

json::JSONObject MessageDeleteRequest::toJSON() const
{
    json::JSONObject json = Message::toJSON();
    json.put("key", *vkey);
    return json;
}

Message* MessageDeleteRequest::clone(const json::JSONObject& json) const
{
    MessageDeleteRequest* msg = new MessageDeleteRequest(json);
    return msg;
}

MessageDeleteRequest::~MessageDeleteRequest()
{
    if(vkey != NULL)
    {
        delete vkey;
    }
} 

/***
 * Class: Message Delete Reply
 *
 */
MessageDeleteReply::MessageDeleteReply():MessageReply(DELETE_REPLY)
{
}

MessageDeleteReply::MessageDeleteReply(const bool success):MessageReply(DELETE_REPLY, success)
{
}

MessageDeleteReply::MessageDeleteReply(const json::JSONObject& json):MessageReply(json)
{
}

MessageDeleteReply::MessageDeleteReply(const MessageDeleteReply &message):MessageReply(message.type(), message.success())
{
}

Message* MessageDeleteReply::clone(const json::JSONObject& json) const
{
    MessageDeleteReply* msg = new MessageDeleteReply(json);

    return msg;
}

/***
 * Class: Message Clean Request
 *
 */
MessageCleanRequest::MessageCleanRequest():Message(CLEAN_REQUEST)
{
}

MessageCleanRequest::MessageCleanRequest(const json::JSONObject& json):Message(json)
{
}

MessageCleanRequest::MessageCleanRequest(const MessageCleanRequest &message):Message(message)
{
}

Message* MessageCleanRequest::clone(const json::JSONObject& json) const
{
    MessageCleanRequest* msg = new MessageCleanRequest(json);
    return msg;
}

/***
 * Class: Message Clean Reply
 *
 */
MessageCleanReply::MessageCleanReply():MessageReply(CLEAN_REPLY)
{
}

MessageCleanReply::MessageCleanReply(const bool success):MessageReply(CLEAN_REPLY, success)
{
}

MessageCleanReply::MessageCleanReply(const json::JSONObject& json):MessageReply(json)
{
}

MessageCleanReply::MessageCleanReply(const MessageCleanReply &message):MessageReply(message)
{
}

Message* MessageCleanReply::clone(const json::JSONObject& json) const
{
    MessageCleanReply* msg = new MessageCleanReply(json);
    return msg;
}

/***
 * Class: Message Factory
 *
 */
MessageFactory::MessageFactory()
{
    Message* msg = new MessageGetRequest();
    msgs[msg->type()]= msg;
    
    msg = new MessageGetReply();
    msgs[msg->type()] = msg;

    msg = new MessageGetAllRequest();
    msgs[msg->type()] = msg;

    msg = new MessageGetAllReply();
    msgs[msg->type()] = msg;

    msg = new MessageSetRequest();
    msgs[msg->type()] = msg;

    msg = new MessageSetReply();
    msgs[msg->type()] = msg;

    msg = new MessageDeleteRequest();
    msgs[msg->type()] = msg;

    msg = new MessageDeleteReply();
    msgs[msg->type()] = msg;

    msg = new MessageCleanRequest();
    msgs[msg->type()] = msg;

    msg = new MessageCleanReply();
    msgs[msg->type()] = msg;
}

Message* MessageFactory::parse(const std::string& text)
{
    Message *rv = NULL;
    json::JSONObject json(text);

    rv = msgs[json.get("type")]->clone(json);
    
    return rv;
}

MessageFactory::~MessageFactory()
{
    std::map<std::string, Message*>::iterator it;

    Message *msg = NULL;

    for(it=msgs.begin(); it != msgs.end(); it++)
    {
        msg = (*it).second;
        delete msg;
    }

    msgs.clear();
}
