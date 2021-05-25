#include "Server.h"
#include <ctime>
#include <cstdio>
#include <objbase.h>
using namespace std;

string Raw_GetDateTimeStr()
{
    time_t t = time(NULL);
    tm ts;
    localtime_s(&ts,&t);
    char buf[24]= {0};
    strftime(buf, 24, "%Y-%m-%d %H:%M:%S", &ts);
    return string(buf); 
}

string Raw_RandomGuid()
{
    GUID guid;
    CoCreateGuid(&guid);

    char dst[36] = {0};
    snprintf(dst, 36,
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2],
        guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);
    return string(dst);
}