#include "Action.hpp"

#include <iostream>

#define TYPE_SET     "set request"
#define TYPE_GET     "get request"
#define TYPE_GET_ALL "getall request"
#define TYPE_DELETE  "delete request"
#define TYPE_CLEAN   "clean request"

/***
 * Class: Action
 *
 */
Action::Action(const std::string& type)
{
    vtype = new std::string(type);
    vdb = NULL;
}

Action::Action(leveldb::DB *db, const std::string& type)
{
    vtype = new std::string(type);
    vdb = db;
}

Action::Action(const Action& action)
{
    vtype = new std::string(*(action.vtype));
    vdb = action.vdb;
}

std::string Action::type() const
{
    std::string rv(*vtype);
    return rv;
}

Action::~Action()
{
    delete vtype;
}

/***
 * Class: Action Set
 *
 */
ActionSet::ActionSet():Action(TYPE_SET)
{
	vkey = NULL;
	vvalue = NULL;
}

ActionSet::ActionSet(leveldb::DB *db, const std::string &key, const std::string &value):Action(db, TYPE_SET)
{
    vvalue = new std::string(value);
    vkey = new std::string(key);

}

ActionSet::ActionSet(const ActionSet &action):Action(action)
{
    vvalue = new std::string(*action.vvalue);
    vkey = new std::string(*action.vkey);
}

Message* ActionSet::execute()
{
    Message *rv = NULL;
    
    leveldb::Status s = vdb->Put(leveldb::WriteOptions(), *vkey, *vvalue);
    
    if(s.ok())
    {
        rv = new MessageSetReply(true);
    }
    else
    {
        rv = new MessageSetReply(false);
    }
    
    return rv;
}

Action* ActionSet::clone(leveldb::DB *db, const Message& msg) const
{
    Action *rv = new ActionSet(db, ((MessageSetRequest&)msg).key(), ((MessageSetRequest&)msg).value());
    return rv;
}

ActionSet::~ActionSet()
{
    delete vvalue;
    delete vkey;
}

/***
 * Class: Action Get
 *
 */
ActionGet::ActionGet():Action(TYPE_GET)
{
	vkey = NULL;
}

ActionGet::ActionGet(leveldb::DB *db, const std::string& key):Action(db, TYPE_GET)
{
    vkey = new std::string(key);
}

ActionGet::ActionGet(const ActionGet &action):Action(action.vdb, *action.vtype)
{
    vkey = new std::string(*action.vkey);
}

Message* ActionGet::execute()
{
    Message *rv = NULL;
    std::string value;
    vdb->Get(leveldb::ReadOptions(), *vkey, &value);
    rv = new MessageGetReply(value);
    return rv;
}

Action* ActionGet::clone(leveldb::DB *db, const Message& msg) const
{
    Action *rv = new ActionGet(db, ((MessageGetRequest&)msg).key());
    return rv;
}

ActionGet::~ActionGet()
{
    delete vkey;
}

/***
 * Class Action Get All
 *
 */
ActionGetAll::ActionGetAll():Action(TYPE_GET_ALL)
{
	vkeys = NULL;
}

ActionGetAll::ActionGetAll(leveldb::DB *db, const std::vector<std::string>& keys):Action(db, TYPE_GET_ALL)
{
    vkeys = new std::vector<std::string>(keys);
}

ActionGetAll::ActionGetAll(const ActionGetAll &action):Action(action)
{
    vkeys = new std::vector<std::string>(*action.vkeys);
}

Message* ActionGetAll::execute()
{
    Message *rv = NULL;
    std::vector<std::string> values;
    std::vector<std::string> keys;
    std::string value;
    leveldb::Status status;

    std::sort(vkeys->begin(), vkeys->end());
    for(size_t i = 0; i < vkeys->size(); i++)
    {
        status = vdb->Get(leveldb::ReadOptions(), (*vkeys)[i], &value);
        if(status.ok())
        {
            keys.push_back((*vkeys)[i]);
            values.push_back(value);
        }
    }

    rv = new MessageGetAllReply(keys, values);
    return rv;
}

Action* ActionGetAll::clone(leveldb::DB *db, const Message& msg) const
{
    Action *rv = new ActionGetAll(db, ((MessageGetAllRequest&)msg).keys());
    return rv;
}

ActionGetAll::~ActionGetAll()
{
    if(vkeys != NULL)
    {
        delete vkeys;
    }
}

/***
 * Class: Action Delete
 *
 */
ActionDelete::ActionDelete():Action(TYPE_DELETE)
{
	vkey = NULL;
}

ActionDelete::ActionDelete(leveldb::DB *db, const std::string& key):Action(db, TYPE_DELETE)
{
    vkey = new std::string(key);
}

ActionDelete::ActionDelete(const ActionDelete &action):Action(action)
{
    vkey = new std::string(*action.vkey);
}

Message* ActionDelete::execute()
{
    Message *rv = NULL;
    leveldb::Status s = vdb->Delete(leveldb::WriteOptions(), *vkey);

    if(s.ok())
    {
        rv = new MessageDeleteReply(true);
    }
    else
    {
        rv = new MessageDeleteReply(false);
    }

    return rv;
}

Action* ActionDelete::clone(leveldb::DB *db, const Message& msg) const
{
    Action *rv = new ActionDelete(db, ((MessageDeleteRequest&)msg).key());
    return rv;
}

ActionDelete::~ActionDelete()
{
    delete vkey;
}

/***
 * Class: Action Clean
 *
 */
ActionClean::ActionClean():Action(TYPE_CLEAN)
{
}

ActionClean::ActionClean(leveldb::DB *db):Action(db, TYPE_CLEAN)
{
}

ActionClean::ActionClean(const ActionClean &action):Action(action)
{
}

Message* ActionClean::execute()
{
    Message *rv = NULL;
    leveldb::Iterator* it = vdb->NewIterator(leveldb::ReadOptions());
    
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        vdb->Delete(leveldb::WriteOptions(), it->key());
    }
  
    if(it->status().ok())
    {
        rv = new MessageCleanReply(true);
    }
    else
    {
        rv = new MessageCleanReply(false);
    }
    
    delete it;

    return rv;
}

Action* ActionClean::clone(leveldb::DB *db, const Message& msg) const
{
    Action *rv = new ActionClean(db);
    return rv;
}

ActionFactory::ActionFactory(leveldb::DB *db)
{
    Action* action = new ActionSet();
    actions[action->type()]= action;
    
    action = new ActionGet();
    actions[action->type()]= action;

    action = new ActionGetAll();
    actions[action->type()]= action;

    action = new ActionDelete();
    actions[action->type()]= action;

    action = new ActionClean();
    actions[action->type()]= action;

    vdb = db;
}

Action* ActionFactory::parse(const Message &msg)
{
    Action *rv = NULL;
    rv = actions[msg.type()]->clone(vdb, msg);

    return rv;
}

ActionFactory::~ActionFactory()
{
    std::map<std::string, Action*>::iterator it;
    Action *action = NULL;
    for(it=actions.begin(); it != actions.end(); it++)
    {
        action = (*it).second;
        delete action;
    }
    actions.clear();
}
