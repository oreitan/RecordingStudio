#pragma once
#include "includes.hpp"

class Song
{
public:
    Song(int id, char *name, char *composer, int length, char *lyrics, char *date, char *genre, char *tech) : id(id), name(name), composer(composer), length(length), lyrics(lyrics), date(date), genre(genre), tech(tech) {}
    ~Song() {}

    int getID() const { return this->id; }
    std::string getName() const { return this->name; }
    std::string getComposer() const { return this->composer; }
    int getLength() const { return this->length; }
    std::string getLyrics() const { return this->lyrics; }
    std::string getDate() const { return this->date; }
    std::string getGenre() const { return this->genre; }
    std::string getTech() const { return this->tech; }

    std::string toString() const
    {
        std::stringstream os;
        os << this->id << ", " << this->name << ", " << this->composer << ", " << this->length << ", " << this->lyrics << ", " << this->date << ", " << this->genre << ", " << this->tech;
        return os.str();
    }

private:
    int id;
    std::string name;
    std::string composer;
    int length;
    std::string lyrics;
    std::string date;
    std::string genre;
    std::string tech;
};

std::vector<Song *> initSongs(mysqlx_result_t *res);
std::ostream &operator<<(std::ostream &os, const Song &ob);