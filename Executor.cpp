#include "Executor.hpp"

#include <zmq.h>

Executor::Executor(MessageFactory *msgFactory, ActionFactory *actionFactory, void *context)
{
    vstop = false;
    vrunning = false;
    vmsgFactory = msgFactory;
    vactionFactory = actionFactory;
    vcontext = context;
}

void Executor::start()
{
    if(vrunning == false)
    {
        vrunning = true;
        vthread = new boost::thread(&Executor::work, this);
    }
}

void Executor::stop()
{
    if(vrunning == true)
    {
        vstop = true;
        vthread->join();
        vrunning = false;
    }
}

Executor::~Executor()
{
    delete vthread;
}

void Executor::work()
{
    void *socket = zmq_socket(vcontext, ZMQ_REP);
    zmq_msg_t zMsg;
    char *cstring = NULL;
    size_t msgSize = 0;
    
    zmq_connect(socket, "inproc://storageWorkers");
    
    while(vstop==false)
    {
        zmq_msg_init(&zMsg);
        zmq_msg_recv(&zMsg, socket, 0);
        msgSize = zmq_msg_size(&zMsg);
        cstring = new char[msgSize+1];
        memcpy(cstring, zmq_msg_data(&zMsg), msgSize);
        zmq_msg_close(&zMsg);
        cstring[msgSize] = '\0';
        std::string str(cstring);
        delete cstring;

        Message* msg = vmsgFactory->parse(str);

        Action *action = vactionFactory->parse(*msg);
        delete msg;

        msg = action->execute();
        delete action;

        msgSize = msg->toString().size()+1;
        zmq_msg_init_size(&zMsg, msgSize);
        memcpy(zmq_msg_data(&zMsg), msg->toString().c_str(), msgSize);
        delete msg;
        zmq_msg_send(&zMsg, socket, 0);
        zmq_msg_close(&zMsg);
    }
    zmq_close(socket);
}    
