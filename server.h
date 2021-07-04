#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <chrono>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;
using namespace std::filesystem;

#ifndef SCANDIRSERVER_SERVER_H
#define SCANDIRSERVER_SERVER_H

class server {

    string scan_dir(const string& dir);
public:
    void run();
};


#endif //SCANDIRSERVER_SERVER_H
