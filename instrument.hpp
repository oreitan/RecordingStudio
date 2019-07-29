#pragma once

#include"includes.hpp"

class Instrument
{
public:
    Instrument(int id, char* brand, char* type) : id(id),brand(brand),type(type) {}
    ~Instrument() {}

    int getID() const { return this->id; }
    std::string getBrand() const { return this->brand; }
    std::string getType() const { return this->type; }

    std::string toString() const
    {
        std::stringstream os;
        os << this->id << ", " << this->brand << ", " << this->type;
        return os.str();
    }

private:
    int id;
    std::string brand;
    std::string type;
};

std::vector<Instrument *> initInstruments(mysqlx_result_t *res);
std::ostream &operator<<(std::ostream &os, const Instrument &ob);
