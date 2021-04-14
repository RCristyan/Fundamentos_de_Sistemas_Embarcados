#ifndef __SERVER_TEMPERATURE_SOCKET__
#define __SERVER_TEMPERATURE_SOCKET__

void setupServer();
void waitRequest();
void setStopPrinting(bool value);
bool getStopPrinting();

#endif
