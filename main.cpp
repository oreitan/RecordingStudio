#include "db.hpp"

extern char username[256];
extern char pass[256];

int main(int argc, char *argv[])
{
    std::string start_date;
    std::string end_date;
    std::string name;
    int choise, chack;
    bool fleg = true;
    if (argc < 3)
    {
        std::cout << "Please insert connection details" << std::endl;
        exit(-1);
    }

    strcpy(username, argv[1]);
    strcpy(pass, argv[2]);
    username[strlen(username)] = '\0';
    pass[strlen(pass)] = '\0';

    if (init() == -1)
    {
        std::cout << "There was an error with DB initlization" << std::endl;
        exit(-1);
    }

    std::cout << "DB initlized successfully" << std::endl;

    do
    {
        std::cout << "choose the number of the question:" << std::endl;
        std::cout << "1) how many albums recorded between the following dates" << std::endl;
        std::cout << "2) how many track a performer recorded between the following dates" << std::endl;
        std::cout << "3) how many albums a performer recorded between the following dates" << std::endl;
        std::cout << "4) the most popular music instrument" << std::endl;
        std::cout << "5) the list of music instrument in an album" << std::endl;
        std::cout << "6) the most fruitful producer" << std::endl;
        std::cout << "7) the most fruitful producer between the following dates" << std::endl;
        std::cout << "8) how many performer recorded in all the years" << std::endl;
        std::cout << "9) the performer that colobrt the most with other perf1ormers" << std::endl;
        std::cout << "10) the most popular music genre " << std::endl;
        std::cout << "11) who is the recording technician that recorded the most tracks in the following dates" << std::endl;
        std::cout << "12) the first album that recorded in the studio" << std::endl;
        std::cout << "13) list of tracks that is included in two albums or more" << std::endl;
        std::cout << "14) the technicians the recorded an album" << std::endl;
        std::cout << "15) the most diverse preformer" << std::endl;
        std::cout << "15) exit" << std::endl;
        std::cin >> choise;
        switch (choise)
        {
        case 1:
            std::cout << "Enher the the dates (yyyy-mm-dd):" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q1(start_date, end_date);
            break;
        case 2:
            std::cout << "enther preformer name:" << std::endl;
            std::cin >> name;
            chack = q2(name);
            break;
        case 3:
            std::cout << "enther preformer name:" << std::endl;
            std::cin >> name;
            chack = q3(name);
            break;
        case 4:
            chack = q4();
            break;

        case 5:
            std::cout << "Enther album name:" << std::endl;
            std::cin >> name;
            chack = q5(name);

        case 6:
            std::cout << "Enher the the dates (yyyy-mm-dd):" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q6(start_date, end_date);
            break;

        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            std::cout << "goodbay" << std::endl;
            fleg = false;
            break;

        default:
            break;
        }

    } while (fleg);

    return 0;
}
