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

int get_Index(std::vector<std::string> &vector, std::string val)

{

  for (int i = 0; i < vector.size(); ++i)

    if (vector[i] == val)

      return i;

  return -1;
}

int get_IndexI(std::vector<int> &vector, int x)
{
  for (int i = 0; i < vector.size(); ++i)

    if (vector[i] == x)

      return i;

  return -1;
}

int get_max(std::vector<int> &vector)

{

  int index = 0;

  int max = vector[0];

  for (int i = 1; i < vector.size(); ++i)

    if (vector[i] > max)

    {

      max = vector[i];

      index = i;
    }

  return index;
}
template <typename T>
void vector_Free(std::vector<T*> vec)
{
  for (int i ;i<vec.size();i++)
  {
    delete vec[i];
  }
}

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

  query = mysqlx_sql_new(session, "CREATE TABLE `Preformer` (\
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

  query = mysqlx_sql_new(session, "CREATE TABLE `Songs` (\
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
 `Songs_Amount` INT NOT NULL,\
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

  query = mysqlx_sql_new(session, "CREATE TABLE `Preformer_Inst` (\
 `M_ID` INT NOT NULL,\
 `I_ID` INT NOT NULL,\
 PRIMARY KEY (`I_ID`, `M_ID`),\
 INDEX `MI_mID_idx` (`M_ID` ASC) VISIBLE,\
 CONSTRAINT `MI_mID`\
 FOREIGN KEY (`M_ID`)\
 REFERENCES `Preformer` (`ID_Musician`)\
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

  query = mysqlx_sql_new(session, "CREATE TABLE `Preformer_Songs` (\
 `M_ID` INT NOT NULL,\
 `T_ID` INT NOT NULL,\
 PRIMARY KEY (`M_ID`, `T_ID`),\
 INDEX `MT_tID_idx` (`t_ID` ASC) VISIBLE,\
 CONSTRAINT `MT_mID`\
 FOREIGN KEY (`M_ID`)\
 REFERENCES `Preformer` (`ID_Musician`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION,\
 CONSTRAINT `MT_tID`\
 FOREIGN KEY (`T_ID`)\
 REFERENCES `Songs` (`T_ID`)\
 ON DELETE NO ACTION\
 ON UPDATE NO ACTION);",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "CREATE TABLE `Album_Songs` (\
 `T_ID` INT NOT NULL,\
 `A_ID` INT NOT NULL,\
 PRIMARY KEY (`T_ID`, `A_ID`),\
 INDEX `AT_aID_idx` (`A_ID` ASC) VISIBLE,\
 CONSTRAINT `AT_tID`\
 FOREIGN KEY (`T_ID`)\
 REFERENCES `Songs` (`T_ID`)\
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
  query = mysqlx_sql_new(session, "INSERT INTO `Preformer` (`ID_musician`, `Name`, `Skill`, `Phone`, `Address`) VALUES\
 (1, 'Marshmello', '1', '1-111-234', 'US'),\
 (2, 'Blink-182', '2', '1-234-652', 'US'),\
 (3, 'Pixies', '2', '1-432-566', 'US'),\
 (4, 'Kesha', '1', '1-324-643', 'US'),\
 (5, 'Pitbull', '0', '1-432-654', 'US'),\
 (6, 'Madonna', '0', '1-749-234', 'US'),\
 (7, 'Bastille', '2', '44-234-743', 'UK'),\
 (8, 'ODESZA', '1', '1-532-754', 'US'),\
 (9, 'WYNNE', '0', '1-643-830', 'US'),\
 (10, 'Mansionair', '0', '61-321-964', 'AU'),\
 (11, 'Dan Smith', '0', '44-234-743', 'UK'),\
 (12, 'Madonna', '0', '61-593-9532', 'AU');",
                         MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  query = mysqlx_sql_new(session, "INSERT INTO `Songs` (`T_ID`, `Name`, `Music_Comp`, `Lyrics_Comp`, `Length`, `Date`, `Genre`, `Tech`) VALUES\
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

  query = mysqlx_sql_new(session, "INSERT INTO `albums` (`A_ID`, `Name`, `Start_Date`, `End_Date`, `Songs_Amount`) VALUES\
 (1, 'Joytime', '2017-01-08', '2018-09-25', 2),\
 (2, 'Happier', '2018-09-24', '2018-09-25', 1),\
 (3, 'Buddha', '1997-09-22', '1997-09-23', 1),\
 (4, 'Doolittle', '1988-11-01', '1988-11-02', 1),\
 (5, 'Animal', '2009-08-06', '2013-10-06', 3),\
 (6, 'Timber', '2013-10-05', '2013-10-06', 1),\
 (7, 'Loyal', '2017-09-07', '2017-09-08', 1),\
 (8, 'Pompeii', '2012-12-10', '2012-12-12', 1),\
 (9, 'VS', '2018-01-07', '2018-01-09', 2),\
 (10, 'Vogue', '1990-03-25', '1990-03-27', 1),\
 (11, 'Joytime II', '2019-01-08', '2019-09-25', 1);",
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

  query = mysqlx_sql_new(session, "INSERT INTO `Preformer_inst` (`M_ID`, `I_ID`) VALUES\
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

  query = mysqlx_sql_new(session, "INSERT INTO `Preformer_Songs` (`M_ID`, `T_ID`) VALUES\
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

  query = mysqlx_sql_new(session, "INSERT INTO `album_Songs`(`T_ID`, `A_ID`) VALUES\
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
 (10,10),\
 (1,11);",
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

  std::string str_query = "select count(*) from albums where End_Date > '" + start_date +
                          "' AND End_Date < '" + end_date + "';\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);
  if ((result = mysqlx_execute(query)) == NULL)
    return -1;

  mysqlx_row_t *row;
  int64_t x;

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The number of albums in between the two dates are: "<< x << std::endl;
  }
  else
  {
    std::cout << "Query Error." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);

  return 0;
}

int q2(std::string name, std::string start_date, std::string end_date)
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

  std::string str_query = "select * from Preformer where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Performer *> performers = initPerformers(result);

  if (performers.size() == 0)
  {
    std::cout << "No performer with this name" << std::endl;
    return -1;
  }

  int choice = 0;

  for (int i = 0; i < performers.size(); ++i)
  {
    std::cout << i + 1 << " - " << *performers[i] << std::endl;
  }

  if (performers.size() > 1)
  {
    do
    {
      std::cout << "Please select a performer" << std::endl;
      std::cin >> choice;

      if (choice < 1 || choice > performers.size())
        std::cout << "number isnt in range" << std::endl;

    } while (choice < 1 || choice > performers.size());
  }

  str_query = "select count(*) from Songs as t join Preformer_Songs as m on \
 m.T_ID = t.T_ID where M_ID = " +
              std::to_string(performers[choice - 1]->getID()) + " and t.Date > '" + start_date + "' and t.Date < '" + end_date + "';\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  mysqlx_row_t *row;
  int64_t x;

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "Number of Songs from " << performers[choice - 1]->getName() << " is: " << x << std::endl;
  }
  else
  {
    std::cout << "Query Error." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(performers);

  return 0;
}

int q3(std::string name, std::string start_date, std::string end_date)
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
  mysqlx_row_t *row;
  int64_t x;

  std::string str_query = "select * from Preformer where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  int choice = 1;

  std::vector<Performer *> performers = initPerformers(result);

  if (performers.size() == 0)
  {
    std::cout << "There is no performer with this name" << std::endl;
    return -1;
  }

  if (performers.size() > 1)
  {
    for (int i = 0; i < performers.size(); ++i)
    {
      std::cout << i + 1 << " - " << *performers[i] << std::endl;
    }

    do
    {
      std::cout << "Please select a performer" << std::endl;
      std::cin >> choice;

      if (choice < 1 || choice > performers.size())
        std::cout << "choice out of range" << std::endl;

    } while (choice < 1 || choice > performers.size());
  }

  str_query = "select count(*) from albums as ab join (select A_ID from( (select * from Album_Songs) as a INNER JOIN\
 (select * from Preformer_Songs where Preformer_Songs.M_ID = " +
              std::to_string(performers[choice - 1]->getID()) + ") as b\
 on a.T_ID = b.T_ID) group by A_ID ) as c on ab.A_id = c.A_ID where End_Date > '" +
              start_date + "' and End_Date < '" + end_date + "' ;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The number of albums are " << x << std::endl;
  }
  else
  {
    std::cout << "Query Error." << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(performers);
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

  std::string str_query = "SELECT * FROM inst;\0";
  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Instrument *> instruments = initInstruments(result);
  int max = -1;
  int index = -1;

  mysqlx_row_t *row;
  int64_t x;

  for (int i = 0; i < instruments.size(); ++i)
  {
    str_query = "SELECT count(*) from (\
 (SELECT * FROM Preformer_Inst where I_ID = " +
                std::to_string(instruments[i]->getID()) + " ) as a join Preformer_Songs as b \
 on a.M_ID = b.M_ID );";

    std::cout << str_query << std::endl;
    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &x);
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }
      if (x > max)
      {
        max = x;
        index = i;
      }
    }
  }

  if (max == -1 || index == -1)
    std::cout << "Query Error." << std::endl;
  else
    std::cout << "The most popular instrument is:\t" << instruments[index]->getBrand() << ", " << instruments[index]->getType() << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(instruments);
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

  std::string str_query = "SELECT * FROM albums where Name Like '%" + name + "%';\0";
  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Album *> album = initAlbums(result);

  if (album.size() == 0)
  {
    std::cout << "There are no albums with this name" << std::endl;
    return -1;
  }

  str_query = "select * from Inst as i join\
 (select I_ID from Preformer_Inst as mi join\
 (select M_ID from Album_Songs as a  join Preformer_Songs as b on a.T_ID = b.T_ID where a.A_ID = " +
              std::to_string(album[0]->getID()) + ")\
 as mb on mi.M_ID = mb.M_ID) as b on i.I_ID = b.I_ID ;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Instrument *> instruments = initInstruments(result);

  if (instruments.size() == 0)
    std::cout << "No instruments were used in this album" << std::endl;
  else
  {
    std::cout << "Instruments used are :" << std::endl;
    for (int i = 0; i < instruments.size(); ++i)
      std::cout << i + 1 << ".\t" << *instruments[i] << std::endl;
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(album);
  vector_Free(instruments);

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

  std::string str_query = "SELECT * FROM Prod;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Producer *> producers = initProducers(result);
  int max = -1;
  int index = -1;
  mysqlx_row_t *row;
  int64_t x;

  for (int i = 0; i < producers.size(); ++i)
  {
    str_query = "select count(*) from (\
 (SELECT * FROM album_prod where P_ID = " +
                std::to_string(producers[i]->getID()) + ") as a join albums as b on a.A_ID = b.A_ID )\
 where End_Date  > '" +
                start_Date + "' and End_Date < '" + end_Date + "';";

    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);
    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &x);
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }
      if (x > max)
      {
        max = x;
        index = i;
      }
    }
  }

  if (max == -1 || index == -1)
    std::cout << "Query Error." << std::endl;
  else
    std::cout << producers[index]->getName() << " is the most fruitful Producer" << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(producers);
  return 0;
}

