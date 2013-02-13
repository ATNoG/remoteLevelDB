#ifndef _ACTION_H_
#define _ACTION_H_

#include "Message.hpp"

#include <leveldb/db.h>

#include <string>

class Action
{
    public:
        Action(const std::string& type);
        Action(leveldb::DB *db, const std::string& type);
        Action(const Action& action);

        std::string type() const;
        virtual Message* execute()=0;
        virtual Action* clone(leveldb::DB *db, const Message& msg)const=0;

        virtual ~Action();
    protected:
        std::string *vtype;
        leveldb::DB *vdb;
};

class ActionSet: public Action
{
    public:
        ActionSet();
        ActionSet(leveldb::DB *db, const std::string &key, const std::string &value);
        ActionSet(const ActionSet& action);

        Message* execute();
        Action* clone(leveldb::DB *db, const Message& msg) const;

        virtual ~ActionSet();
    private:
        std::string *vkey;
        std::string *vvalue;
}; 

class ActionGet: public Action
{
    public:
        ActionGet();
        ActionGet(leveldb::DB *db, const std::string& key);
        ActionGet(const ActionGet& action);

        Message* execute();
        Action* clone(leveldb::DB *db, const Message& msg)const;

        virtual ~ActionGet();
    private:
        std::string *vkey;
};

class ActionGetAll: public Action
{
    public:
        ActionGetAll();
        ActionGetAll(leveldb::DB *db, const std::vector<std::string>& keys);
        ActionGetAll(const ActionGetAll& action);

        Message* execute();
        Action* clone(leveldb::DB *db, const Message& msg)const;

        virtual ~ActionGetAll();
    private:
        std::vector<std::string> *vkeys;
};

class ActionDelete: public Action
{
    public:
        ActionDelete();
        ActionDelete(leveldb::DB *db, const std::string& key);
        ActionDelete(const ActionDelete& action);

        Message* execute();
        Action* clone(leveldb::DB *db, const Message& msg)const;

        virtual ~ActionDelete();
    private:
        std::string *vkey;
};

class ActionClean: public Action
{
    public:
        ActionClean();
        ActionClean(leveldb::DB *db);
        ActionClean(const ActionClean& action);

        Message* execute();
        Action* clone(leveldb::DB *db, const Message& msg)const;
};

class ActionFactory
{
    public:
        ActionFactory(leveldb::DB *db);

        Action* parse(const Message &msg);

        ~ActionFactory();
    
    private:
        std::map<std::string, Action*> actions;
        leveldb::DB *vdb;
};

#endif
