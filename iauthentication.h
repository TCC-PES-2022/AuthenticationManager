#ifndef IAUTHENTICATION_H
#define IAUTHENTICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/stat.h>
#include <gcrypt.h>

#define MIN_LEN_USER 8
#define MAX_LEN_USER 32
#define MIN_LEN_PW   8
#define MAX_LEN_PW   32
#define MAX_USERS    5
#define BUFFER_SIZE  256
#define ITERATIONS   6947
#define MAC_KEY      "24437c1cbc0ceb94e5d5b1bdae2706f"
#define SALT         "40fde96ec89576607327d51ef7fb81fa600593d15c1d6b9e525310e6"

#define LOGIN_FILE     "shadow"
#define TMP_LOGIN_FILE "newshadow"

/* Authentication system status return */
typedef enum authentication_status {
	AU_AUTHENTICATION_OK    = 0,
	AU_AUTHENTICATION_ERROR = 1,
	AU_SIGN_UP_OK           = 2,
	AU_SIGN_UP_ERROR        = 3,
	AU_REMOVE_USER_OK       = 4,
	AU_REMOVE_USER_ERROR    = 5,
	AU_USER_EXIST           = 6,
	AU_MAX_USERS_REACHED    = 7,
	AU_ERROR                = 99
} Au_status;

/********************/
/* EXTERN FUNCTIONS */
/********************/

Au_status signUp(const char *user, const char *password);
Au_status login(const char *user, const char *password);
Au_status removeUser(const char *user);

#ifdef __cplusplus
}
#endif

#endif
