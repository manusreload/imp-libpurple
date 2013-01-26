#include <libpurple/status.h>

#include "impconnection.h"

char * md5(char * data) {
    unsigned char sig[MD5_SIZE];
    char * str = g_new0(char, 33);
    md5_buffer(data, strlen(data), sig);
    md5_sig_to_string(sig, str, 33);
    return str;
}

void imparty_client_login(IMPConnection * conn) {
    IMPMessage * message = impmessage_allocate(MESSAGE_LOGIN);
    impmessage_add_string(message, "email", conn->account->username);
    impmessage_add_string(message, "pass-secure", md5(conn->account->password));
    imparty_send_message(conn, message);
}

void imparty_client_friends(IMPConnection * conn) {
    IMPMessage * message = impmessage_allocate(MESSAGE_FRIEND_LIST);
    imparty_send_message(conn, message);
}

int imparty_client_send_im(IMPConnection * conn, char * to, char * chat) {
    IMPMessage * message = impmessage_allocate(MESSAGE_SEND_CHAT);
    char * result = g_new0(char *, strlen(chat) + 40);
    sprintf(result, "{\"type\":\"string\",\"data\":\"%s\"}", chat);
    impmessage_add_string(message, "email", to);
    impmessage_add_string(message, "message", result);
    return imparty_send_message(conn, message);
}

void imparty_client_send_im_recv(IMPConnection * conn, long msgid, const char * who) {
    purple_debug_warning("imparty", "who %s\n", who);
    IMPMessage * message = impmessage_allocate(MESSAGE_RECV_CHAT);
    impmessage_add_long(message, "mid", msgid);
    impmessage_add_string(message, "email", who);
    imparty_send_message(conn, message);
}

