/* This page contains a client program that can request a file from the server program
*  on the next page. The server responds by sending the whole file.
*/

#include <sys/types.h>
#include <string.h>                                                    //memset
#include <stdlib.h>                                                    //exit
#include <stdio.h>                                                     //printf
#include <Winsock2.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")                                      //winsock library

#define BUF_SIZE 4096                                                  //block transfer size

int main(int argc, char **argv){
   int c, s, bytes;
   char buf[BUF_SIZE];				                                   //buffer for incoming file
   struct hostent *h;                                                  //info about server
   struct sockaddr_in channel;                                         //holds IP address
   char *server_command, *itemName;
   int server_port, server_command_num, itemID, minBid, biddingPrice, bufcount;
   WSADATA wsaData;

   //Handle input errors
   char *usagemsg = "\n-- USAGE: client servername/hostname port command parameter --\n\n   command parameter(s):\n      post itemname minbid\n      list\n      bid itemID biddingprice\n";
   if(argc < 4 || argc > 6){
      fatal(usagemsg);
   }

   if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){                       //initialize Winsock
	   fatal("WSAStartup failed");
   }

   h = gethostbyname(argv[1]);                                         //look up host's IP address
   if (!h) fatal("gethostbyname failed");

   //Handle port input error
   if (sscanf(argv[2],"%d",&server_port) < 1 || server_port < 1024 || server_port > 65555){
		   fatal("port number must be between 1024 - 65555");
   }

   //Store command and parameters
   if (strcmp(argv[3],"post") == 0){
	  int i;
	  char postmsg[] = "\n -- USAGE parameter minBid must be an integer --\n";
	  char postmsg2[] = "\n -- USAGE parameter itemName must be alphanumeric and under 16 characters --\n";
	  char postmsg3[] = "\n -- USAGE parameter minBid must be larger than 0 and less than 1,000,000,000 --\n";
	  server_command_num = 1;

	  //Handle input errors for parameters
      if (argc < 6){
         fatal(usagemsg);
      }

      if (strlen(argv[4]) > 16){
			 fatal(postmsg2);
      }
	  for (i=0; i<strlen(argv[4]); i++){
         if (!isalnum(argv[4][i])){
			 fatal(postmsg2);
		 }
	  }
      itemName = argv[4];

      for (i=0; i<strlen(argv[5]); i++){
         if (!isdigit(argv[5][i])){
            fatal(postmsg);
         }
      }

      sscanf(argv[5],"%d",&minBid);

      if (minBid <= 0 || minBid >= 1000000000){
    	  fatal(postmsg3);
      }
   }
   else if (strcmp(argv[3],"list") == 0){
	  if (argc > 4){
	     fatal(usagemsg);
	  }
      server_command_num = 2;
   }
   else if (strcmp(argv[3],"bid") == 0){
	  int i;
	  char postmsg[] = "\n -- USAGE parameter itemID must be an integer --\n";
	  char postmsg2[] = "\n -- USAGE parameter biddingPrice must be an integer --\n";
	  char postmsg3[] = "\n -- USAGE parameter biddingPrice must be less than 1,000,000,000 --\n";
	  server_command_num = 3;

	  //Handle input errors for parameters
      if (argc < 6){
         fatal(usagemsg);
      }

	  for (i=0; i<strlen(argv[4]); i++){
         if (!isdigit(argv[4][i])){
            fatal(postmsg);
         }
      }
      for (i=0; i<strlen(argv[5]); i++){
         if (!isdigit(argv[5][i])){
            fatal(postmsg2);
         }
      }

      sscanf(argv[4],"%d",&itemID);
      sscanf(argv[5],"%d",&biddingPrice);

      if (biddingPrice >= 1000000000){
    	  fatal(postmsg3);
      }
   }
   else{
	   fatal(usagemsg);
   }

   s= socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (s < 0) fatal("socket");
   memset(&channel, 0, sizeof(channel));
   channel.sin_family = AF_INET;
   memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
   channel.sin_port = htons(server_port);

   c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
   if (c < 0) fatal("connect failed");

   //Connection is now established.

   //Go get the file and write it to standard output.
   while (1){
      //Receive and print connected message
	  do{
         memset(buf, 0, BUF_SIZE);
         bufcount = recv(s, buf, BUF_SIZE, 0);
	  } while (bufcount != BUF_SIZE);
      printf("\n%s\n",buf);

      //Clear buffer
      memset(buf, 0, BUF_SIZE);

      //Send client command(s) to server
      if (server_command_num == 1){
    	 //Send post command
         sprintf(buf,"%d^%s^%d",server_command_num,itemName,minBid);
         //printf("\nBuffer text: %s\n", buf);
         if (send(s, buf, BUF_SIZE, 0) == 0){
        	 fatal("send failed");
         }
      }
      else if (server_command_num == 2){
    	 //Send list command
         sprintf(buf,"%d",server_command_num);
         if (send(s, buf, BUF_SIZE, 0) == 0){
        	 fatal("send failed");
         }
      }
      else if (server_command_num == 3){
         //Send bid command
         sprintf(buf,"%d^%d^%d",server_command_num,itemID,biddingPrice);
         if (send(s, buf, BUF_SIZE, 0) == 0){
        	 fatal("send failed");
         }
      }

      //Return Status and Goodbye message
      memset(buf, 0, BUF_SIZE);
      recv(s, buf, BUF_SIZE, 0);
      printf("%s\n", buf);

      closesocket(s);
	  WSACleanup();
      exit(0);
   }
}

fatal(char *string){
   printf("%s\n", string);
   exit(1);
}
