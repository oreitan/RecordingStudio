MYSQL_CONCPP_DIR = C:/Program Files/MySQL/Connector C++ 8.0
CPPFLAGS = -I "$(MYSQL_CONCPP_DIR)/include" -L "$(MYSQL_CONCPP_DIR)/lib64" -L "$(MYSQL_CONCPP_DIR)/lib64/vs14"
LDLIBS = -lmysqlcppconn8
CXX = clang++ -stdlib=libc++

main : main.cpp db.cpp instrument.cpp producer.cpp song.cpp album.cpp 