#include "song.hpp"

std::vector<Song *> initSongs(mysqlx_result_t *res)
{
    std::vector<Song *> v;

    mysqlx_row_t *row;
    do
    {
        while ((row = mysqlx_row_fetch_one(res)) != NULL)
        {
            int64_t x;
            int64_t y;
            char buff1[256] = {0};
            char buff2[256] = {0};
            char buff3[256] = {0};
            char buff4[256] = {0};
            char buff5[256] = {0};
            char buff6[256] = {0};

            size_t size = 0;

            size = sizeof(buff1);
            mysqlx_get_sint(row, 0, &x);
            size = sizeof(buff1);
            mysqlx_get_bytes(row, 1, 0, buff1, &size);
            size = sizeof(buff2);
            mysqlx_get_bytes(row, 2, 0, buff2, &size);
            mysqlx_get_sint(row, 3, &y);
            size = sizeof(buff3);
            mysqlx_get_bytes(row, 4, 0, buff3, &size);
            size = sizeof(buff4);
            mysqlx_get_bytes(row, 5, 0, buff4, &size);
            size = sizeof(buff5);
            mysqlx_get_bytes(row, 6, 0, buff5, &size);
            size = sizeof(buff6);
            mysqlx_get_bytes(row, 7, 0, buff6, &size);

            v.push_back(new Song(x, buff1, buff2, y, buff3, buff4, buff5, buff6));
        }
    } while (RESULT_OK == mysqlx_next_result(res));

    return v;
}

std::ostream &operator<<(std::ostream &os, const Song &ob)
{
    os << ob.toString();
    return os;
}