#include <stdio.h>
#include <string.h>
#include <regex.h>

/* interfaces */
#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Authentication_status signUp(char *, char *);
Authentication_status login(char *, char *);
static Authentication_status sanitizeUser(char *);
static Authentication_status sanitizePassword(char *);
static Authentication_status saveNewUserAndPassword(FILE *,char *, char *);
static Authentication_status checkLogin(FILE *, char *, char *);
static int countMaxUsers(FILE *);

/********************/
/* PUBLIC FUNCTIONS */
/********************/

Authentication_status
signUp(char *user, char *password)
{
	Authentication_status status;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "rw");

	status = sanitizeUser(user);
	status = sanitizePassword(password);
	if (countMaxUsers < 11 && status == AU_SIGN_UP_OK)
		status = saveNewUserAndPassword(login_file, user, password);
	else
		return AU_MAX_USERS_REACHED;
	
	fclose(login_file);

	return status;
}

Authentication_status
login(char *user, char *password)
{
	Authentication_status status;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "r");

	status = sanitizeUser(user);
	status = sanitizePassword(password);
	status = checkLogin(login_file, user, password);

	return status;
}

static Authentication_status
sanitizeUser(char *user)
{
	regex_t regex;

	/* test length of user and password */
	if (strlen(user) < MIN_LEN_USER || strlen(user) > MAX_LEN_USER)
		return AU_SIGN_UP_ERROR;

	/* compile regex */
	if (regcomp(&regex, "[A-Z]", REG_EXTENDED))
		return AU_ERROR;

	/* execute regular expression */
	if (regexec(&regex, user, 0, NULL, 0) != 0)
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}

static Authentication_status
sanitizePassword(char *password)
{
	regex_t regex;

	/* test length of user and password */
	if (strlen(password) < MIN_LEN_PW || strlen(password) > MAX_LEN_PW)
		return AU_SIGN_UP_ERROR;

	/* compile regular expression */
	if (regcomp(&regex, "[A-Z]", REG_EXTENDED))
		return AU_ERROR;
	
	/* execute regular expression */
	if (regexec(&regex, password, 0, NULL, 0) != 0)
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}

static Authentication_status
saveNewUserAndPassword(FILE *login_file, char *user, char *password)
{
	login_file = fopen("shadow", "a");
	if (!login_file)
		return AU_ERROR;	

	fprintf(login_file, "%s:%s:", user, password);

	return AU_SIGN_UP_OK;
}

static Authentication_status
checkLogin(FILE *login_file, char *user, char *password)
{
	char buffer[300];
	char *token;

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;

	token = strtok(buffer, ":");

	while (token) {
		if (!strcmp(user, token)) {
			token = strtok(NULL, ":");
			if (!strcmp(password, token))
				return AU_AUTHENTICATION_OK;
		}
		token = strtok(NULL, ":");
	}
	return AU_AUTHENTICATION_ERROR;	
}

static int
countMaxUsers(FILE *login_file)
{
	int count;
	char buffer[300];
	char *token;

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;

	token = strtok(buffer, ":");
	count = 1;
	while (token) {
		token = strtok(NULL, ":");
		count++;
	}
	return count;
}
