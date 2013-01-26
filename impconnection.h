#ifndef CONNECTION_H
#define	CONNECTION_H


#include <libpurple/account.h>
#include "blist.h"
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "conversation.h"

#include "md5.c"
#include "message.c"
#include "protocol.c"
#include "impclient.c"
#include "http.c"
#include "json.c"

//#define DEBUG_TEST

//#define IMPCONNECTION_INPECT_SEND

#define SERVER_ADDR                     "imparty.no-ip.org"
//#define SERVER_ADDR                     "localhost"
#define SERVER_PORT                     6868

#undef g_new0
#define g_new0(struct_type, n_structs)          \
    ((struct_type *) g_malloc0 (((gsize) sizeof (struct_type)) * ((gsize) (n_structs))))
enum ACTIONS
{
    ACTION_RECV_MSG = 1
};
typedef struct {
    PurpleAccount *account;
    PurpleConnection *pc;
    //friends
    GHashTable *buddies;
    guint handler;
    int acction;
    //Connection details
    IMProtocol * proto;
    int sockfd;
    struct hostent *host;
    struct sockaddr_in serv_addr;
    pthread_t thread;
    gboolean connected;
    //internal
    int processing;

} IMPConnection;

typedef struct {
    char * imp_id;
    char * avatar;
    char * name;
    char * status;
    boolean online;
    IMPConnection * conn;
} IMPFriend;


void imparty_buddy_free(char * _friend);
void imparty_client_login(IMPConnection * conn);
void imparty_client_friends(IMPConnection * conn);
int imparty_client_send_im(IMPConnection * conn, char * to, char * chat);
void imparty_client_send_im_recv(IMPConnection * conn, long msgid,const char * who);
static int imparty_find_key(json_value * json, char * key);
static void
do_buddy_avatar_update_fromurl(PurpleUtilFetchUrlData *url_data,
                               gpointer user_data, const gchar *url_text,
                               gsize len, const gchar *error_message);

static int imparty_message_receiver(IMPConnection * conn, IMPMessage * message);
void * imp_handler(void* arg);
int imparty_send_raw(IMPConnection * conn, char * buffer, int leng);
int imparty_send_message(IMPConnection * conn, IMPMessage * message);
void test(IMPConnection * conn);
void imparty_connect(PurpleAccount* pa);
void imparty_close_connection(PurpleConnection * pc);
void imparty_disconnection(PurpleConnection * pc);
void register_handlers(IMPConnection * conn);
char * md5(char * data);
gboolean imparty_eventloop(gpointer userdata);
#endif	/* CONNECTION_H */
