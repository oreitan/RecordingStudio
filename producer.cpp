#include "producer.hpp"


std::vector<Producer *> initProducers(mysqlx_result_t *res)
{
    std::vector<Producer *> v;

    mysqlx_row_t *row;
    do
    {
        while ((row = mysqlx_row_fetch_one(res)) != NULL)
        {
            int64_t x;
            char buff1[256] = {0};
            size_t size = 0;

            size = sizeof(buff1);

            mysqlx_get_sint(row, 0, &x);
            mysqlx_get_bytes(row, 1, 0, buff1, &size);

            v.push_back(new Producer(x, buff1));
        }
    } while (RESULT_OK == mysqlx_next_result(res));

    return v;
}

std::ostream& operator<<(std::ostream& os, const Producer& ob)
{
    os << ob.toString();
    return os;
}