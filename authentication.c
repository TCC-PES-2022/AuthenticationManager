#include "iauthentication.h"

/*************************/
/* FUNCTION DECLARATIONS */
/*************************/

Au_status           signUp(const char *, const char *);
Au_status           login(const char *, const char *);
Au_status           removeUser(const char *);
static Au_status    saveNewUserAndPassword(const char *, const char *);
static Au_status    checkLogin(const char *, const char *);
static int          userExist(const char *);
static void         touchFile(void);
static void         substituteFile(void);
static int          sanitizeUser(const char *);
static int          sanitizePassword(const char *);
static void         encode(char *, const char *, unsigned int);
static unsigned int countMaxUsers(void);

/********************/
/* EXTERN FUNCTIONS */
/********************/

Au_status
signUp(const char *user, const char *password)
{
	touchFile();

	/* sanitize user and password */
	if (!sanitizeUser(user))
		return AU_SIGN_UP_ERROR;
	if (!sanitizePassword(password))
		return AU_SIGN_UP_ERROR;

	if (countMaxUsers() >= MAX_USERS)
		return AU_MAX_USERS_REACHED;

	if (userExist(user))
		return AU_USER_EXIST;

	/* save new user and password; final step */
	return saveNewUserAndPassword(user, password);
}

Au_status
login(const char *user, const char *password)
{
	touchFile();

	/* sanitize user and password */
	if (!sanitizeUser(user))
		return AU_AUTHENTICATION_ERROR;
	if (!sanitizePassword(password))
		return AU_AUTHENTICATION_ERROR;

	return checkLogin(user, password);
}

Au_status
removeUser(const char *user)
{
	int user_exist;
	char buffer[BUFFER_SIZE];
	char tmp_buffer[BUFFER_SIZE];
	char *find_user;
	FILE *login_file;
	FILE *tmp_file;

	touchFile();

	login_file = fopen(LOGIN_FILE, "rw");
	tmp_file = fopen(TMP_LOGIN_FILE, "w");

	user_exist = 0;

	/* Get users and copy to temporary file, if isn't the user to remove */
	/* If user exist, don't copy to temporary file and change flag value */
	while (fgets(buffer, BUFFER_SIZE, login_file)) {
		strcpy(tmp_buffer, buffer);
		find_user = strtok(tmp_buffer, ":");
		if (strcmp(user, find_user))
			fprintf(tmp_file, "%s", buffer);
		else
			user_exist = 1;
	}

	substituteFile();

	fclose(login_file);
	fclose(tmp_file);

	return (user_exist) ? AU_REMOVE_USER_OK : AU_REMOVE_USER_ERROR;
}

/********************/
/* STATIC FUNCTIONS */
/********************/

static Au_status
saveNewUserAndPassword(const char *user, const char *password)
{
	int i;
	unsigned int sha256_length;
	char string_encode[BUFFER_SIZE];
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "a+b");

	if (!login_file)
		return AU_ERROR;	
	
	/* get size of sha256 length */
	sha256_length = gcry_md_get_algo_dlen(GCRY_MD_SHA256);
	/* encode using HMAC sha256 */
	encode(string_encode, password, sha256_length);
	/* encode using HMAC sha256 - N iterations */
	for (i = 0; i < ITERATIONS; i++)
		encode(string_encode, string_encode, sha256_length);

	/* write on file */
	fprintf(login_file, "%s:%s:\n", user, string_encode);

	fclose(login_file);

	return AU_SIGN_UP_OK;
}

static Au_status
checkLogin(const char *user, const char *password)
{
	int i;
	unsigned int sha256_length;
	char string_encode[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	char *token;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "r");

	/* get size of sha256 length */
	sha256_length = gcry_md_get_algo_dlen(GCRY_MD_SHA256);
	/* encode using HMAC sha256 - first iteration */
	encode(string_encode, password, sha256_length);
	/* encode using HMAC sha256 - N iterations */
	for (i = 0; i < ITERATIONS; i++)
		encode(string_encode, string_encode, sha256_length);

	while (fgets(buffer, sizeof(buffer), login_file)) {
		token = strtok(buffer, ":");
		if (!strcmp(user, token)) {
			token = strtok(NULL, ":");
			if (!strcmp(string_encode, token))
				return AU_AUTHENTICATION_OK;
		}
	}

	fclose(login_file);

	return AU_AUTHENTICATION_ERROR;	
}

static int
userExist(const char *user)
{
	int user_exist;
	char buffer[BUFFER_SIZE];
	char tmp_buffer[BUFFER_SIZE];
	char *find_user;
	FILE *login_file;

	login_file = fopen(LOGIN_FILE, "r");

	user_exist = 0;

	/* Identify if user already exist */
	while (fgets(buffer, BUFFER_SIZE, login_file)) {
		strcpy(tmp_buffer, buffer);
		find_user = strtok(tmp_buffer, ":");
		if (strcmp(user, find_user))
			continue;
		else
			user_exist = 1;
	}

	fclose(login_file);

	return (user_exist) ? 1 : 0;
}

/************************** Create and substitute file ************************/

static void
touchFile(void)
{
	/* If file doesn't exist, create it */
	char touch_file[32];
	snprintf(touch_file, 32, "%s %s", "touch", LOGIN_FILE);
	system(touch_file);

	return;
}

static void
substituteFile(void)
{
	/* Substitute temporary file to new login file */
	char substitute_file[32];
	snprintf(substitute_file, 32, "%s %s %s", "mv", TMP_LOGIN_FILE, LOGIN_FILE);
	system(substitute_file);

	return;
}

/****************************** Sanitize Input *******************************/

static int
sanitizeUser(const char *user)
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
sanitizePassword(const char *password)
{
	regex_t regex;
	char regex_string[192] =
	"(.*[A-Z].*[0-9].*[!@#$%&*].*)|(.*[A-Z].*[!@#$%&*].*[0-9].*)|"
	"(.*[0-9].*[A-Z].*[!@#$%&*].*)|(.*[0-9].*[!@#$%&*].*[A-Z].*)|"
	"(.*[!@#$%&*].*[A-Z].*[0-9].*)|(.*[!@#$%&*].*[0-9].*[A-Z].*)";

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


/***************************** Encode algorithm *******************************/

static void
encode(char *string_encode, const char *password, unsigned int sha256_length)
{
	char newpassword[96];
	unsigned int i;
	unsigned char *encode_password;
	gcry_md_hd_t h;

	/* include salt */
	strncpy(newpassword, password, 32);
	newpassword[32] = '\0';
	strcat(newpassword, SALT);

	/* init context */
	gcry_md_open(&h, GCRY_MD_SHA256, GCRY_MD_FLAG_HMAC);
	/* insert mac key */
	gcry_md_setkey(h, MAC_KEY, sha256_length);
	/* generate hash */
	gcry_md_write(h, newpassword, strlen(newpassword));
	/* get result */
	encode_password = gcry_md_read(h, GCRY_MD_SHA256);

	/* convert encode password to string */
	for (i = 0; i < sha256_length; i++)
		sprintf(string_encode+(i*2), "%02x", encode_password[i]);

	return;
}


static unsigned int
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
