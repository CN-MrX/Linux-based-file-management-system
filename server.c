#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <sys/stat.h>
#include <fcntl.h>

int get_cmd_type(char *cmd)
{
    if(!strcmp("ls",cmd))   return LS;
    if(!strcmp("quit",cmd)) return QUIT;
    if(!strcmp("pwd",cmd)) return PWD;
    if(strstr(cmd,"cd")!=NULL) return CD;
    if(strstr(cmd,"get")!=NULL) return GET;
    if(strstr(cmd,"put")!=NULL) return PUT;
    return 100;
}

char *getDesDir(char* cmsg)
{
    char *p;
    p=strtok(cmsg," ");
    p=strtok(NULL," ");
    return p;
}

void msg_handler(struct Msg msg, int fd)
{
    char dataBuf[1024]={0};
    char *file=NULL;
    int fdfile;

    printf("cmd:%s\n",msg.data);
    int ret=get_cmd_type(msg.data);

    switch(ret){
            case LS:
            case PWD:
                msg.type=0;
                FILE *r=popen(msg.data,"r");
                fread(msg.data,sizeof(msg.data),1,r);
                write(fd,&msg,sizeof(