static int imparty_find_key(json_value * json, char * key) {
    int i, count = json->u.object.length;
    for (i = 0; i < count; i++) {
        if (strcmp(json->u.object.values[i].name, key) == 0) {
            return i;
        }
    }
    return -1;
}
void imparty_buddy_free(char * friend)
{
    
}
static int imparty_message_receiver(IMPConnection * conn, IMPMessage * message) {
    if (message == NULL) {
        purple_debug_warning("imparty", "null message!\n");
        return 0;
    }
    purple_debug_info("imparty", "new message: %d\n", message->message_type);
    char * result;
    if (message->message_type == MESSAGE_LOGIN) {
        result = impmessage_get_string(message, "result");
        purple_debug_info("imparty", "login result: %s\n", result);

        if (strcmp(result, "succeed") == 0) {
            //Ready to chat! :)
            purple_connection_set_state(conn->pc, PURPLE_CONNECTED);
            //retrieve friends... :P
            imparty_client_friends(conn);
        } else {
            purple_connection_error(conn->pc, impmessage_get_string(message, "message"));

        }
    } else if (message->message_type == MESSAGE_FRIEND_LIST) {
        //conn->friends = g_new0(GList, 1);
        if (impmessage_has_key(message, "count")) {
            char * icon_data = imparty_post_or_get("3.bp.blogspot.com", "/-z1NE5AvS8eU/T2SmW3sB9GI/AAAAAAAAAC8/EQO9euJzyCc/s45/1303501660_Bigotes-Plantilla.jpg");

            int count = impmessage_get_integer(message, "count");
            purple_debug_info("imparty", "friends count: %d\n", count);
            int i = 0;
            PurpleGroup *friends;
            friends = purple_find_group("Amigos");
            if (!friends) {
                friends = purple_group_new("Amigos");
            }
            /*
                        PurpleGroup *groups;
                        groups = purple_find_group("Grupos");
                        if (!groups) {
                            groups = purple_group_new("Grupos");
                        }
             */
            char * key = g_new0(char, 100);
            conn->buddies = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, (GDestroyNotify)imparty_buddy_free);
            for (i = 0; i < count; i++) {
                IMPFriend * friend = g_new0(IMPFriend, 1);
                sprintf(key, "email%d", i);
                friend->imp_id = impmessage_get_string(message, key);
                sprintf(key, "avatar%d", i);
                friend->avatar = impmessage_get_string(message, key);
                sprintf(key, "name%d", i);
                friend->name = purple_utf8_try_convert(impmessage_get_string(message, key));
                sprintf(key, "status%d", i);
                friend->status = impmessage_get_string(message, key);
                sprintf(key, "online%d", i);
                friend->online = impmessage_get_boolean(message, key);
                friend->conn = conn;
                PurpleBuddy *buddy;
                buddy = purple_find_buddy(conn->account, friend->imp_id);
                if (!buddy) {
                    buddy = purple_buddy_new(conn->account, friend->imp_id, friend->name);
                    purple_blist_add_buddy(buddy, NULL, friends, NULL);
                }
                g_hash_table_insert(conn->buddies, buddy->name, g_strdup(friend->status));
                buddy->proto_data = friend;
                PurplePresence *presence = purple_buddy_get_presence(buddy);
                impmessage_get_boolean(message, key);
                purple_debug_info("imparty", "online: %s %d\n", friend->imp_id, friend->online);
                if(presence != NULL)
                {
                    presence = purple_presence_new_for_buddy(buddy);
                }
                if(friend->online == 1)
                {
                    purple_presence_set_status_active(presence, "online", true );
                }
                else
                {
                    purple_presence_set_status_active(presence, "online", false );
                    purple_presence_set_status_active(presence, "offline", true );
                }
                buddy->presence = presence;
/*
                PurpleStatus * status;
                status = purple_presence_get_status(presence, "imparty");

                if (status == NULL) {
                    presence = purple_presence_new_for_buddy(buddy);
                    PurpleStatusType *type;
                    type = purple_status_type_new_with_attrs(PURPLE_STATUS_AVAILABLE,
                            "online", NULL, TRUE, TRUE, FALSE,
                            "message", ("Message"), purple_value_new(PURPLE_TYPE_STRING),
                            NULL);
                    status = purple_status_new(type, presence);
                    purple_status_set_active(status, true);

                    //purple_status_set_attr_string(status, "online", friend->status);
                    buddy->presence = presence;
                    //purple_status_set_attr_string(status, "message", friend->status);
                    //purple_presence_add_status(presence, status);
                }
*/
                //

                purple_util_fetch_url_len(friend->avatar, TRUE, NULL, TRUE,
                        1024 * 1024,
                        do_buddy_avatar_update_fromurl, friend);
            }
            purple_debug_info("imparty", "table size: %d\n", g_hash_table_size(conn->buddies));
            
        }
        else
        {
            purple_debug_info("imparty", "No count\n");
        }
    } else if (message->message_type == MESSAGE_RECV_CHAT) {
        char * who = impmessage_get_string(message, "email");
        char * chatData = impmessage_get_string(message, "message");
        //purple_debug_info("imparty", "raw msg %s\n", chatData);
        char * chat = "";
        long mtime = impmessage_get_long(message, "date");
        long mid = impmessage_get_long(message, "mid");
        if (who != NULL) {
            json_value * json = json_parse(chatData);
            int msg_flag = PURPLE_MESSAGE_RECV;
            if (json != NULL && json->u.object.length == 2) {

                int typeKey = imparty_find_key(json, "type");
                int valueKey = imparty_find_key(json, "data");
                if (strcmp(json->u.object.values[typeKey].value->u.string.ptr, "string") == 0) {
                    chat = json->u.object.values[valueKey].value->u.string.ptr;
                } else if (strcmp(json->u.object.values[typeKey].value, "photo") == 0) {
                    char genMsg = "Archivo multimedia: %s";
                    sprintf(chat, genMsg, json->u.object.values[valueKey].value->u.string.ptr);
                    msg_flag = PURPLE_MESSAGE_IMAGES;
                } else {
                    msg_flag = PURPLE_MESSAGE_ERROR;
                }

            } else {
                chat = chatData;
            }
            //purple_debug_info("imparty", "recv chat: %s, from %s, at %d\n", chat, who, mtime);
            serv_got_im(conn->pc, who, chat, msg_flag, time(NULL));
            imparty_client_send_im_recv(conn, mid, who);
        }
        //purple_signal_emit(purple_connections_get_handle(), "imparty-message-recv", conn, who, chat, mtime);
    }
    return true;

}

static void
do_buddy_avatar_update_fromurl(PurpleUtilFetchUrlData *url_data,
        gpointer user_data, const gchar *url_text,
        gsize len, const gchar *error_message) {
    IMPFriend *info = user_data;
    gpointer icon_data;

    if (!url_text) {
        purple_debug_error("jabber",
                "do_buddy_avatar_update_fromurl got error \"%s\"",
                error_message);
        goto out;
    }

    icon_data = g_memdup(url_text, len);
    char * checksum = purple_util_get_image_checksum(icon_data, strlen(icon_data));
    purple_buddy_icons_set_for_user(info->conn->account, info->imp_id, icon_data, len, icon_data);

out:
    g_free(info);
}

