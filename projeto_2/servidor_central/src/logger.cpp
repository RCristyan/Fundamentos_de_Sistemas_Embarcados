#include <fstream>
#include <string>

#include "logger.h"

using namespace std;

ofstream log_file;

void createLogFile(){
    log_file.open("./utils/log.txt", ofstream::out);
    log_file << "T,P,U\n";
}

void appendToLogFile(const char* content){
    log_file << content << endl;
}

void closeLogFile(){
    log_file.close();
}
