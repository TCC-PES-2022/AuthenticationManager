#ifndef IAUTHENTICATION_H
#define IAUTHENTICATION_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/stat.h>

#define MIN_LEN_USER 8
#define MAX_LEN_USER 16
#define MIN_LEN_PW   8
#define MAX_LEN_PW   16

#define LOGIN_FILE "shadow"

/* enumerations */
typedef enum authentication_system {
	AU_AUTHENTICATION_OK = 0,
	AU_AUTHENTICATION_ERROR = 1,
	AU_SIGN_UP_OK = 2,
	AU_SIGN_UP_ERROR = 3,
	AU_REMOVE_USER_OK = 4,
	AU_REMOVE_USER_ERROR = 5,
	AU_MAX_USERS_REACHED = 7,
	AU_PERMISSION_DENIED = 8,
	AU_ERROR = 9
} Authentication_status;

/* visible functions */
Authentication_status sign_up(char *, char *);
Authentication_status login(char *, char *);
Authentication_status remove_user(char *);

#endif
