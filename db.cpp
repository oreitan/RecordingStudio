#include "db.hpp"
#include "preformer.hpp"
#include "instrument.hpp"
#include "song.hpp"
#include "album.hpp"
#include "producer.hpp"

mysqlx_session_t *session;
mysqlx_error_t *err;

char username[256];
char pass[256];

int init()
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

#pragma region create
  mysqlx_stmt_t *query;
  mysqlx_result_t *result;

  if (mysqlx_get_schema(session, "recording-studio", 1))
  {
    query = mysqlx_sql_new(session, "DROP database `recording-studio`;",
                           MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) == NULL)
      return -1;
  }

  query = mysqlx_sql_new(session, "create database `recording-studio`;",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  mysqlx_session_close(session);

  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);

  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  query = mysqlx_sql_new(session, "CREATE TABLE `Musician` (\
 `ID_Musician` INT NOT NULL,\
 `Name` VARCHAR(256) NOT NULL,\
 `Skill` VARCHAR(256) NOT NULL,\
 `Phone` VARCHAR(256) NOT NULL,\
 `Address` VARCHAR(256) NOT NULL,\
 PRIMARY KEY (`ID_Musician`));",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Inst` (\
 `I_ID` INT NOT NULL UNIQUE,\
 `Brand` VARCHAR(256) NOT NULL,\
 `Type` VARCHAR(256) NOT NULL,\
 PRIMARY KEY (`I_ID`, `Brand`, `Type`));",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Tracks` (\
 `T_ID` INT NOT NULL,\
 `Name` VARCHAR(256) NOT NULL,\
 `Music_Comp` VARCHAR(256) NULL,\
 `Lyrics_Comp` VARCHAR(256) NULL,\
 `Length` INT NOT NULL,\
 `Date` VARCHAR(256) NOT NULL,\
 `Genre` VARCHAR(256) NOT NULL,\
 `Tech` VARCHAR(256) NOT NULL,\
 PRIMARY KEY (`T_ID`));",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Albums` (\
 `A_ID` INT NOT NULL,\
 `Name` VARCHAR(256) NOT NULL,\
 `Start_Date` VARCHAR(256) NOT NULL,\
 `End_Date` VARCHAR(256) NOT NULL,\
 `Tracks_Amount` INT NOT NULL,\
 PRIMARY KEY (`A_ID`));",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Prod` (\
 `P_ID` INT NOT NULL,\
 `Name` VARCHAR(256) NOT NULL,\
 PRIMARY KEY (`P_ID`));",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  // Relations tables

  query = mysqlx_sql_new(session, "CREATE TABLE `Musician_Inst` (\
 `M_ID` INT NOT NULL,\
 `I_ID` INT NOT NULL,\
 PRIMARY KEY (`I_ID`, `M_ID`),\
 INDEX `MI_mID_idx` (`M_ID` ASC) VISIBLE,\
 CONSTRAINT `MI_mID`\
 FOREIGN KEY (`M_ID`)\
 REFERENCES `Musician` (`ID_Musician`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION,\
 CONSTRAINT `MI_iID`\
 FOREIGN KEY (`I_ID`)\
 REFERENCES `Inst` (`I_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Musician_Tracks` (\
 `M_ID` INT NOT NULL,\
 `T_ID` INT NOT NULL,\
 PRIMARY KEY (`M_ID`, `T_ID`),\
 INDEX `MT_tID_idx` (`t_ID` ASC) VISIBLE,\
 CONSTRAINT `MT_mID`\
 FOREIGN KEY (`M_ID`)\
 REFERENCES `Musician` (`ID_Musician`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION,\
 CONSTRAINT `MT_tID`\
 FOREIGN KEY (`T_ID`)\
 REFERENCES `Tracks` (`T_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Album_Tracks` (\
 `T_ID` INT NOT NULL,\
 `A_ID` INT NOT NULL,\
 PRIMARY KEY (`T_ID`, `A_ID`),\
 INDEX `AT_aID_idx` (`A_ID` ASC) VISIBLE,\
 CONSTRAINT `AT_tID`\
 FOREIGN KEY (`T_ID`)\
 REFERENCES `Tracks` (`T_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION,\
 CONSTRAINT `AT_aID`\
 FOREIGN KEY (`A_ID`)\
 REFERENCES `Albums` (`A_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Album_Prod` (\
 `A_ID` INT NOT NULL,\
 `P_ID` INT NOT NULL,\
 PRIMARY KEY (`A_ID`, `P_ID`),\
 INDEX `AP_pID_idx` (`P_ID` ASC) VISIBLE,\
 CONSTRAINT `AP_aID`\
 FOREIGN KEY (`A_ID`)\
 REFERENCES `albums` (`A_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION,\
 CONSTRAINT `AP_pID`\
 FOREIGN KEY (`P_ID`)\
 REFERENCES `prod` (`P_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

#pragma endregion

#pragma region insert
  query = mysqlx_sql_new(session, "INSERT INTO `Musician` (`ID_musician`, `Name`, `Skill`, `Phone`, `Address`) VALUES\
 (1, 'Marshmello', '1', '1-111-234', 'US'),\
 (2, 'Blink-182', '2', '1-234-652', 'US'),\
 (3, 'Pixies', '2', '1-432-566', 'US'),\
 (4, 'Kesha', '1', '1-324-643', 'US'),\
 (5, 'Pitbull', '0', '1-432-654', 'US'),\
 (6, 'Madonna', '0', '1-749-234', 'US'),\
 (7, 'Bastille', '2', '44-234-743', 'UK'),\
 (8, 'ODESZA', '1', '1-532-754', 'US'),\
 (9, 'WYNNE', '0', '1-643-830', 'US'),\
 (10, 'Mansionair', '0', 'H', 'AU');",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `Tracks` (`T_ID`, `Name`, `Music_Comp`, `Lyrics_Comp`, `Length`, `Date`, `Genre`, `Tech`) VALUES\
 (1, 'Summer', 'Marshmello', 'NULL', 171, '2017-01-09', 'Electonic', 'Daniel Malikier'),\
 (2, 'Happier', 'Marshmello', 'Dan Smith', 223, '2018-09-24', 'Electronic', 'Mercedes Bryce Morgan'),\
 (3, 'Dammit', 'Mark Hoppus', 'Tom DeLonge', 165, '1997-09-23', 'Rock', 'Mark Trombino'),\
 (4, 'Hey', 'Joey Santiago', 'Black Francis', 210, '1988-11-02', 'Rock', 'Gil Norton'),\
 (5, 'TikTok', 'Benny Blanco', 'Kesha Sebert', 200, '2009-08-07', 'Pop', 'Benny Blanco'),\
 (6, 'Dinosaur', 'Benny Blanco', 'Kesha Sebert', 175, '2010-01-01', 'Pop', 'Benny Blanco'),\
 (7, 'Timber', 'Benny Blanco', 'Kesha Sebert', 204, '2013-10-06', 'Pop', 'Dr Luke'),\
 (8, 'Boy', 'ODESZA', 'WYNNE', 183, '2017-09-08', 'Electonic', 'ODESZA'),\
 (9, 'Pompeii', 'ODESZA', 'Dan Smith', 203, '2012-12-12', 'Electonic', 'Mercedes Bryce Morgan'),\
 (10, 'Vogue', 'Shep Pettibone', 'Madonna', 290, '1990-03-27', 'Pop', 'Shep Pettibone');",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `albums` (`A_ID`, `Name`, `Start_Date`, `End_Date`, `Tracks_Amount`) VALUES\
 (1, 'Joytime', '2017-01-08', '2018-09-25', 2),\
 (2, 'Happier', '2018-09-24', '2018-09-25', 1),\
 (3, 'Buddha', '1997-09-22', '1997-09-23', 1),\
 (4, 'Doolittle', '1988-11-01', '1988-11-02', 1),\
 (5, 'Animal', '2009-08-06', '2013-10-06', 3),\
 (6, 'Timber', '2013-10-05', '2013-10-06', 1),\
 (7, 'Loyal', '2017-09-07', '2017-09-08', 1),\
 (8, 'Pompeii', '2012-12-10', '2012-12-12', 1),\
 (9, 'VS', '2018-01-07', '2018-01-09', 2),\
 (10, 'Vogue', '1990-03-25', '1990-03-27', 1);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `inst` (`I_ID`, `Brand`, `Type`) VALUES\
 (1, 'Arturia', 'Synth'),\
 (2, 'Korg', 'Synth'),\
 (3, 'Yamaha', 'Piano'),\
 (4, 'Gibson', 'Guitar'),\
 (5, 'Ludwig', 'Drums'),\
 (6, 'Fender', 'Bass'),\
 (7, 'LP', 'Conga'),\
 (8, 'Tycoon', 'Djemba'),\
 (9, 'Meinl', 'Cajon'),\
 (10, 'Kala', 'Ukelele');",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `prod` (`P_ID`, `Name`) VALUES\
 (1, 'Daniel Malikier'),\
 (2, 'Mercedes Bryce Morgan'),\
 (3, 'Quincy Jones'),\
 (4, 'Dr Luke'),\
 (5, 'George Martin'),\
 (6, 'Shep Pettibone'),\
 (7, 'Jay-Z'),\
 (8, 'Benny Blanco'),\
 (9, 'Gil Norton'),\
 (10, 'Mark Trombino');",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `musician_inst` (`M_ID`, `I_ID`) VALUES\
 (1,1),\
 (1,8),\
 (2,4),\
 (2,5),\
 (2,6),\
 (3,3),\
 (3,4),\
 (3,5),\
 (3,6),\
 (4,10),\
 (7,4),\
 (7,5),\
 (7,7),\
 (7,8),\
 (7,9),\
 (8,2),\
 (8,8);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `musician_tracks` (`M_ID`, `T_ID`) VALUES\
 (1,1),\
 (1,2),\
 (7,2),\
 (2,3),\
 (3,4),\
 (4,5),\
 (4,6),\
 (4,7),\
 (5,7),\
 (8,8),\
 (9,8),\
 (10,8),\
 (7,9),\
 (8,9),\
 (6,10);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `album_tracks`(`T_ID`, `A_ID`) VALUES\
 (1,1),\
 (2,1),\
 (2,2),\
 (3,3),\
 (4,4),\
 (5,5),\
 (6,5),\
 (7,5),\
 (7,6),\
 (8,7),\
 (9,8),\
 (2,9),\
 (9,9),\
 (10,10);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `album_prod` (`A_ID`,`P_ID`) VALUES\
 (1,1),\
 (2,2),\
 (2,3),\
 (3,10),\
 (4,9),\
 (5,8),\
 (5,5),\
 (5,4),\
 (6,8),\
 (7,8),\
 (8,7),\
 (9,2),\
 (10,3),\
 (10,6);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;
#pragma endregion

  mysqlx_free(result);
  mysqlx_session_close(session);
  return 0;
}

int q1(std::string start_date, std::string end_date)
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;
  mysqlx_result_t *result;

  std::string qstr = "select count(*) from albums where End_Date > '" + start_date +
                     "' AND End_Date < '" + end_date + "';\0";

  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);
  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  mysqlx_row_t *row;
  int64_t x;

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The amount of albums between " << start_date << " and " << end_date
              << " are : " << x << std::endl
              << std::endl
              << std::endl;
  }
  else
  {
    std::cout << "There was an error running the Query." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);

  return 0;
}

int q2(std::string name)
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;
  mysqlx_result_t *result;
  std::string start_date;
  std::string end_date;

  std::string qstr = "select * from musician where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  int choice = 1;

  std::vector<Performer *> v = initPerformers(result);

  if (v.size() == 0)
  {
    std::cout << "There is no user with that name want to try again ? y/n";
    char c;
    std::cin >> c;
    if (c == 'y' || c == 'Y')
      return 2;
    else
      return -1;
  }

  if (v.size() > 1)
  {
    for (int i = 0; i < v.size(); ++i)
    {
      std::cout << i + 1 << " - " << *v[i] << std::endl;
    }

    do
    {
      std::cout << "Please insert the index of the Performer you want" << std::endl;
      std::cin >> choice;

      if (choice < 1 || choice > v.size())
        std::cout << "INCORRECT VALUE RETRY" << std::endl;

    } while (choice < 1 || choice > v.size());
  }

  std::cout << "Please input the start and end date (YEAR-MONTH-DAY)" << std::endl;
  std::cin >> start_date >> end_date;

  qstr = "select count(*) from tracks as t join musician_tracks as m on \
 m.T_ID = t.T_ID where M_ID = " +
         std::to_string(v[choice - 1]->getID()) + " and t.Date > '" + start_date + "' and t.Date < '" + end_date + "';\0";

  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  mysqlx_row_t *row;
  int64_t x;

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The amount of tracks that " << v[choice - 1]->getName() << " preformed between " << start_date << " and " << end_date << " are : " << x << std::endl
              << std::endl
              << std::endl;
  }
  else
  {
    std::cout << "There was an error running the Query." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);

  return 0;
}

