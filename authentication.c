#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Authentication_status sign_up(char *, char *);
Authentication_status login(char *, char *);
Authentication_status remove_user(char *);
static int check_file(void);
static Authentication_status sanitizeUser(char *);
static Authentication_status sanitizePassword(char *);
static Authentication_status saveNewUserAndPassword(char *, char *);
static Authentication_status checkLogin(char *, char *);
static Authentication_status user_exist(char *);
static int countMaxUsers(void);

/********************/
/* PUBLIC FUNCTIONS */
/********************/

Authentication_status
sign_up(char *user, char *password)
{
	Authentication_status status;

	/* check if file exist */
	if (!check_file())
		if (system("touch shadow"))
			return AU_ERROR;

	/* sanitize user and password */
	status = sanitizeUser(user);
	if (status != AU_SIGN_UP_OK)
		return status;
	status = sanitizePassword(password);
	if (status != AU_SIGN_UP_OK)
		return status;

	if (countMaxUsers() > 11)
		return AU_MAX_USERS_REACHED;
	if (user_exist(user) == AU_USER_EXIST)
		return AU_USER_EXIST;
	if (status == AU_SIGN_UP_ERROR)
		return AU_SIGN_UP_ERROR;

	status = saveNewUserAndPassword(user, password);
	
	return status;
}

Authentication_status
login(char *user, char *password)
{
	Authentication_status status;

	/* sanitize user and password */
	status = sanitizeUser(user);
	if (status != AU_SIGN_UP_OK)
		return status;
	status = sanitizePassword(password);
	if (status != AU_SIGN_UP_OK)
		return status;

	status = checkLogin(user, password);

	return status;
}

Authentication_status
remove_user(char *user)
{
	FILE *login_file;
	char *tmp_file_name = "newshadow";
	FILE *tmp_file;
	char *find_user;
	char buffer[1024];

	login_file = fopen(LOGIN_FILE, "rw");
	tmp_file = fopen(tmp_file_name, "w");

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;
	
	find_user = strstr(buffer, user);
	if (!find_user)
		return AU_REMOVE_USER_ERROR;

	buffer[strlen(buffer) - strlen(find_user)] = '\0';
	find_user = find_user + strlen(user) + 1;
	strcat(buffer, find_user);

	fprintf(login_file, "%s", buffer);
	fprintf(tmp_file, "%s", buffer);

	system("mv newshadow shadow");

	fclose(login_file);
	fclose(tmp_file);

	return AU_REMOVE_USER_OK;
}

/*******************/
/* LOCAL FUNCTIONS */
/*******************/

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

/********************************* Sanitize **********************************/

static Authentication_status
sanitizeUser(char *user)
{
	regex_t regex;

	char *regex_string = ".*[A-Z].*";

	/* test length of user and password */
	if (strlen(user) < MIN_LEN_USER || strlen(user) > MAX_LEN_USER)
		return AU_SIGN_UP_ERROR;

	/* compile regex */
	if (regcomp(&regex, regex_string, REG_EXTENDED))
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

	char *regex_string = ".*[!@#$%&*].*";

	/* test length of user and password */
	if (strlen(password) < MIN_LEN_PW || strlen(password) > MAX_LEN_PW)
		return AU_SIGN_UP_ERROR;

	/* compile regular expression */
	if (regcomp(&regex, regex_string, REG_EXTENDED))
		return AU_ERROR;
	
	/* execute regular expression */
	if (regexec(&regex, password, 0, NULL, 0) != 0)
		return AU_SIGN_UP_ERROR;

	/* free regex */
	regfree(&regex);

	/* sign up with success */
	return AU_SIGN_UP_OK;
}

/************************* Save data or check data ****************************/

static Authentication_status
saveNewUserAndPassword(char *user, char *password)
{
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "a+b");

	if (!login_file)
		return AU_ERROR;	

	fprintf(login_file, "%s:%s\n", user, password);

	fclose(login_file);

	return AU_SIGN_UP_OK;
}

static Authentication_status
checkLogin(char *user, char *password)
{
	FILE *login_file;
	char buffer[500];
	char *token;

	login_file = fopen(LOGIN_FILE, "r");

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;

	token = strtok(buffer, ":");

	while (token) {
		if (!strcmp(user, token)) {
			token = strtok(buffer, "\n");
			if (!strcmp(password, token))
				return AU_AUTHENTICATION_OK;
		}
		token = strtok(NULL, ":");
	}

	fclose(login_file);

	return AU_AUTHENTICATION_ERROR;	
}

static Authentication_status
user_exist(char *user)
{
	char buffer[BUFFER_SIZE];
	char *token;
	FILE *file;

	file = fopen(LOGIN_FILE, "r");

	fgets(buffer, BUFFER_SIZE, file);
	token = strtok(buffer, ":");
	if (!strcmp(user, token))
		return AU_USER_EXIST;
	while (fgets(buffer, BUFFER_SIZE, file)) {
		strtok(NULL, ":");
		if (!strcmp(user, token))
			return AU_USER_EXIST;
	}

	return AU_USER_DOES_NOT_EXIST;
}

static int
countMaxUsers(void)
{
	FILE *login_file;
	int count;
	char buffer[1024];
	char *token;

	login_file = fopen(LOGIN_FILE, "r");

	if (!fgets(buffer, sizeof(buffer), login_file))
		return AU_ERROR;

	token = strtok(buffer, ":");
	count = 1;
	while (token) {
		token = strtok(NULL, ":");
		count++;
	}

	fclose(login_file);

	return count;
}
