#include <stdio.h>
#include <string.h>       //strlen
#include <sys/socket.h>
#include <arpa/inet.h>    //inet_addr
#include <unistd.h>       //write
#include "oid_api.h"
#include "net_api.h"      // NET_IF_LINK_NAME_TYPES
#include "net_link_api.h" // start_ms_timer()
#include "net_conf_api.h" // WIFI_DEV_DRIVER
#include "net_wifi_api.h"
#include "net_wifi_vars.h"
#include "os_system.h"
#include "net_debug.h"    // DBG_ macro family
#include "db_api.h"
#include "PaperCut_NW_Reader.h"
#define PAPERCUT_STACK_SIZE    POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 PaperCutStack[PAPERCUT_STACK_SIZE];
static int          PapercutPort = 0;
static int          client_sock  = -1;
int authenticateCardID(char *papercut_code);

int sendResponse(int sock, char *buf, int len)
{
   int err = -1;
   if(sock < 0 || len <= 0 || buf == NULL)
      return err;
   pthread_mutex_lock(&papercut_mutex);
   err = send(sock, buf, len, MSG_NOSIGNAL);
   pthread_mutex_unlock(&papercut_mutex);
   return err;
}

void *PaperCutInit(void *unused)
{
   int                socket_desc, c, read_size;
   struct sockaddr_in server, client;
   char               client_message[201];
   //Create socket
   fflush(stdout);
   printf("\nCard reader server");
   socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1)
   {
      printf("\nCould not create socket..Exiting");
   }
   printf("\nSocket created\n");
   //Prepare the sockaddr_in structure
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PapercutPort);
   int reuse_addr = 1;
   setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr, sizeof(reuse_addr));
   //Bind
   if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      //print the error message
      printf("\nbind failed. Error");
      close(socket_desc);
      return(0);
   }
   // printf("bind done\n");
   //Listen
   listen(socket_desc, 3);
   //Accept and incoming connection
   printf("Waiting for incoming connections...\n");
   c = sizeof(struct sockaddr_in);
   while (1)
   {
      printf("\n[PaperCut] Waiting for new connections");
      //accept connection from an incoming client
      pthread_mutex_lock(&papercut_mutex);
      client_sock = -1;
      pthread_mutex_unlock(&papercut_mutex);
      client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
      if (client_sock < 0)
      {
         
         printf("\n[PaperCut]accept failed");
         close(socket_desc);
         return(0);
      }
      printf("Connection accepted\n");
      //Receive a message from client
      char *t;
      memset(client_message, 0, sizeof(client_message));
      read_size = recv(client_sock, client_message, 201, 0);
      if (read_size == 0)
      {
         printf("\n[PaperCut]Client disconnected\n");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         continue;
      }
      else if (read_size == -1)
      {
         printf("\n[PaperCut]recv failed");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         //close(socket_desc);
         continue;
      }
      t = client_message;
      while (*t == '\r')
      {
         t++;
      }
      int l = strlen(client_message);
      while (client_message[l - 1] == '\r')
      {
         client_message[l - 1] = '\0';
         l--;
      }
      printf("\n[PaperCut]Message1 [%s]", t);
      fflush(stdout);
      memset(client_message, 0, sizeof(client_message));
      read_size = recv(client_sock, client_message, 201, 0);
      if (read_size == 0)
      {
         printf("\n[PaperCut]Client disconnected");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         continue;
      }
      else if (read_size == -1)
      {
         printf("\n[PaperCut]recv failed");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         //close(socket_desc);
         continue;
      }
      t = client_message;
      while (*t == '\r')
      {
         t++;
      }
      l = strlen(client_message);
      while (client_message[l - 1] == '\r')
      {
         client_message[l - 1] = '\0';
         l--;
      }
      printf("\n[PaperCut]Message2 [%s]\n", t);
      sendResponse(client_sock, "True", strlen("True"));
      fflush(stdout);
      static int i = 3;
      memset(client_message, 0, sizeof(client_message));
      while ((read_size = recv(client_sock, client_message, 201, 0)) > 0)
      {
         //Send the message back to client
         // write(client_sock , client_message , strlen(client_message));
         fflush(stdout);
         // printf("Before Trim Message[%d] [%s]\n", i, client_message);
         t = client_message;
         while (*t == '\r' || *t == '\n')
         {
            t++;
         }
         l = strlen(client_message);
         while (client_message[l - 1] == '\r' || client_message[l - 1] == '\n')
         {
            client_message[l - 1] = '\0';
            l--;
         }
         fflush(stdout);
         printf("\n[PaperCut]Message%d [%s]\n", i++, client_message);
         if ((client_message[0] == 'v') || (client_message[0] == 'V'))
         {
            sendResponse(client_sock, "1.0.0\r", strlen("1.0.0\r"));
         }
         else if ((client_message[0] == 'e') || (client_message[0] == 'E'))
         {
            sendResponse(client_sock, "No Error\r", strlen("No Error\r"));
         }
         else if ((client_message[0] == 'b') || (client_message[0] == 'B'))
         {
            sendResponse(client_sock, "expBeep\r", strlen("Beep\r"));
         }
         else if (client_message[0] == '\0')
         {
            continue;
         }
         memset(client_message, 0, sizeof(client_message));
      }
      if (read_size == 0)
      {
         printf("\n[PaperCut]Client disconnected\n");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         continue;
      }
      else if (read_size == -1)
      {
         printf("\n[PaperCut]recv failed");
         pthread_mutex_lock(&papercut_mutex);
         close(client_sock);
         client_sock = -1;
         pthread_mutex_unlock(&papercut_mutex);
         //close(socket_desc);
         continue;
      }
   }
   return(0);
}


void SysServicePaperCutInit(void)
{
   unsigned int     os_rcode = -1;
   static pthread_t paper_cut_thread;
   error_type_t     db_retval = FAIL;
   PapercutPort = db_get_var_int("CONF_SOLUTIONUP", "PaperCutPort", &db_retval);
   if (db_retval == FAIL)
   {
      DBG_ERR("Error open DB! (papercut) \n");
   }
   os_rcode = posix_create_thread(&paper_cut_thread, PaperCutInit, (void *)0, "paper_cut_thr",
                                  PaperCutStack, PAPERCUT_STACK_SIZE, POSIX_THR_PRI_NORMAL);
   if (os_rcode != OS_SUCCESS)
   {
      DBG_ERR("error creating thread %s (os_rcode=0x%02x)\n", "paper_cut_thr", os_rcode);
   }
   else
   {
      printf("[PaperCut]PaperCut creating thread SUCCESS\n");
   }
}


int authenticateCardID(char *papercut_code)
{
   if (papercut_code == NULL)
   {
      return FAIL;
   }
   pthread_mutex_lock(&papercut_mutex);
   if (client_sock == -1)
   {
      DBG_ERR("\n[Papercut] Client not connected");
      pthread_mutex_unlock(&papercut_mutex);
      return FAIL;
   }
   pthread_mutex_unlock(&papercut_mutex);
   if (papercut_code[0] != '\0')
   {
      printf("\n[PaperCut] Sending Card ID [%s] for Auth",papercut_code);
      sendResponse(client_sock, papercut_code, strlen(papercut_code));
      return OK;
   }
   else
   {
      printf("\n[PaperCut] Invalid card ID..Skip Auth");
      return FAIL;
   }
}
