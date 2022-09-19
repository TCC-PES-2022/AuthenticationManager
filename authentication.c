#include <stdio.h>
#include <string.h>
#include <regex.h>

/* interfaces */
#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Authentication_status signUp(char *, char *);
Authentication_status signIn(char *, char *);
static Authentication_status sanitizeUser(char *);
static Authentication_status sanitizePassword(char *);
static Authentication_status saveNewUserAndPassword(FILE *,char *, char *);
static Authentication_status checkLogin(FILE *, char *, char *);
static int countLinesShadowFile(FILE *);

/************************/
/* FUNCTION DEFINITIONS */
/************************/

Authentication_status
signUp(char *user, char *password)
{
	Authentication_status status;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "rw");

	status = sanitizeUser(user);
	status = sanitizePassword(password);
	if (countLinesShadowFile(login_file) < 5)
		status = saveNewUserAndPassword(login_file, user, password);
	else
		return AU_MAX_USERS_REACHED;
	
	fclose(login_file);

	return status;
}

Authentication_status
signIn(char *user, char *password)
{
	Authentication_status status;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "r");

	status = sanitizePassword(password);
	status = checkLogin(login_file, user, password);

	return status;
}

static Authentication_status
sanitizeUser(char *user)
{
	regex_t regex_specialchar, regex_capital;

	/* test length of user and password */
	if (strlen(user) < MIN_LEN_USER || strlen(user) > MAX_LEN_USER)
		return AU_SIGN_UP_ERROR;

	/* compile regex */
	if (regcomp(&regex_specialchar, "#@%*-=+$", REG_EXTENDED))
		return AU_ERROR;
	if (regcomp(&regex_capital, "[A-Z]", 0))
		return AU_ERROR;

	/* execute regular expression */
	if (regexec(&regex_specialchar, user, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;
	if (regexec(&regex_capital, user, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex_specialchar);
	regfree(&regex_capital);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}

static Authentication_status
sanitizePassword(char *password)
{
	regex_t regex_specialchar, regex_capital;

	/* test length of user and password */
	if (strlen(password) < MIN_LEN_PW || strlen(password) > MAX_LEN_PW)
		return AU_SIGN_UP_ERROR;

	/* compile regular expression */
	if (regcomp(&regex_specialchar, "#@%*-=+$", REG_EXTENDED))
		return AU_ERROR;
	if (regcomp(&regex_capital, "[A-Z]", 0))
		return AU_ERROR;
	
	/* execute regular expression */
	if (regexec(&regex_specialchar, password, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;
	if (regexec(&regex_capital, password, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex_specialchar);
	regfree(&regex_capital);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}

static Authentication_status
saveNewUserAndPassword(FILE *login_file, char *user, char *password)
{

	login_file = fopen("shadow", "a");
	if (!login_file)
		return AU_ERROR;	

	fprintf(login_file, "%s:%s\n", user, password);

	return AU_SIGN_UP_OK;
}

static Authentication_status
checkLogin(FILE *login_file, char *user, char *password)
{
	int count;
	char buffer[300];
	char *token_user;

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;

	token_user = strtok(buffer, ":");

	count = 0;
	while (1) {
		if (!strcmp(user, token_user))
			break;
		puts("hello");
		token_user = strtok(NULL, "\n");
		puts("hello");
		token_user = strtok(NULL, ":");

		if (count == 10)
			return AU_AUTHENTICATION_ERROR;
		count++;
	}
	/* unreachable */
	return AU_AUTHENTICATION_OK;	
}

static int
countLinesShadowFile(FILE *login_file)
{
	int lines;
	char c;

	for (c = getc(login_file); c != EOF; c = getc(login_file))
		if (c == '\n')
			lines++;

	return lines;
}