int q3(std::string name)
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;
  mysqlx_result_t *result;
  std::string start_date;
  std::string end_date;
  mysqlx_row_t *row;
  int64_t x;

  std::string qstr = "select * from musician where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  int choice = 1;

  std::vector<Performer *> v = initPerformers(result);

  if (v.size() == 0)
  {
    std::cout << "There is no user with that name want to try again ? y/n";
    char c;
    std::cin >> c;
    if (c == 'y' || c == 'Y')
      return 2;
    else
      return -1;
  }

  if (v.size() > 1)
  {
    for (int i = 0; i < v.size(); ++i)
    {
      std::cout << i + 1 << " - " << *v[i] << std::endl;
    }

    do
    {
      std::cout << "Please insert the index of the Performer you want" << std::endl;
      std::cin >> choice;

      if (choice < 1 || choice > v.size())
        std::cout << "INCORRECT VALUE RETRY" << std::endl;

    } while (choice < 1 || choice > v.size());
  }

  std::cout << "Please input the start and end date (YEAR-MONTH-DAY)" << std::endl;
  std::cin >> start_date >> end_date;

  qstr = "select count(*) from albums as ab join (select A_ID from( (select * from Album_tracks) as a INNER JOIN\
 (select * from Musician_tracks where Musician_tracks.M_ID = " +
         std::to_string(v[choice - 1]->getID()) + ") as b\
 on a.T_ID = b.T_ID) group by A_ID ) as c on ab.A_id = c.A_ID where End_Date > '" +
         start_date + "' and End_Date < '" + end_date + "' ;\0";

  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The amount of Albums that " << v[choice - 1]->getName() << " released between " << start_date << " and " << end_date << " are : " << x << std::endl
              << std::endl
              << std::endl;
  }
  else
  {
    std::cout << "There was an error running the Query." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  return 0;
}

