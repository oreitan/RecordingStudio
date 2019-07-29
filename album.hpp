#pragma once
#include "includes.hpp"

class Album
{
public:
    Album(int id, char *name, char *start, char *end, int tracks) : id(id), name(name), start(start), end(end), tracks(tracks) {}
    ~Album() {}

    int getID() const { return this->id; }
    std::string getName() const { return this->name; }
    std::string getStartDate() const { return this->start; }
    std::string getEndDate() const { return this->end; }
    int getTracks() const { return this->tracks; }

    std::string toString() const
    {
        std::stringstream os;
        os << this->id << ", " << this->name << ", " << this->start + ", " << this->end << ", " << tracks;
        return os.str();
    }

private:
    int id;
    std::string name;
    std::string start;
    std::string end;
    int tracks;
};

std::vector<Album *> initAlbum(mysqlx_result_t *res);
std::ostream &operator<<(std::ostream &os, const Album &ob);