#include "db.hpp"

extern char username[256];
extern char pass[256];

int main(int argc, char *argv[])
{
    std::string start_date;
    std::string end_date;
    std::string name;
    int choice, chack;
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
        std::cout << "Type action number:" << std::endl;
        std::cout << "1 Count how many albums recorded between chosen dates" << std::endl;
        std::cout << "2 Count how many track a musician recorded between chosen dates" << std::endl;
        std::cout << "3 Count how many albums a musician recorded between chosen dates" << std::endl;
        std::cout << "4 The most popular instrument" << std::endl;
        std::cout << "5 List of musical instruments in a chosen album" << std::endl;
        std::cout << "6 The most prolific producer between chosen dates" << std::endl;
        std::cout << "7 The most popular instrument brand" << std::endl;
        std::cout << "8 How many musicians recorded in all time" << std::endl;
        std::cout << "9 The Musician that collaborated the most with other musicians" << std::endl;
        std::cout << "10 The most popular music genre " << std::endl;
        std::cout << "11 The technician that recorded the most tracks between chosen dates" << std::endl;
        std::cout << "12 The first album that was recorded in the studio" << std::endl;
        std::cout << "13 List of tracks included in two albums or more" << std::endl;
        std::cout << "14 List of technicians participating in at least one full album" << std::endl;
        std::cout << "15 the most diverse preformer" << std::endl;
        std::cout << "16 EXIT" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "Enher the the dates (yyyy-mm-dd):" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q1(start_date, end_date);
            break;
        case 2:
            std::cout << "enther preformer name:" << std::endl;
            std::cin >> name;
            std::cout << "Please input the start and end date (YEAR-MONTH-DAY)" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q2(name, start_date, end_date);
            break;
        case 3:
            std::cout << "enther preformer name:" << std::endl;
            std::cin >> name;
            std::cout << "Please input the start and end date (YEAR-MONTH-DAY)" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q3(name,start_date,end_date);
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
            chack = q7();
            break;
        case 8:
            chack = q8();
            break;
        case 9:
            chack = q9();
            break;
        case 10:
            chack = q10();
            break;
        case 11:
            std::cout << "Enher the the dates (yyyy-mm-dd):" << std::endl;
            std::cin >> start_date >> end_date;
            chack = q11(start_date, end_date);
            break;
        case 12:
            chack = q12();
            break;
        case 13:
            chack = q13();
            break;
        case 14:
            chack = q14();
            break;
        case 15:
            chack = q15();
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
