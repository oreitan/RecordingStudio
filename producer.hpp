#pragma once
#include "includes.hpp"

class Producer
{
public:
    Producer(int id, char* name) : id(id),name(name) {}
    ~Producer() {}

    int getID() const { return this->id; }
    std::string getName() const { return this->name; }

    std::string toString() const
    {
        std::stringstream os;
        os << this->id << ", " << this->name;
        return os.str();
    }

private:
    int id;
    std::string name;
};

std::vector<Producer *> initProducers(mysqlx_result_t *res);
std::ostream& operator<<(std::ostream& os, const Producer& ob);