int q4()
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;
  mysqlx_result_t *result;

  std::string qstr = "SELECT * FROM inst;\0";
  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  std::vector<Instrument *> v = initInstruments(result);
  int max = -1;
  int index = -1;

  mysqlx_row_t *row;

  for (int i = 0; i < v.size(); ++i)
  {
    qstr = "SELECT count(*) from (\
 (SELECT * FROM musician_Inst where I_ID = " +
           std::to_string(v[i]->getID()) + " ) as a join musician_Tracks as b \
 on a.M_ID = b.M_ID );";

    std::cout << std::endl
              << std::endl
              << qstr << std::endl;
    query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

    int value = -1;

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        int64_t x;
        mysqlx_get_sint(row, 0, &x);
        value = x;
        std::cout << std::to_string(v[i]->getID()) << " , " << value << std::endl;
      }
      else
      {
        std::cout << "There was an error running the Query." << std::endl;
      }
      if (value > max)
      {
        max = value;
        index = i;
      }
    }
  }

  if (max == -1 || index == -1)
    std::cout << "There was an error running the Query." << std::endl;
  else
    std::cout << "The most popular instrument is:\t" << v[index]->getBrand() << ", " << v[index]->getType() << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  return 0;
}

int q5(std::string name)
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;
  mysqlx_result_t *result;

  std::string qstr = "SELECT * FROM albums where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There was an error executing the Query" << std::endl;
    return -1;
  }

  std::vector<Album *> album = initAlbum(result);

  if (album.size() == 0)
  {
    std::cout << "There is no user with that name want to try again ? y/n";
    char c;
    std::cin >> c;
    if (c == 'y' || c == 'Y')
      return 2;
    else
      return -1;
  }

  qstr = "select * from Inst as i join\
 (select I_ID from Musician_Inst as mi join\
 (select M_ID from Album_Tracks as a  join Musician_Tracks as b on a.T_ID = b.T_ID where a.A_ID = " +
         std::to_string(album[0]->getID()) + ")\
 as mb on mi.M_ID = mb.M_ID) as b on i.I_ID = b.I_ID ;\0";

  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "There is no Performer with this name" << std::endl;
    return -1;
  }

  std::vector<Instrument *> v = initInstruments(result);

  if (v.size() == 0)
    std::cout << "No instruments were used in this album" << std::endl;
  else
  {
    std::cout << "Instruments used in this album are :" << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < v.size(); ++i)
      std::cout << i + 1 << ".\t" << *v[i] << std::endl;
  }
  std::cout << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);

  return 0;
}