int q7()
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
  mysqlx_row_t *row;
  int64_t x;
  std::vector<std::string> manufacterers;
  std::vector<int> count;

  std::string str_query = "SELECT * FROM Inst;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Instrument *> instruments = initInstruments(result);

  for (int i = 0; i < instruments.size(); ++i)
  {
    str_query = "SELECT count(*) from (\
 (SELECT * FROM Preformer_inst where I_ID = " +
                std::to_string(instruments[i]->getID()) + ") as a join Preformer_Songs as b\
 on a.M_ID = b.M_ID);";

    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &x);
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }

      if (get_Index(manufacterers, instruments[i]->getBrand()) == -1)
      {
        manufacterers.push_back(instruments[i]->getBrand());
        count.push_back(x);
      }
      else
      {
        int index;
        if ((index = get_Index(manufacterers, instruments[i]->getBrand())) != -1)
          count[index] += x;
      }
    }
  }

  std::cout << manufacterers[get_max(count)] <<"is the most Popular Manufacterer" << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(instruments);

  return 0;
}

int q8()
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

  std::string str_query = "SELECT COUNT(*) FROM (SELECT * FROM Preformer  JOIN Preformer_Songs ON id_musician = M_ID group by M_ID) AS b;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  mysqlx_row_t *row;
  int64_t x;

  if ((row = mysqlx_row_fetch_one(result)) != NULL)
  {
    mysqlx_get_sint(row, 0, &x);
    std::cout << "The number of performers:\t" << x << std::endl;
  }
  else
  {
    std::cout << "Query Error." << std::endl;
  }

  mysqlx_free(result);

  return 0;
}

