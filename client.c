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


char *getdir(char* cmd)
{
    char *p;
    p=strtok(cmd," ");
    p=strtok(NULL," ");
    return p;
}

int get_cmd_type(char *cmd)
{
    if(!strcmp("ls",cmd))   return LS;
    if(!strcmp("quit",cmd)) return QUIT;
    if(!strcmp("pwd",cmd)) return PWD;
    if(strstr(cmd,"cd")!=NULL) return CD;
    if(strstr(cmd,"get")!=NULL) return GET;
    if(strstr(cmd,"put")!=NULL) return PUT;
	if(!strcmp("lls",cmd)) return LLS;
    return -1;
}


int cmd_handler(struct Msg msg, int fd)
{
    char *dir=NULL;
    char buf[32];
    int ret;
    int filefd;

    ret=get_cmd_type(msg.data);

    printf("cmd:%s\n",msg.data);
    ret=get_cmd_type(msg.data);

    switch(ret){
            case LS:
            case PWD:
            case CD:
                msg.type=0;
                write(fd,&msg,sizeof(msg));
                break;

            case GET:
                msg.type=2;
                write(fd,&msg,sizeof(msg));
                break;

            case PUT:
                strcpy(buf,msg.data);
                dir=getdir(buf);

                if(access(dir,F_OK)==-1){
                    printf("%s not exist\n",dir);
                }else{
                    filefd=open(dir,O_RDWR);
                    read(filefd,msg.secondBuf,sizeof(msg.secondBuf));
                    close(filefd);
                    write(fd,&msg,sizeof(msg));

                }
                    break;
            case LLS:
                system("ls");
                break;
            
            case LCD:
                dir=getdir(msg.data);
                chdir(dir);
                break;
            case QUIT:
                    strcpy(msg.data,"quit");
                    write(fd,&msg,sizeof(msg));
                    close(fd);
                    exit(-1);
    }
    return ret;
}

void handler_server_message(int c_fd,struct Msg msg)
{
    int n_read;
    struct Msg msgget;
    int newfilefd;

    n_read=read(c_fd,&msgget,sizeof(msgget));

    if(n_read==0){
        printf("server is out,quit\n");
        exit(-1);
    }else if(msgget.type==DOFILE){

        char *p=getdir(msg.data);
        newfilefd=open(p,O_RDWR|O_CREAT,0600);
        write(newfilefd,msgget.data,strlen(msgget.data));
        putchar('>');
        fflush(stdout);
    }else{
        printf("-----------------------\n");
        printf("\n%s\n",msgget.data);
        printf("-----------------------\n");

        putchar('>');
        fflush(stdout);



    }


}

int main(int argc, char const *argv[])
{
    int c_fd;
    int n_read;
    char readBuf[128];

    struct sockaddr_in c_addr;
    struct Msg msg;

    if(argc!=3){
            printf("not enough num\n");
            exit(-1);
    }

    memset(&c_addr,0,sizeof(struct sockaddr_in));
    
    c_fd=socket(AF_INET,SOCK_STREAM,0);
    if(c_fd==-1){
        perror("socket");
        exit(-1);
    }
    
    c_addr.sin_family=AF_INET;
    c_addr.sin_port=htons(atoi(argv[2]));
    inet_aton(argv[1],&c_addr.sin_addr);

    if(connect(c_fd,(struct sockaddr *)&c_addr,sizeof(struct sockaddr))==-1){
        perror("connect");
        exit(-1);
    }
    printf("connect ...\n");
    int mark=0;
    while(1)
    {
        memset(msg.data,0,sizeof(msg.data));
        if(mark==0) printf(">");

        gets(msg.data);

        if(strlen(msg.data)==0){
                if(mark==1){
                    printf(">");
                }
                continue;

        }
       
        mark=1;

        int ret=cmd_handler(msg,c_fd);

        if(ret>IFGO){
            putchar('>');
            fflush(stdout);
            continue;
        }
        if(ret==-1){
            printf("command not\n");
            printf(">");
            fflush(stdout);
            continue;


        }
        handler_server_message(c_fd,msg);
    }    

    return 0;
}

