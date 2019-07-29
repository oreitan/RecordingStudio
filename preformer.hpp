#pragma once

#include "includes.hpp"

class Preformer
{
public:
    Preformer(int id, char* name, char* address, char* phone, char* skill):id(id),name(name),address(address),phone(phone),skill(skill){}
    ~Preformer(){}
    int getID() const { return this->id; }
    std::string getName() const { return this->name; }
    std::string getAddress() const { return this->address; }
    std::string getPhone() const { return this->phone; }
    std::string getSkill() const { return this->skill; }
private:
    int id;
    std::string name;
    std::string address;
    std::string phone;
    std::string skill;
};

std::vector<Preformer *> initPreformers(mysqlx_result_t *res)
{
    std::vector<Preformer *> v;

    mysqlx_row_t *row;
    do
    {
        while ((row = mysqlx_row_fetch_one(res)) != NULL)
        {
            int64_t x;
            char buff1[256] = {0};
            char buff2[256] = {0};
            char buff3[256] = {0};
            char buff4[256] = {0};
            size_t size = 0;

            size = sizeof(buff1);

            mysqlx_get_sint(row, 0, &x);
            mysqlx_get_bytes(row, 1, 0, buff1, &size);
            size = sizeof(buff2);
            mysqlx_get_bytes(row, 2, 0, buff2, &size);
            size = sizeof(buff3);
            mysqlx_get_bytes(row, 3, 0, buff3, &size);
            size = sizeof(buff4);
            mysqlx_get_bytes(row, 4, 0, buff4, &size);

            v.push_back(new Preformer(x, buff1, buff2, buff3, buff4));
        }
    } while (RESULT_OK == mysqlx_next_result(res));

    return v;
}