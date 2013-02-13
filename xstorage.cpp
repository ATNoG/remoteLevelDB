#include "JSONObject.hpp"
#include "Message.hpp"
#include "Action.hpp"
#include "Executor.hpp"

#include <zmq.h>
#include <leveldb/db.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <csignal>

#define CONFIG "config.json"

static void *context = NULL;
static leveldb::DB* db;
static size_t nExecutors = 0;
static Executor** executors = NULL;

static void s_signal_handler(int signal_value)
{
    delete db;
    exit(0);
}

static void s_catch_signals(void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

int main(int argc, char** argv)
{
    s_catch_signals();
    std::string configPath(CONFIG);

    if(argc > 1)
    {
        configPath = std::string(argv[1]);
    }

    std::ifstream file;
    std::stringstream ss;

    file.open(configPath.c_str());
    if (file.is_open())
    {
        ss << file.rdbuf();   
        file.close();
    }
    else
    {
        exit(1);
    }

    json::JSONObject config(ss);

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::DB::Open(options, config.get("StorageDirectory"), &db);

    context = zmq_init(1);
    void *frontend = zmq_socket(context, ZMQ_ROUTER),
         *backend  = zmq_socket(context, ZMQ_DEALER);
    zmq_bind(frontend, config.get("StorageAdd").c_str());
    zmq_bind(backend, "inproc://storageWorkers");

    MessageFactory *msgFactory = new MessageFactory();
    ActionFactory *actionsFactory = new ActionFactory(db);

    nExecutors = config.getInt("StorageNWorkers");
    executors = new Executor*[nExecutors];

    for(size_t i = 0; i < nExecutors; i++)
    {
        executors[i] = new Executor(msgFactory, actionsFactory, context);
        executors[i]->start();
    }    
    zmq_device(ZMQ_QUEUE, frontend, backend);

    for(size_t i = 0; i < nExecutors; i++)
    {
        delete executors[i];
    }

    delete actionsFactory;
    delete msgFactory;
    delete executors;
    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}
