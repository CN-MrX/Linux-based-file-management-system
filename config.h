#define LS 0
#define GET 1
#define PWD 2
#define IFGO 3
#define LCD 4
#define LLS 5
#define CD 6
#define PUT 7
#define QUIT 8
#define DOFILE 9

struct Msg{

    int type;
    char data[1024];
    char secondBuf[128];


};
