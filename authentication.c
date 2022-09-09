#include <stdio.h>
#include <string.h>
#include <regex.h>

/* interfaces */
#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Authentication_status signUP(char *, char *);
Authentication_status authentication(char *password);
static Authentication_status sanitizeUser(char *);
static Authentication_status sanitizePassword(char *);

/************************/
/* FUNCTION DEFINITIONS */
/************************/

Authentication_status
signUp(char *user, char *password)
{
	Authentication_status status;

	status = sanitizeUser(user);
	status = sanitizePassword(password);
	
	return status;
}

Authentication_status
authentication(char *password)
{
	Authentication_status status;

	status = sanitizePassword(password);

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
	if (!regexec(&regex_specialchar, password, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;
	if (regexec(&regex_capital, password, 0, NULL, 0))
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex_specialchar);
	regfree(&regex_capital);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}
