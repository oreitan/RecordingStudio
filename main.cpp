#include "db.hpp"

extern char username[256];
extern char pass[256];

int main(int argc, char *argv[])
{
    std::string start_date;
    std::string end_date;
    std::string name;
    int choise, chack;
    bool fleg;
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
        printf("choose the number of the question:\n");
        printf("1) how many albums recorded between the following dates\n");
        printf("2) how many track a performer recorded between the following dates\n");
        printf("3) how many albums a performer recorded between the following dates\n");
        printf("4) the most popular music instrument\n");
        printf("5) the list of music instrument in an album\n");
        printf("6) the most fruitful producer\n");
        printf("7) the most fruitful producer between the following dates\n");
        printf("8) how many performer recorded in all the years\n");
        printf("9) the performer that colobrt the most with other perf1ormers\n");
        printf("10) the most popular music genre \n");
        printf("11) who is the recording technician that recorded the most tracks in the following dates\n");
        printf("12) the first album that recorded in the studio\n");
        printf("13) list of tracks that is included in two albums or more\n");
        printf("14) the technicians the recorded an album\n");
        printf("15) the most diverse preformer\n");
        scanf("%d", choise);
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

        case 5:
            std::cout<<"Enther album name:"<<std::endl;
            std::cin>>name;
            chack = q5(name);


        default:
            break;
        }

    } while (fleg);

    return 0;
}
