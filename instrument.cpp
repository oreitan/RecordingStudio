#include "instrument.hpp"

std::vector<Instrument *> initInstruments(mysqlx_result_t *res)
{
    std::vector<Instrument *> v;

    mysqlx_row_t *row;
    do
    {
        while ((row = mysqlx_row_fetch_one(res)) != NULL)
        {
            int64_t x;
            char buff1[256] = {0};
            char buff2[256] = {0};

            size_t size = 0;

            size = sizeof(buff1);

            mysqlx_get_sint(row, 0, &x);
            mysqlx_get_bytes(row, 1, 0, buff1, &size);
            size = sizeof(buff2);
            mysqlx_get_bytes(row, 2, 0, buff2, &size);

            v.push_back(new Instrument(x, buff1, buff2));
        }
    } while (RESULT_OK == mysqlx_next_result(res));

    return v;
}

std::ostream &operator<<(std::ostream &os, const Instrument &ob)
{
    os << ob.toString();
    return os;
}