void * imp_handler(void* arg) {
    if (!arg) return NULL;
    IMPConnection * conn = (IMPConnection *) arg;
    conn->proto = improtocol_allocate();
    conn->processing = false;

    purple_debug_info("imparty", "socket connected: %d\n", conn->connected);
    if (conn->connected == 1) {
        int bufferLen = 1024;
        int leng = 0;
        char buffer[bufferLen];
        time_t start = time(NULL);
        while (((leng = recv(conn->sockfd, buffer, sizeof (buffer), 0)) > 0)) {
            start = 0;
            while(conn->processing==true)
            {
                if((time(NULL) - start) > 5)
                {
                    purple_debug_info("imparty", "processing time out\n");
                    conn->processing = false;
                }
            }
            improtocol_add_byte_raw(conn->proto, buffer, leng);
            
            if (conn->processing == false) {
                purple_timeout_add_seconds(0, (GSourceFunc) imparty_eventloop, conn->pc);
            } else {
                purple_debug_info("imparty", "ready processing 2\n");
            }
        }
        purple_timeout_add_seconds(0, (GSourceFunc) imparty_eventloop, conn->pc);
    }
    return NULL;
}

int imparty_send_raw(IMPConnection * conn, char * buffer, int leng) {

    return send(conn->sockfd, buffer, leng, 0);
}

int imparty_send_message(IMPConnection * conn, IMPMessage * message) {
    IMProtocol * proto = impmessage_save_protocol(message);
#ifdef IMPCONNECTION_INPECT_SEND
    improtocol_inspect(proto);
#endif
    int res = imparty_send_raw(conn, proto->m_raw, proto->length);
    //g_free(proto);
    return res;
}

void test(IMPConnection * conn) {
#ifdef DEBUG_TEST
    purple_debug_info("imparty", "Running some test...\n");

    purple_debug_info("imparty", "========PROTOCOL TEST=============\n");
    IMProtocol * foo_proto = improtocol_allocate();
    ;
    purple_debug_info("imparty", "test EOP: %d\n", improtocol_eop(foo_proto));
    foo_proto->read_pos = 0;
    improtocol_add_string(foo_proto, "long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string long string ");
    char * _foo_str;
    purple_debug_info("imparty", "test EOP: %d\n", improtocol_eop(foo_proto));

    _foo_str = improtocol_get_string(foo_proto);
    purple_debug_info("imparty", "\tfoo_proto string %s\n", _foo_str);



    purple_debug_info("imparty", "======== MESSAGE + PROTOCOL TEST=============\n");
    IMPMessage * m;
    m = g_new0(IMPMessage, 1);
    m->message_type = 10;
    impmessage_add_string(m, "key1", "hola, para usted mi cola");
    impmessage_add_string(m, "key2", ":D");
    impmessage_add_short(m, "short1", 155);
    impmessage_add_integer(m, "integer1", 5842);

    IMProtocol * p = impmessage_save_protocol(m);
    purple_debug_info("imparty", "proto len: %d\n", p->length);

    purple_debug_info("imparty", "proto read init: %d\n", p->read_pos);
    IMPMessage * m2;
    m2 = g_new0(IMPMessage, 1);
    impmessage_load_protocol(m2, p);
    purple_debug_info("imparty", "proto read end: %d\n", p->read_pos);
    purple_debug_info("imparty", "new message type: %d\n", m2->message_type);
    purple_debug_info("imparty", "new message keys: %d\n", m2->values_count);
    int i = 0;
    for (i = 0; i < m2->values_count; i++) {
        purple_debug_info("imparty", "\tkey { %d } %s\n", i, m2->values[i]->key);
        purple_debug_info("imparty", "\t\t¿key { %d } %s==key1? %d\n", i, m2->values[i]->key, strcmp(m2->values[i]->key, "key1"));
    }
    char * foo;

    purple_debug_info("imparty", "key: %d\n", impmessage_get_value(m2, "key1"));
    purple_debug_info("imparty", "value: %d\n", impmessage_get_value(m2, "key1"));
    purple_debug_info("imparty", "yes: %d\n", impmessage_has_key(m2, "key1"));
    purple_debug_info("imparty", "no: %d\n", impmessage_has_key(m2, "key2"));

    if (impmessage_has_key(m2, "key1") == true) {

        foo = impmessage_get_string(m2, "key1");
        purple_debug_info("imparty", "test1: %s\n", foo);
        purple_debug_info("imparty", "test short: %d\n", impmessage_get_short(m2, "short1"));
        purple_debug_info("imparty", "test int: %d\n", impmessage_get_integer(m2, "integer1"));
    } else {
        purple_debug_info("imparty", "no key!\n");

    }

    purple_debug_info("imparty", "======== MD5 TEST =============\n");


    purple_debug_info("imparty", "md5(''): %s\n", md5(""));

    purple_debug_info("imparty", "======== CLIENT TEST =============\n");

    IMPMessage * login = g_new0(IMPMessage, 1);
    login->message_type = MESSAGE_LOGIN;
    imparty_send_message(conn, login);
    g_free(m);
    purple_debug_info("imparty", "End of some test...\n");
#endif
}