int q6(std::string start_Date, std::string end_Date)
{
  session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, pass,
                               "recording-studio", &err);
  if (NULL == session)
  {
    std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
    return -1;
  }

  mysqlx_stmt_t *query;

  mysqlx_result_t *result;

  std::string qstr = "SELECT * FROM Prod;\0";

  query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)

  {

    std::cout << "There was an error executing the Query" << std::endl;

    return -1;
  }

  std::vector<Producer *> v = initProducers(result);

  int max = -1;

  int index = -1;

  mysqlx_row_t *row;

  int64_t x;

  for (int i = 0; i < v.size(); ++i)

  {

    qstr = "select count(*) from (\
 (SELECT * FROM album_prod where P_ID = " +
           std::to_string(v[i]->getID()) + ") as a join albums as b on a.A_ID = b.A_ID )\
 where End_Date  > '" +
           start_Date + "' and End_Date < '" + end_Date + "';";

    query = mysqlx_sql_new(session, qstr.c_str(), MYSQLX_NULL_TERMINATED);

    int value = 0;

    if ((result = mysqlx_execute(query)) != NULL)

    {

      if ((row = mysqlx_row_fetch_one(result)) != NULL)

      {

        mysqlx_get_sint(row, 0, &x);

        value = x;
      }

      else

      {

        std::cout << "There was an error running the Query." << std::endl;
      }

      if (value > max)

      {

        max = value;

        index = i;
      }
    }
  }

  if (max == -1 || index == -1)

    std::cout << "There was an error running the Query." << std::endl;

  else

    std::cout << "The most popular Producer is:\t" << v[index]->getName() << std::endl;

  mysqlx_free(result);

  return 0;
}