int q9()
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
  std::vector<int> musicians;
  std::vector<int> count;
  int64_t x;

  std::string str_query = "SELECT * FROM Songs;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Song *> songs = initSongs(result);

  for (int i = 0; i < songs.size(); ++i)
  {
    int64_t qcount = 0;
    mysqlx_row_t *row;

    str_query = "select count(*) from Preformer_Songs where T_ID = " + std::to_string(songs[i]->getID()) + " ;";
    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &qcount);

        if (qcount > 1)
        {
          str_query = "select * from Preformer_Songs where T_ID = '" + std::to_string(songs[i]->getID()) + "';\0";

          query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

          if ((result = mysqlx_execute(query)) != NULL)
          {
            do
            {
              while ((row = mysqlx_row_fetch_one(result)) != NULL)
              {
                mysqlx_get_sint(row, 0, &x);
                int index;

                if ((index = get_IndexI(musicians, x)) == -1)
                {
                  musicians.push_back(x);
                  count.push_back(qcount - 1);
                }
                else
                {
                  count[index] += (qcount - 1);
                }
              }
            } while (RESULT_OK == mysqlx_next_result(result));
            mysqlx_result_free(result);
          }
        }
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }
    }
  }

  if (musicians.size() == 0)
  {
    std::cout << "Query Error." << std::endl;
  }
  else
  {
    str_query = "select * from Preformer where ID_musician = " + std::to_string(musicians[get_max(count)]) + ";\0";

    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) == NULL)
    {
      std::cout << "Query Error." << std::endl;
      return -1;
    }

    std::vector<Performer *> performers = initPerformers(result);
    std::cout << "the performer who collabed the most : " << performers[0]->getName() << std::endl;
    vector_Free(performers);
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(songs);


  return 0;
}