void imparty_close_connection(PurpleConnection * pc) {
    IMPConnection * conn = (IMPConnection *) pc->proto_data;
    if (conn != NULL) {
        conn->connected = false;
        if (conn->sockfd != NULL)
            close(conn->sockfd);
    }
    //purple_timeout_remove(conn->handler);
}

void imparty_connect(PurpleAccount* pa) {


    purple_debug_info("imparty", "starting connection...\n");
    PurpleConnection *pc;
    IMPConnection * conn = g_new0(IMPConnection, 1);
    pc = purple_account_get_connection(pa);
    purple_connection_set_state(pc, PURPLE_CONNECTING);
    conn->account = pa;
    conn->pc = pc;

    conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (conn->sockfd < 0) {
        purple_connection_error(conn->pc, "Could not create socket");
        return;
    }
    conn->host = gethostbyname(SERVER_ADDR);
    if (conn->host == NULL) {
        purple_connection_error(conn->pc, "Could not resolve host");
        return;
    }
    memset(&conn->serv_addr, 0, sizeof (conn->serv_addr));
    conn->serv_addr.sin_family = AF_INET;
    conn->serv_addr.sin_port = htons(SERVER_PORT);
    conn->serv_addr.sin_addr.s_addr = *((unsigned long *) conn->host->h_addr_list[0]);

    if (connect(conn->sockfd, (struct sockaddr *) &conn->serv_addr, sizeof (conn->serv_addr)) < 0) {
        purple_connection_error(conn->pc, "Could not connect to remote host");
        return;
    }
    conn->connected = 1;
    conn->processing = false;
    pthread_create(&conn->thread, NULL, &imp_handler, conn);
    conn->acction = 0;
    purple_debug_info("imparty", "connected!\n");
    pa->gc->proto_data = conn;
    test(conn);
    register_handlers(conn);
    imparty_client_login(conn);
}

void register_handlers(IMPConnection * conn) {

}

void imparty_disconnection(PurpleConnection * pc) {

    purple_debug_info("imparty", "socket disconnecteed from server\n");
    purple_connection_error(pc, "Disconnected from server!");
    purple_connection_set_state(pc, PURPLE_DISCONNECTED);
}

gboolean
imparty_eventloop(gpointer userdata) {

    PurpleConnection *pc = (PurpleConnection *) userdata;
    if (pc == NULL) {
        purple_debug_info("imparty", "connection closed \n");
        return false;
    }
    IMPConnection *conn = (IMPConnection *) pc->proto_data;
    if (conn == NULL || conn->connected == false || conn->proto == NULL) {
        purple_debug_info("imparty", "connection closed 2s\n");
        //imparty_disconnection(pc);
        return false;
    }
    if (conn->processing == true) {
        purple_debug_info("imparty", "ready processing...\n");
        return false;
    }
    conn->processing = true;
    //purple_timeout_remove(conn->handler);
    IMPMessage * message;
    boolean aux = true;
    while (aux == true) {
        message = g_new0(IMPMessage, 1);
        if ((aux = impmessage_load_protocol(message, conn->proto)) == true) {
            //Do stuf...
            //purple_debug_info("imparty", "new message: %d\n", message->message_type);
            imparty_message_receiver(conn, message);
        }
        else
        {
                purple_debug_info("imparty", "EOP...\n");
        }
        g_free(message);
    }
    conn->processing = false;
    return false;
}


