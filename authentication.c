#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Authentication_status sign_up(char *, char *);
Authentication_status login(char *, char *);
Authentication_status remove_user(char *);
static Authentication_status saveNewUserAndPassword(char *, char *);
static Authentication_status checkLogin(char *, char *);
static int check_file(void);
static int sanitizeUser(char *);
static int sanitizePassword(char *);
static int countMaxUsers(void);

/********************/
/* PUBLIC FUNCTIONS */
/********************/

Authentication_status
sign_up(char *user, char *password)
{
	/* check if file exist */
	if (!check_file())
		if (system("touch shadow"))
			return AU_ERROR;

	/* sanitize user and password */
	if (!sanitizeUser(user))
		return AU_SIGN_UP_ERROR;
	if (!sanitizePassword(password))
		return AU_SIGN_UP_ERROR;

	if (countMaxUsers() > MAX_USERS)
		return AU_MAX_USERS_REACHED;
	if (checkLogin(user, password) == AU_AUTHENTICATION_OK)
		return AU_USER_EXIST;

	return saveNewUserAndPassword(user, password);
}

Authentication_status
login(char *user, char *password)
{
	/* sanitize user and password */
	if (!sanitizeUser(user))
		return AU_SIGN_UP_ERROR;
	if (!sanitizePassword(password))
		return AU_SIGN_UP_ERROR;

	return checkLogin(user, password);
}

Authentication_status
remove_user(char *user)
{
	char buffer[BUFFER_SIZE];
	char tmp_buffer[BUFFER_SIZE];
	char *find_user;
	char tmp_file_name[32] = "newshadow";
	FILE *login_file;
	FILE *tmp_file;

	login_file = fopen(LOGIN_FILE, "rw");
	tmp_file = fopen(tmp_file_name, "w");

	while (fgets(buffer, BUFFER_SIZE, login_file)) {
		strcpy(tmp_buffer, buffer);
		find_user = strtok(tmp_buffer, ":");
		if (strcmp(user, find_user))
			fprintf(tmp_file, "%s", buffer);
	}

	system("mv newshadow shadow");

	fclose(login_file);
	fclose(tmp_file);

	return AU_REMOVE_USER_OK;
}

/*******************/
/* LOCAL FUNCTIONS */
/*******************/

/************************* Save login or check login **************************/

static Authentication_status
saveNewUserAndPassword(char *user, char *password)
{
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "a+b");

	if (!login_file)
		return AU_ERROR;	

	fprintf(login_file, "%s:%s:\n", user, password);

	fclose(login_file);

	return AU_SIGN_UP_OK;
}

static Authentication_status
checkLogin(char *user, char *password)
{
	char buffer[BUFFER_SIZE];
	char *token;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "r");

	while (fgets(buffer, sizeof(buffer), login_file)) {
		token = strtok(buffer, ":");
		if (!strcmp(user, token)) {
			token = strtok(NULL, ":");
			if (!strcmp(password, token))
				return AU_AUTHENTICATION_OK;
		}
	}

	fclose(login_file);

	return AU_AUTHENTICATION_ERROR;	
}

/********************************* Create file ********************************/

static int
check_file(void)
{
	struct stat st;
	if (!stat(LOGIN_FILE, &st))
		return 1;
	else
		return 0;
}

/****************************** Sanitize Input *******************************/

static int
sanitizeUser(char *user)
{
	regex_t regex;
	char regex_string[32] = ".*[A-Z].*";

	/* test length of user */
	if (strlen(user) < MIN_LEN_USER || strlen(user) > MAX_LEN_USER)
		return 0;

	/* compile regex; return 0 on success */
	if (regcomp(&regex, regex_string, REG_EXTENDED))
		return 0;

	/* execute regular expression; return 0 on success */
	if (regexec(&regex, user, 0, NULL, 0) != 0)
		return 0;

	/* free regex */
	regfree(&regex);

	return 1;
}

static int
sanitizePassword(char *password)
{
	regex_t regex;
	char regex_string[32] = ".*[!@#$%&*].*";

	/* test length of password */
	if (strlen(password) < MIN_LEN_PW || strlen(password) > MAX_LEN_PW)
		return 0;

	/* compile regular expression */
	if (regcomp(&regex, regex_string, REG_EXTENDED))
		return 0;
	
	/* execute regular expression */
	if (regexec(&regex, password, 0, NULL, 0) != 0)
		return 0;

	/* free regex */
	regfree(&regex);

	return 1;
}


/******************************** MAX USERS ***********************************/

static int
countMaxUsers(void)
{
	FILE *login_file;
	int count;
	char buffer[1024];

	login_file = fopen(LOGIN_FILE, "r");

	count = 0;
	while (fgets(buffer, BUFFER_SIZE, login_file))
		count++;

	fclose(login_file);

	return count;
}
