#include "album.hpp"

std::vector<Album *> initAlbums(mysqlx_result_t *res)
{
    std::vector<Album *> v;

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
            size_t size = 0;

            size = sizeof(buff1);

            mysqlx_get_sint(row, 0, &x);
            size = sizeof(buff1);
            mysqlx_get_bytes(row, 1, 0, buff1, &size);
            size = sizeof(buff2);
            mysqlx_get_bytes(row, 2, 0, buff2, &size);
            size = sizeof(buff3);
            mysqlx_get_bytes(row, 3, 0, buff3, &size);
            mysqlx_get_sint(row, 4, &y);

            v.push_back(new Album(x, buff1, buff2, buff3, y));
        }
    } while (RESULT_OK == mysqlx_next_result(res));

    return v;
}

std::ostream &operator<<(std::ostream &os, const Album &ob)
{
    os << ob.toString();
    return os;
}