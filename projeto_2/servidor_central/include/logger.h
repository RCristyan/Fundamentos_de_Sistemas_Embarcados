#ifndef __LOGGER__
#define __LOGGER__

void createLogFile();
void appendToLogFile(const char* content);
void closeLogFile();

#endif