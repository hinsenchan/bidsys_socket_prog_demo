#include <sys/types.h>				                                   //This is the server code
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>					                                   //memset
#include <stdlib.h>					                                   //exit
#include <stdio.h>					                                   //printf
#include <errno.h>					                                   //error

#define BUF_SIZE 4096				                                   //block transfer size
#define QUEUE_SIZE 10

struct itemList {
   int itemID;
   char *itemName;
   int minBid;
   int maxBid;
   struct itemList *next;
};

typedef struct itemList auctionList;

int PostItem(int *totalItems, auctionList **current, auctionList **head, char *userInputItemName, int *userInputMinBid, char **msg);
int ListItems(auctionList *head, char **msg);
int Bid(int *itemNumber, int *bidPrice, auctionList *current, auctionList *head, char **msg);

int main(int argc, char *argv[]){

   int s, b, l, fd, sa, bytes, on = 1;
   char *token, *tokentemp, buf[BUF_SIZE];					                       //buffer for outgoing file
   struct sockaddr_in channel;				                           //holds IP address
   int server_port = 22221;				                               //default port
   int server_command, status, bufcount;

   //Auction variables
   auctionList *current, *head = NULL;
   char *userInputItemName;
   int i, userInputMinBid, userInputItemID, userInputBidPrice, totalItems = 0;

   //Build address structure to bind to socket.
   memset(&channel, 0, sizeof(channel));                               //zero channel
   channel.sin_family = AF_INET;
   channel.sin_addr.s_addr = htonl(INADDR_ANY);
   channel.sin_port = htons(server_port);

   //Passive open. Wait for connection.
   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                      //create socket

   if (s < 0) fatal("socket failed");
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

   do{                                                                 //bind to an open port
      server_port++;
      channel.sin_port = htons(server_port);
      b = bind(s, (struct sockaddr *) & channel, sizeof(channel));
      if (b < 0 && errno != 98) fatal("bind failed");
   } while (b < 0);

   l = listen(s, QUEUE_SIZE);				                           //specify queue size
   if (l < 0) fatal("listen failed");

   printf("Listening on port: %d\n\n", htons(channel.sin_port));

   //Socket is now set up and bound. Wait for connection and process it.
   while (1) {
      sa = accept(s, 0, 0);				                               //block for connection request
      if (sa < 0) fatal("accept failed");

      while (1){
         //Welcome message
         char *msg;
         msg = malloc(BUF_SIZE);
         msg = "-- client connected --";
         memset(buf,0,BUF_SIZE);
         strncpy(buf,msg,strlen(msg));
         send(sa, buf, BUF_SIZE, 0);
         printf("%s\n", msg);

	 memset(buf,0,BUF_SIZE);
         
	 //Receive and execute client command	          
         bufcount = recv(sa, buf, BUF_SIZE, 0);
         //printf("\nBuffer text: %s\n",buf);
         token = strtok(buf,"^");
	 server_command = atoi(token);
         //printf("Server command: %d\n",server_command);

         if (server_command == 1){
        	//Capture item name and initial bid from client
        	token = strtok(NULL,"^");
        	userInputItemName = malloc(sizeof(char)+strlen(token));
            strncpy(userInputItemName, token, strlen(token));
            printf("item name: %s\n",userInputItemName);
            token = strtok(NULL,"^");
            userInputMinBid = atoi(token);
            printf("min bid: %d\n",userInputMinBid);

            //Post client item
            char *postMsg;
            postMsg = malloc(BUF_SIZE);

            if (PostItem(&totalItems, &current, &head, userInputItemName, &userInputMinBid, &postMsg) != 1){
               postMsg = "## post failed ##\n";
            }

            //Return status to client
            memset(buf, 0, BUF_SIZE);
            memcpy(buf,postMsg,strlen(postMsg));
            //sprintf(buf,"%s",postMsg);
            //printf("%s", buf);
         }
         else if (server_command == 2){
        	//Return status to client
            char *listMsg;
            listMsg = malloc(BUF_SIZE);

            if (totalItems == 0){
               listMsg = "## auction is empty ##\n";
            }
            else if (ListItems(head, &listMsg) != 1){
               listMsg = "## list failed ##\n";
            }

            memset(buf, 0, BUF_SIZE);
            memcpy(buf,listMsg,strlen(listMsg));
         }
         else if (server_command == 3){
            //Capture client itemID and bidding price
         	token = strtok(NULL,"^");
            userInputItemID = atoi(token);
        	token = strtok(NULL,"^");
            userInputBidPrice = atoi(token);

            //Bid client itemID
            char *bidMsg;
            bidMsg = malloc(BUF_SIZE);
            switch(Bid(&userInputItemID, &userInputBidPrice, current, head, &bidMsg)){
               case 0:
                  bidMsg = "## itemID does not exist in auction ##\n";
                  break;
               case -1:
            	  bidMsg = "## bidding price is lower than the current bid ##\n";
            	  break;
            }

            //Return status to client
            memset(buf, 0, BUF_SIZE);
            memcpy(buf,bidMsg,strlen(bidMsg));
         }
         //Return Status and Goodbye message
         msg = "-- client disconnected --\n";
         strcat(buf,msg);
         send(sa, buf, BUF_SIZE, 0);
         printf("%s\n", buf);

         break;
      }
      close(sa);					                                   //close connection
   }
   exit(0);
}

fatal(char *string){
   printf("%s\n", string);
   exit(1);
}

int PostItem(int *totalItems, auctionList **current, auctionList **head, char *userInputItemName, int *userInputMinBid, char **msg){
   int status = 1;
   *totalItems = *totalItems + 1;

   *current = (auctionList*)malloc(sizeof(auctionList));
   (*current)->itemID = *totalItems;
   (*current)->itemName = userInputItemName;
   (*current)->minBid = *userInputMinBid;
   (*current)->maxBid = 0;
   (*current)->next = *head;
   *head = *current;

   sprintf(*msg+strlen(*msg),"## new item posted ##\nItem Name: %s\nMin Bid: %d\n",userInputItemName,*userInputMinBid);
   return status;
}

int ListItems(auctionList *head, char **msg){
   int status = 1;
   auctionList *temp;
   temp = head;

   while(head) {
      sprintf(*msg+strlen(*msg),"itemID:%d itemName:%s minBid:%d maxBid:%d\n", head->itemID, head->itemName, head->minBid, head->maxBid);
      head = head->next ;
   }

   head = temp;
   return status;
}

int Bid(int *itemNumber, int *bidPrice, auctionList *current, auctionList *head, char **msg){
   int status = 1, match = 0;
   auctionList *temp;
   temp = head;

   while(head){
      if (head->itemID == *itemNumber){
    	 match++;
    	 if (head->maxBid < *bidPrice && head->minBid < *bidPrice){
            head->maxBid = *bidPrice;
            sprintf(*msg+strlen(*msg),"## new bid posted ##\nItemID: %d\nBidding price: %d\n",*itemNumber,*bidPrice);
            break;
    	 }
    	 else{
    	    status = -1;
    	    break;
    	 }
      }
      head = head->next;
   }

   if (match == 0){
	   status = 0;
   }

   head = temp;
   //sprintf(*msg+strlen(*msg),"## new bid posted ##\nItemID: %d\nBidding price: %d\n",*itemNumber,*bidPrice);
   return status;
}
