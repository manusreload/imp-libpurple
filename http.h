/* 
 * File:   http.h
 * Author: manus
 *
 * Created on 24 de enero de 2013, 10:37
 */

#ifndef HTTP_H
#define	HTTP_H

#include <curl/curl.h>

#define USERAGENT "IMC-PIDGINv0.1"

#define BUFSIZ 1024

char * imparty_post_or_get(char * host, char * page);
int imparty_create_and_connect_socket(char * addr);

#endif	/* HTTP_H */

