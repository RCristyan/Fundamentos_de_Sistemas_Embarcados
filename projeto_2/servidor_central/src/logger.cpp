#include <fstream>
#include <string>
#include <ctime>

#include "logger.h"

using namespace std;

ofstream log_file;

void createLogFile(){
    log_file.open("./utils/log.txt", ofstream::out);
    log_file << "data e hora, comando\n";
}

void registerDateTime(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    log_file << ltm->tm_mday << "/" << ltm->tm_mon << "/" << 1900 + ltm->tm_year << " ";
    log_file << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << ", ";
}

void appendToLogFile(const char* content){
    registerDateTime();
    log_file << content << endl;
}

void closeLogFile(){
    log_file.close();
}
