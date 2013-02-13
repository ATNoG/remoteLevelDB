#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include "Action.hpp"
#include "Message.hpp"

#include <boost/thread/thread.hpp>

class Executor
{
    public:
        Executor(MessageFactory *msgFactory, ActionFactory *actionFactory, void *context);
        void start();
        void stop();
        ~Executor();

    private:
        volatile bool vstop, vrunning;
        boost::thread *vthread;
        MessageFactory *vmsgFactory;
        ActionFactory *vactionFactory;
        void *vcontext;
        
        void work();
};

#endif
