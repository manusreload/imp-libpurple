#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>


#include "http.h"

char * imparty_post_or_get(char * host, char * page) {
    int sock = imparty_create_and_connect_socket(host);
    if (sock != NULL) {
        char *get;
        char *tpl = "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
        get = (char *) malloc(strlen(host) + strlen(page) + strlen(USERAGENT) + strlen(tpl));
        sprintf(get, tpl, page, host, USERAGENT);
        purple_debug_info("imparty", "headers %s\n", get);
        //Send the query to the server
        int sent = 0;
        int tmpres;
        while (sent < strlen(get)) {
            tmpres = send(sock, get + sent, strlen(get) - sent, 0);
            if (tmpres == -1) {
                return NULL;
            }
            sent += tmpres;
        }
        char * buf;
        int htmlstart = 0;
        char * htmlcontent = "";
        char * result = "";
        char * aux = "";
        buf = g_new0(char, BUFSIZ);
        
        while ((tmpres = recv(sock, buf, sizeof(buf), 0)) > 0) {
            //purple_debug_info("imparty", "recv %s\n", buf);
            if (htmlstart == 0) {
                /* Under certain conditions this will not work.
                 * If the \r\n\r\n part is splitted into two messages
                 * it will fail to detect the beginning of HTML content
                 */
                htmlcontent = strcmp(buf, "\r\n\r\n");
                purple_debug_info("imparty", "\tnew started %d\n",htmlcontent);
                if (htmlcontent != NULL) {
                    htmlstart = 1;
                    //htmlcontent += 4;
                }
            } else {
                //htmlcontent = buf;
/*
                purple_debug_info("imparty", "allocated %d bytes\n", (strlen(result) + tmpres + 1) * sizeof(char*));
                result = realloc(result ,(strlen(result) + tmpres + 1) * sizeof(char*));
                strcat(result, buf);
*/
                
                aux = g_new0(char* ,strlen(result) + 1);
                //purple_debug_info("imparty", "-allocated %d bytes\n", (strlen(result)));
                strcpy(aux, result);
                result = g_new0(char* ,(strlen(result) + tmpres));
                //purple_debug_info("imparty", "allocated %d bytes\n", (strlen(result) + tmpres));
                strcat(result, aux);
                strcat(result, buf);
                //purple_debug_info("imparty", "\tnew content %s\n", result);
            }
        }
        return result;
    }
    return NULL;
}

int imparty_create_and_connect_socket(char * addr) {
    struct hostent *host;
    struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return NULL;
    }
    host = gethostbyname(addr);
    if (host == NULL) {
        return NULL;
    }
    memset(&serv_addr, 0, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    serv_addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        return NULL;
    }
    return sockfd;
}