int q10()
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
  std::vector<std::string> genre;
  std::vector<int> count;
  mysqlx_row_t *row;
  int64_t x;

  std::string str_query = "SELECT * FROM Songs;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Song *> songs = initSongs(result);

  for (int i = 0; i < songs.size(); ++i)
  {
    int index;
    if ((index = get_Index(genre, songs[i]->getGenre())) == -1)
    {
      genre.push_back(songs[i]->getGenre());
      count.push_back(1);
    }
    else
    {
      count[index]++;
    }
  }

  std::cout << "the popular genre :" << genre[get_max(count)] << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(songs);

  return 0;
}

int q11(std::string start_Date, std::string end_Date)
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
  std::vector<std::string> tech;
  std::vector<int> count;
  mysqlx_row_t *row;
  int64_t x;

  std::string str_query = "select * from Songs where Date > '" + start_Date + "' AND Date < '" + end_Date + "';\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Song *> songs = initSongs(result);

  for (int i = 0; i < songs.size(); ++i)
  {
    int index;

    if ((index = get_Index(tech, songs[i]->getTech())) == -1)
    {
      tech.push_back(songs[i]->getTech());
      count.push_back(1);
    }
    else
    {
      count[index]++;
    }
  }

  std::cout << "technician who worked on the most number of Songs :" << tech[get_max(count)] << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(songs);

  return 0;
}

int q12()
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
  std::vector<std::string> genre;
  std::vector<int> count;
  mysqlx_row_t *row;
  int64_t x;

  std::string str_query = "select * from Albums where End_Date = (select min(End_Date) from albums);\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Album *> albums = initAlbums(result);

  std::cout << "The first album is :\t" << albums[0]->getName() << std::endl;

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(albums);

  return 0;
}

