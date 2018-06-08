#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
  
const int port = 8000;   //端口号为8000
const char* ip = "192.168.252.128";    //服务器的ip地址为192.168.252.128
  
int main()  
{  
      //创建套接字,socket是一个编程调用接口，属于传输层
      int clt_sock = socket(AF_INET, SOCK_STREAM, 0);  //AF_INET:协议族，决定使用的是ipv4地址与端口号的组合
      if(clt_sock < 0)                       //SOCK_STREAM:提供有序的可靠的、双向的、基于连接的字节流,为Internet地址族使用TCP
      {                                   //protocol：套接口所用的协议，=0,表示使用缺省的连接方式，自动选择第二个参数类型的默认协议
          //创建失败  
          perror("socket");                  //socket成功返回非负值，出错返回-1.perror函数会返回socket函数出现的错误
          return 1;  
      }  
  
      //绑定信息，即命名socket   
      struct sockaddr_in addr;    //sockaddr_in是一个结构体，包含在库文件netinet/in.h里，用于处理网络通信的地址
      addr.sin_family = AF_INET;   //地址族，指定为AF_INET
      addr.sin_port = htons(port);   //使用htons()函数将TCP端口号转换为网络数据格式 
      addr.sin_addr.s_addr = inet_addr(ip);//inet_addr函数将用点分十进制字符串表示的IPv4地址转化为用网络字节序整数表示的IPv4地址     
		 //sin_addr是结构体in_addr定义的，用来存放32位ipv4地址   

      //客户端不需要监听  
  
      //发起连接  
      socklen_t addr_len = sizeof(addr);  //socklen_t和int有相同的尺寸大小，相当于int,使用sizeof函数得到addr变量的大小
	  //使用connect函数创建与指定外部端口的连接，将socket连接到addr指定的网络地址，调用失败返回-1
      int connect_fd = connect(clt_sock, (struct sockaddr*)&addr, addr_len);  
      if(connect_fd < 0)  
      {  
          perror("connect");  
          return 2;  
      }   //连接没有成功就直接跳出程序

      char buf[1024];  //缓冲区大小为1024
  
      while(1)     //连接建立成功
      {  
          memset(buf, '\0', sizeof(buf));   //每次循环要把从前的字符清掉
          printf("client, please enter your word: ");  
          fflush(stdout);  //刷新标准输出缓冲区，把输出缓冲区的东西显示到屏幕上
          ssize_t size = read(0, buf, sizeof(buf) - 1);  //把终端输入的读到缓存区
          if(size > 0)  
          {  
              buf[size - 1] = '\0';  
          }  
          else if(size == 0)  
          {  
              printf("Finish...\n");  
              break;  
          }  
          else  
          {  
              perror("read");  
              return 4;  
          }  
          write(clt_sock, buf, strlen(buf));  // write函数将进程缓冲区中的数据向socket的发送缓冲区进行拷贝
          size = read(clt_sock, buf, sizeof(buf));  //读对方发来的数据
          if(size > 0)  
          {  
              buf[size] = '\0';  
          }  
          else if(size == 0)  
          {  
              printf("Finish...\n");  
              break;  
          }  
          else   
          {  
              perror("read");  
              return 5;  
          }  
          printf("The information from server is: %s\n", buf);  
       }  
      close(clt_sock);  
      return 0;  
}  
