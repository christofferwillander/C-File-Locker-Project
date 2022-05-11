#ifndef STUDENTFUNCTIONS_H
#define STUDENTFUNCTIONS_H

#define MAX_FILES_PER_USER 20 /* Max files per user 20*/
#define MAX_USERNAME_LEN 11 /* Max username length 10, one byte reserved for \0 */
#define MAX_PASSWORD_LEN 11 /* Max password length 10, one byte reserved for \0 */
#define MIN_UN_LEN 5 /* Min username length 5 */
#define MIN_PW_LEN 5 /* Min password length 5 */
#define MAX_FILE_NAME_LEN 11 /* Max file name length, one byte reserved for \0 */
#define MAX_FILE_PATH_LEN 11 /* Max file path length */
#define KEY_LENGTH 4 /* Max key length 3, one byte reserved for 0\ */
#define NEEDS_CHECKING 0 /* Used in registration & during password change - username/password needs to be evaluated */
#define FORBIDDEN_CHARS 1 /* Used in registration & during password change - username/password contains forbidden chars */
#define ACCEPTED 2 /* Used in registration & during password change - username/password is accepted */
#define TOO_LONG 3 /* Used in registration & during password change - username/password is too long */
#define ALREADY_EXISTS 4 /* Used in registration - username already exists in database */
#define NOT_EXISTING 5 /* Used in registration & during password change - username doesn't exist in database */
#define TOO_SHORT 6 /* Used in registration & during password change - username is too short */
#define NO_MATCH -1 /* Used in user registration/login to check if a match to username/password has been found in database - no match found */
#define MATCH 0 /* Used in user registration/login to check if a match to username/password has been found in database - match found */
#define NO_SELECTION 0 /* Used for menu selection - no selection made */
#define LOGIN 1 /* Used for menu selection - user login */
#define REGISTER 2 /* Used for menu selection - user registration */
#define ENTER_NEW_TXT 1 /* Used for menu selection - enter new text */
#define SAVE_TEXT 2 /* Used for menu selection - save text on file */
#define ADD_TEXT 3 /* Used for menu selection - add new text to existing file */
#define VIEW_FILE 4 /* Used for menu selection - view existing file */
#define CHANGE_PW 5 /* Used for menu selection - change password */
#define SIGN_OUT 6 /* Used for menu selection - sign out */
#define EXIT_APP 7 /* Used for menu selection - exit application */
#define NOT_LOGGED_IN 0 /* Used to keep track of if user is logged in - logged out */
#define LOGGED_IN 1 /* Used to keep track of if user is logged in - logged in */
#define INP_ACCEPT 1 /* Input accepted */
#define INP_REJECT 0 /* Input rejected */
#define NO_USERS -1; /* Used when initializing userAmount variable */

void welcomeDash(int *selection, int userAmount); /* Prints welcome screen and handles selection */
void userLogin(struct User* users, int *currentUser, int *userAmount, int *loggedIn); /* Handles user login */
void userRegistration(struct User* users, int *userAmount); /* Handles user registration*/
void usernameCheck(struct User* users, int *inputStatus, char usernameInput[], int *userAmount); /* Checks if username is valid & free during registration*/
void passwordCheck(int *inputStatus, char passwordInput[]); /* Checks if password is valid during registration/when changing password */
void userMenu(struct User* users, int currentUser, int userAmount, int *menuSelection, char* textPointer); /* Shows user menu when user is logged in and handles user selection */
void randomizeKey(char tempKey[KEY_LENGTH]); /* Randomizes a 3 char long crypto key upon registrating a new user */
void changePassword(struct User* users, int currentUser); /* Handles password changes when user is logged in */
void encryptText(struct User* users, char *textInput, int textSize, int currentUser, int currentFile, int keyIndex); /* Encrypts text file */
void decryptText(struct User* users, int currentUser, int textSize, char *textInput); /* Decrypts text file */
char *enterText(int *textSize, int isAppend); /* Handles text input */
char *saveText(struct User* users, int *textSize, char *textPointer, int currentUser); /* Saves text as file */
void viewFile(struct User* users, int currentUser); /* Prints contents of chosen file */
void appendText(struct User* users, int currentUser); /* Appends text to file */
void fileExplorer(struct User* users, int currentUser, int *currentFile); /* Prints available files */
char *signOut(struct User* users, int *loggedIn, char *textPointer); /* Signs user out and frees memory */
void quitApp(struct User* users, int *loggedIn, char *textPointer); /* Signs user out and frees memory, quits application */

/* Users struct */
struct User {
	char username[MAX_USERNAME_LEN]; /* Stores username */
	char password[MAX_PASSWORD_LEN]; /* Stores password */
	char crypto_key[KEY_LENGTH]; /* Stores unique encryption key */
	int crypto_key_index[MAX_FILES_PER_USER]; /* Stores encryption key index for each file (used for appending text) */
	char file_paths[MAX_FILES_PER_USER][MAX_FILE_PATH_LEN]; /* Stores file path to each saved file */
	int file_amount; /* Stores amount of files user has saved */
};

#endif