int q13()
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
  int64_t x;

  std::string str_query = "SELECT * FROM Songs;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Song *> songs = initSongs(result);
  std::vector<Song *> choosen;

  for (int i = 0; i < songs.size(); ++i)
  {
    int64_t value = 0;
    mysqlx_row_t *row;

    str_query = "select count(*) from Album_Songs where T_ID = " + std::to_string(songs[i]->getID()) + ";";

    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &value);

        if (value > 1)
        {
          choosen.push_back(songs[i]);
        }
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }
    }
  }

  if (choosen.size() == 0)
  {
    std::cout << "Query Error." << std::endl;
  }
  else
  {
    std::cout << "The songs that were in two or more :" << std::endl;
    for (int i = 0; i < choosen.size(); ++i)
    {
      std::cout << i << "\t" << *choosen[i] << std::endl;
    }
  }
  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(songs);
  vector_Free(choosen);

  return 0;
}

int q14()
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
  std::vector<std::string> tech;
  std::vector<std::string> choosen;

  int64_t x;

  std::string str_query = "SELECT * FROM Songs;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Song *> songs = initSongs(result);

  for (int i = 0; i < songs.size(); ++i)
  {
    if (get_Index(tech, songs[i]->getTech()) == -1)
    {
      tech.push_back(songs[i]->getTech());
    }
  }

  str_query = "SELECT * FROM Albums;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Album *> a = initAlbums(result);

  for (int i = 0; i < a.size(); ++i)
  {
    int albumLength = a[i]->getTracks();

    for (int j = 0; j < tech.size(); ++j)
    {
      int64_t value = 0;
      mysqlx_row_t *row;

      str_query = "select count(*) from Songs as a  join (select * from Album_Songs where a_ID = " + std::to_string(a[i]->getID()) + ")\
 as b on a.T_ID = b.T_ID where Tech = '" +
                  tech[j] + "';";
      query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

      if ((result = mysqlx_execute(query)) != NULL)
      {
        if ((row = mysqlx_row_fetch_one(result)) != NULL)
        {
          mysqlx_get_sint(row, 0, &value);

          if (value == albumLength)
          {
            if (get_Index(choosen, tech[j]) == -1)
            {
              choosen.push_back(tech[j]);
            }
          }
        }
      }
    }
  }

  if (choosen.size() == 0)
  {

    std::cout << "Query Error." << std::endl;
  }
  else
  {
    std::cout << "The technicians that worked on complete albums :" << std::endl;

    for (int i = 0; i < choosen.size(); ++i)
    {
      std::cout << choosen[i] << std::endl;
    }
  }

  mysqlx_free(result);
  mysqlx_session_close(session);
  vector_Free(a);
  vector_Free(songs);
  

  return 0;
}

int q15()
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
  int max = -1;
  int index = -1;

  std::string str_query = "SELECT * FROM Preformer;\0";

  query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

  if ((result = mysqlx_execute(query)) == NULL)
  {
    std::cout << "Query Error." << std::endl;
    return -1;
  }

  std::vector<Performer *> performers = initPerformers(result);

  mysqlx_row_t *row;

  for (int i = 0; i < performers.size(); ++i)
  {
    int64_t value;

    str_query = "select count(*) from Songs as a join (select * from Preformer_Songs where M_ID = " + std::to_string(performers[i]->getID()) + ") as b on a.T_ID = b.T_ID group by Genre;";

    query = mysqlx_sql_new(session, str_query.c_str(), MYSQLX_NULL_TERMINATED);

    if ((result = mysqlx_execute(query)) != NULL)
    {
      if ((row = mysqlx_row_fetch_one(result)) != NULL)
      {
        mysqlx_get_sint(row, 0, &value);

        if (value > max)
        {
          index = i;
          max = value;
        }
      }
      else
      {
        std::cout << "Query Error." << std::endl;
      }
    }
  }

  if (index == -1 || max == -1)
    std::cout << "Query Error." << std::endl;
  else
    std::cout << "The Preformer with the most diverse musical genres is :" << performers[index]->getName() << std::endl;

  mysqlx_session_close(session);
  mysqlx_free(result);
  vector_Free(performers);
  return 0;
}