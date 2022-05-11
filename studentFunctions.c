#pragma warning(disable:4996)
#include "studentFunctions.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void welcomeDash(int *selection, int userAmount)
{	
	int inputAccepted = INP_REJECT;
	
	while (inputAccepted != INP_ACCEPT) /* Iterates while inputAccepted != INP_ACCEPT (1) */
	{
		printf("File Locker - Menu\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Please make a selection:\n");
		printf("1. Log in with user credentials\n");
		printf("2. Register as a new user\n\n");

		printf("Selection: ");

		scanf("%d", selection);
		getchar();
		
		if (*selection != LOGIN && *selection != REGISTER) /* If selection is incorrect */
		{
			inputAccepted = INP_REJECT; /* Setting inputAccepted to INP_REJECT (0) */

			system("@cls||clear");
			printf("Incorrect selection, please try again. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (*selection == LOGIN && userAmount > 0) /* If user selection is LOGIN */
		{
			inputAccepted = INP_ACCEPT; /* Setting inputAccepted to INP_ACCEPT (1) */
		}
		else if (*selection == LOGIN && userAmount == 0) /* If user selection is LOGIN and no users are registered */
		{
			inputAccepted = INP_REJECT; /* Setting inputAccepted to INP_REJECT (0) */
			
			system("@cls||clear");
			printf("No users were found in the database. Please register a user before trying to log in. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (*selection == REGISTER) /* If user selection is REGISTER */
		{
			inputAccepted = INP_ACCEPT; /* Setting inputAccepted to INP_ACCEPT (1) */
		}
	}
}

void randomizeKey(char tempKey[KEY_LENGTH])
{
	int randomNumber; /* Variable used to store randomized number */

	char *letters = "abcdefghijklmnopqrstuvwxyz"; /* Letters available for crypto key */
	
	srand((unsigned int)time(NULL)); /* Initializing srand with time(NULL) as seed */

	for (int i = 0; i < 4; i++) /* Iterates while i < KEY_LENGTH (indexation) */
	{
		if (i < 3)
		{
			randomNumber = rand() % 26; /* Randomizing number between 0-25 for indexation */
			tempKey[i] = letters[randomNumber]; /* Setting tempKey at index i to the letter at index randomNumber */
		}
		else if (i == 3)
		{
			tempKey[i] = '\0'; /* Inserting null terminator */
		}
	}
}

void usernameCheck(struct User* users, int *inputStatus, char usernameInput[], int *userAmount)
{
	int len = strlen(usernameInput); /* Storing length of entered string */
	int match = NO_MATCH; /* Variable used to signal if username does already exist */
	int currentUser = 0;

	if (len > (MAX_USERNAME_LEN - 1)) /* If username is too long */
	{
		*inputStatus = TOO_LONG;
	}
	else if (len < MIN_UN_LEN) /* If username is too short */
	{
		*inputStatus = TOO_SHORT;
	}
	
	if (len <= (MAX_USERNAME_LEN - 1) && len >= MIN_UN_LEN) /* If username is of allowed length */
	{
		FILE* userDatabase = fopen("userDB", "rb");

		while (match != MATCH && currentUser <= *userAmount - 1 ) /* While no match has been found & currentUser <= *userAmount - 1 */
		{
			if (currentUser < *userAmount - 1) /* If currentUser < *userAmount - 1 (- 1 for indexation) */
			{
				fread(&users[currentUser], sizeof(struct User), 1, userDatabase);
				match = strcmp(users[currentUser].username, usernameInput); /* Checking if username at index currentUser matches entered username */

				if (match == MATCH) /* If match has been found */
				{
					*inputStatus = ALREADY_EXISTS;
				}
			}

			if (match != MATCH) /* If no match has been found */
			{
				currentUser++;
			}
		}

		fclose(userDatabase);
	}

		for (int i = 0; i < len && *inputStatus != FORBIDDEN_CHARS && *inputStatus != ALREADY_EXISTS && *inputStatus != TOO_LONG && *inputStatus != TOO_SHORT; i++) /* Iterating only when username is not too long, too short, doesn't already exist, doesn't contain forbidden chars & i < len */
		{
			if ((usernameInput[i] >= 'a' && usernameInput[i] <= 'z') || (usernameInput[i] >= '0' && usernameInput[i] <= '9')) /* If character at index i is in the allowed range */
			{
				*inputStatus = ACCEPTED;
			}
			else
			{
				*inputStatus = FORBIDDEN_CHARS;
			}
		}
}

void passwordCheck(int *inputStatus, char passwordInput[])
{
	int len = strlen(passwordInput); /* Storing length of entered string */

	if (len > ( MAX_PASSWORD_LEN - 1)) /* If password is too long */
	{
		*inputStatus = TOO_LONG; 
	}
	else if (len < MIN_UN_LEN) /* If password is too short */
	{
		*inputStatus = TOO_SHORT;
	}
	else if (len <= (MAX_PASSWORD_LEN - 1) || len >= TOO_SHORT) /* If password is of allowed length */
	{
		for (int i = 0; i < len && *inputStatus != FORBIDDEN_CHARS; i++) /* Iterating while i < len and no forbidden characters have been found */
		{
			if ((passwordInput[i] >= 'a' && passwordInput[i] <= 'z') || (passwordInput[i] >= '0' && passwordInput[i] <= '9')) /* If character at index i is in allowed range*/
			{
				*inputStatus = ACCEPTED;
			}
			else
			{
				*inputStatus = FORBIDDEN_CHARS;
			}
		}
	}
}

void userLogin(struct User* users, int *currentUser, int *userAmount, int *loggedIn)
{
	system("@cls||clear");

	int currentUserTemp;
	int match = NO_MATCH;

	char username[30]; /* Stores username temporarily */
	char password[30]; /* Stores password temporarily */

	FILE* userDatabase = fopen("userDB", "rb");

	while (match != MATCH) /* Iterates while no match has been found */
	{
		currentUserTemp = 0; /* Initializing to 0 for indexation */

		printf("File Locker - User Login\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Enter your username: ");
		scanf("%s", &username);
		getchar();

		while (match != MATCH && currentUserTemp != *userAmount) /* Iterates while no match has been found and the whole database hasn't been checked */
		{
			if (currentUserTemp < *userAmount)
			{
				fread(&users[currentUserTemp], sizeof(struct User), 1, userDatabase); /* Reading current user into struct Users* users */
				
				match = strcmp(users[currentUserTemp].username, username); /* Checking if username at index currentUserTemp matches the entered username */
			}

			if (match != MATCH && currentUserTemp < *userAmount) /* If no match has been found and the whole database hasn't been checked */
			{
				currentUserTemp++; /* Increment currentUserTemp */
			}
			
			if (currentUserTemp == *userAmount && match != MATCH) /* If no match has been found and the whole database has been checked */
			{
				system("@cls||clear");
				printf("Username was not found in the database. Press return to go back.");
				getchar();
				system("@cls||clear");
			}
		}
	}

	match = NO_MATCH; /* Resetting match-variable before prompting for password */

	system("@cls||clear");

	while (match != MATCH) /* While no match has been found */
	{
		printf("File Locker - User Login\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Enter your password: ");
		scanf("%s", &password);
		getchar();

		match = strcmp(users[currentUserTemp].password, password); /* Checking if password at index currenUserTemp matches entered password */

		if (match != MATCH) /* If no match was found */
		{
			system("@cls||clear");
			printf("You've entered an incorrect password, press return to try again.");
			getchar();
			system("@cls||clear");
		}
		else if (match == MATCH) /* If match was found */
		{
			*currentUser = currentUserTemp; /* Setting *currentUser to the id of the user who has logged in */
			*loggedIn = LOGGED_IN; /* Setting login status to LOGGED_IN (1)*/
		}
	}
}

void userRegistration(struct User* users, int *userAmount)
{
	system("@cls||clear");

	int currUser = 0; /* Holds current user ID */
	int inputStatus = NEEDS_CHECKING; /* Holds status for user input */

	char usernameTemp[20]; /* Temporary variable for username input */
	char passwordTemp[20]; /* Temporary variable for password input */
	char tempKey[KEY_LENGTH]; /* Temporary variable for crypto key */
	
	FILE* userDatabase;
	FILE* userCount;

	*userAmount += 1; /* Increase user amount before registration */
	currUser = *userAmount - 1; /* Array indexation for current user */

	while (inputStatus != ACCEPTED) /* Iterates while user input has not yet been accepted */
	{
		printf("File Locker - User registration\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Please choose a username that matches the following criteria: 5-10 characters long, contains only lower case alphabethic letters and numbers.\n\n");
		printf("Choose a username: ");
		scanf("%s", &usernameTemp);
		getchar();
		usernameCheck(users, &inputStatus, usernameTemp, userAmount); /* Checks if username is valid and if it is free */

		if (inputStatus == ACCEPTED) /* If username is accepted */
		{
			strcpy(users[currUser].username, usernameTemp);
		}
		else if (inputStatus == FORBIDDEN_CHARS) /* Username contains forbidden characters */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("Username contains forbidden characters. Press return to continue.");
			getchar();
			system("@cls||clear");	
		}
		else if (inputStatus == TOO_LONG) /* Username is too long */
		{
			inputStatus = NEEDS_CHECKING;
			
			system("@cls||clear");
			printf("Username is too long. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == TOO_SHORT) /* Username is too short */
		{
			inputStatus = NEEDS_CHECKING;
			
			system("@cls||clear");
			printf("Username is too short. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == ALREADY_EXISTS) /* Username already exists */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("Username already exists, please pick another one. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
	}

	system("@cls||clear");
	
	inputStatus = NEEDS_CHECKING; /* Resetting inputStatus before password input */

	while (inputStatus != ACCEPTED) /* Iterates while user input has not yet been accepted */
	{
		printf("File Locker - User registration\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Please choose a password that matches the following criteria: 5-10 characters long, contains only lower case alphabethic letters and numbers.\n\n");
		printf("Choose a password: ");
		scanf("%s", &passwordTemp);
		getchar();

		passwordCheck(&inputStatus, passwordTemp); /* Checks if password is valid */

		if (inputStatus == ACCEPTED) /* If password is accepted */
		{
			strcpy(users[currUser].password, passwordTemp);
		}
		else if (inputStatus == FORBIDDEN_CHARS) /* Password contains forbidden characters */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("Password contains forbidden characters. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == TOO_LONG) /* Password is too long */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("Password is too long. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == TOO_SHORT) /* Password is too short */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("Password is too short. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
	}

	randomizeKey(tempKey); /* Randomizing crypto key */
	strcpy(users[currUser].crypto_key, tempKey); /* Inserting key into struct */
	users[currUser].file_amount = 0; /* Setting file amount to 0 */

	userDatabase = fopen("userDB", "ab");
	fwrite(&users[currUser], sizeof (struct User), 1, userDatabase); /* Writing user data to file "userDB" */

	fclose(userDatabase);

	system("@cls||clear");

	userCount = fopen("userAmount", "w");
	fprintf(userCount, "%d", *userAmount); /* Updating user count in file userAmount */

	fclose(userCount);

	printf("Registration successful. Press return to continue to the login screen.");
	getchar();
}

void userMenu(struct User* users, int currentUser, int userAmount, int *menuSelection, char* textPointer)
{
	*menuSelection = NO_SELECTION;

	while (*menuSelection == NO_SELECTION)
	{
		printf("File Locker - User Menu\n");
		printf("-------------------------------------------------------------------\n");
		printf("Welcome, %s! You've successfully signed in. Crypto key: %s\n\n", users[currentUser].username, users[currentUser].crypto_key);
		printf("Choose an action in the menu below:\n");
		printf("1. Enter new text\n");
		printf("2. Save entered text on file\n");
		printf("3. Add text to already existing file\n");
		printf("4. View existing file\n");
		printf("5. Change password\n");
		printf("6. Sign out\n");
		printf("7. Exit application\n\n");
		printf("Selection: ");

		scanf("%d", menuSelection);
		getchar();

		if (*menuSelection == SAVE_TEXT && textPointer == NULL) /* If text has not yet been entered */
		{
			system("@cls||clear");
			printf("You don't have any text to save. Enter a new text first. Press return to continue.");
			getchar();
			system("@cls||clear");
			*menuSelection = NO_SELECTION;
		}
		else if (*menuSelection == ENTER_NEW_TXT && textPointer != NULL) /* If text has already been entered */
		{
			system("@cls||clear");
			printf("You have already entered text. Please save the text before entering new one. Press return to continue.");
			getchar();
			system("@cls||clear");
			*menuSelection = NO_SELECTION;
		}
		else if (*menuSelection < 1 || *menuSelection > 7) /* If incorrect selection has been made */
		{
			system("@cls||clear");
			printf("You have made an incorrect selection. Press return to continue.");
			getchar();
			system("@cls||clear");
			*menuSelection = NO_SELECTION;
		}
	}
}

void encryptText(struct User* users, char *textPointer, int textSize, int currentUser, int currentFile, int keyIndex)
{

	for (int i = 0; i < textSize - 1; i++) /* Iterates while i < textSize - 1 (for array indexation and exclusion of \0) */
	{
		if (textPointer[i] >= 32 && textPointer[i] <= 126) /* If character is in the allowed range */
		{
			textPointer[i] = (textPointer[i] - 32 + users[currentUser].crypto_key[keyIndex] - 32) % 95 + 32; /* Encrypt character with provided formula */
		}

		if (keyIndex == 2)
		{
			keyIndex = 0;
		}
		else if (keyIndex != 2)
		{
			keyIndex++;
		}
	}

	users[currentUser].crypto_key_index[currentFile] = keyIndex; /* Saving keyIndex for current file in user post, used for future reference (appending text) */
}

void decryptText(struct User* users, int currentUser, int textSize, char *textInput)
{
	char temp[1]; /* Used to temporarily store 1 character */

	int keyIndex = 0; /* Used for indexation of crypto key - initialized to 0 */

	for (int i = 0; i < textSize - 1; i++) /* Iterates while i < textSize -1 (for array indexation and exclusion of \0) */
	{
		if (textInput[i] >= 32 && textInput[i] <= 126) /* If character is in allowed range */
		{
			temp[0] = textInput[i]; /* Temporarily storing character at index i in temp */
			temp[0] = ((temp[0] + 32 + 32 + 95) - users[currentUser].crypto_key[keyIndex] - 32) % 95;

			if (temp[0] >= 32 && temp[0] <= 126) /* If decrypted character is in the allowed range */
			{
				textInput[i] = temp[0];
			}
			else if (!(temp[0] >= 32 && temp[0] <= 126)) /* If decrypted character is not in the allowed range */
			{
				textInput[i] = ((textInput[i] + 32 + 32 + 95) - users[currentUser].crypto_key[keyIndex] - 32);
			}
		}

		if (keyIndex == 2)
		{
			keyIndex = 0;
		}
		else if (keyIndex != 2)
		{
			keyIndex++;
		}
	}
}

char *enterText(int *textSize, int isAppend)
{
	char *textInput = NULL; /* Char pointer to text input - set to NULL */
	char textBuffer[500]; /* Text buffer for text input */
	char continueInput[2]; /* Text buffer that stores the first two characters on each row */
	
	*textSize = 1; /* Initialized to 1 to accomodate \0 */

	textInput = (char*)malloc(*textSize * (sizeof(*textInput)));
	strcpy(textInput, "\0"); /* Setting first character to \0 */

	if (isAppend == 0) /* If text is not being appended */
	{
		system("@cls||clear");
		printf("File Locker - Enter New Text\n");
		printf("-------------------------------------------------------------------\n");
		printf("Enter text and press return to start a new line. If you wish to end text input, type -1 on an empty line.\n\n");
	}

	while (!(continueInput[0] == '-' && continueInput[1] == '1')) /* While the first two characters are not - and 1 */
	{
		memset(textBuffer, 0, sizeof(textBuffer)); /* Setting all characters in textBuffer to 0 */
		fgets(textBuffer, 500, stdin); /* Taking user input */
		continueInput[0] = textBuffer[0]; /* Setting continueInput at index 0 to first character on row */
		continueInput[1] = textBuffer[1]; /* Setting continueInput at index 1 to second character on row */

		if (!(continueInput[0] == '-' && continueInput[1] == '1')) /* If the first two characters are not - and 1*/
		{
			*textSize += strlen(textBuffer); /* Incrementing *textSize with the length of entered string */
			textInput = (char*)realloc(textInput, *textSize); /* Reallocating memory for textInput to accomodate the entered string */
			strcat(textInput, textBuffer); /* Adding entered string to to textInput */
		}
	}

	return textInput; /* Returning pointer to textInput */
}

char *saveText(struct User* users, int *textSize, char* textPointer, int currentUser)
{
	FILE* saveFile;
	FILE* userDatabase;

	if (users[currentUser].file_amount < 20) /* If user has not created maximum amount of files */
	{

		int currentFile = users[currentUser].file_amount;
		int nameAccepted = INP_REJECT;

		char fileName[30]; /* Temporary char array for file name */

		while (nameAccepted != INP_ACCEPT) /* While name is not accepted */
		{
			system("@cls||clear");
			printf("File Locker - Save Text On File\n");
			printf("-------------------------------------------------------------------\n");
			printf("Select a file name (maximum 10 characters): ");

			scanf("%s", &fileName);
			getchar();

			if (strlen(fileName) > 10) /* If file name is too long */
			{
				system("@cls||clear");
				printf("The chosen file name is too long. Press return to continue.");
				getchar();
			}
			else if (strlen(fileName) <= 10) /* If file name is of allowed length */
			{
				saveFile = fopen(fileName, "rb"); /* Trying to open file with chosen file name */

				if (saveFile == NULL) /* If file does not exist */
				{
					nameAccepted = INP_ACCEPT; /* Name is accepted */
				}
				else /* File already exists */
				{
					fclose(saveFile);

					system("@cls||clear");
					printf("The chosen file name already exists. Press return to continue.");
					getchar();
				}
			}
		}

		saveFile = fopen(fileName, "wb"); /* Create file with chosen name and open for writing */

		encryptText(users, textPointer, *textSize, currentUser, currentFile, 0); /* Calling function to encrypt the text that has been entered */

		fwrite(textPointer, *textSize, 1, saveFile); /* Writing encrypted text to file */
		fclose(saveFile);
		free(textPointer); /* Freeing memory */
		textPointer = NULL; /* Setting textPointer to NULL */

		*textSize = 0; /* Resetting *textSize to 0 */

		strcpy(users[currentUser].file_paths[currentFile], fileName); /* Copying file path into user post */
		users[currentUser].file_amount += 1; /* Incrementing file amount in user post */

		userDatabase = fopen("userDB", "rb+"); /* Opening user database for reading and writing */
		fseek(userDatabase, currentUser * sizeof(struct User), SEEK_SET); /* Moving file pointer to current user in user database file */

		fwrite(&users[currentUser], sizeof(struct User), 1, userDatabase); /* Writing new data from user post into database */
		fclose(userDatabase);

		system("@cls||clear");
		printf("File was succesfully saved. Press return to continue.");
		getchar();
		system("@cls||clear");
	}
	else if (users[currentUser].file_amount == 20) /* If user has created maximum amount of files */
	{
		system("@cls||clear");
		printf("You have created the maximum amount of files. Press return to continue.");
		getchar();
		system("@cls||clear");
	}

	return textPointer; /* Returning pointer to textPointer (NULL if save was succesful) */
}

void fileExplorer(struct User* users, int currentUser, int *currentFile)
{
	int fileAmount = (users[currentUser].file_amount); /* Storing the total amount of files for current user */
	int exit = 0;
	int inputAccepted = INP_REJECT;

	while (exit == 0)
	{
		system("@cls||clear");
		printf("File Locker - File Explorer\n");
		printf("-------------------------------------------------------------------\n\n");

		if (fileAmount > 0) /* If user has created files */
		{
			printf("The following files are available for your account:\n\n");

			for (int i = 0; i < fileAmount; i++) /* Printing out all available files for user */
			{
				printf("%d. %s\n", i+1, users[currentUser].file_paths[i]);
			}
			
			while (inputAccepted != INP_ACCEPT) /* While input is not accepted */
			{
				printf("\n\nEnter the number of the file you want to open: ");
				scanf("%d", currentFile);
				getchar();
				exit = 1;

				if (*currentFile > users[currentUser].file_amount || *currentFile < 1) /* If a file that doesn't exist is selected */
				{
					printf("\n\nThe file specified does not exist.");
				}
				else /* If existing file is chosen */
				{
					inputAccepted = INP_ACCEPT;
				}
			}
		}
		else if (fileAmount == 0) /* If user has created no files */
		{
			exit = 1;
			printf("No files available. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
	}
}

void appendText(struct User* users, int currentUser)
{
	FILE* appendFile;
	FILE* userDatabase;

	char *fileName;
	char *enteredText;

	int currentFile = -1; /* Current file selection initialized to -1 (if no files are available) */
	int keyIndex; /* Holds key index from user post for current file */
	int textSize; /* Holds amount of entered characters */
	int fileSize = 0; /* Holds fileSize of the file that is to be appended */

	char* fileContents; /* Char pointer to file contents */

	fileExplorer(users, currentUser, &currentFile); /* Calling fileExplorer to select file */

	if (currentFile != -1) /* If file selection was succesful */
	{
		currentFile = currentFile - 1;
		fileName = users[currentUser].file_paths[currentFile]; /* Changing *fileName to point at file name for current file */
		keyIndex = users[currentUser].crypto_key_index[currentFile]; /* Storing key index for current file in keyIndex */

		system("@cls||clear");
		printf("File Locker - Add text to already existing file\n");
		printf("-------------------------------------------------------------------\n");
		printf("Enter text and press return to start a new line. If you wish to end text input, type -1 on an empty line.\n\n");

		enteredText = enterText(&textSize, 1); /* Call enterText for user input */
		encryptText(users, enteredText, textSize, currentUser, currentFile, keyIndex); /* Calls encryptText to encrypt text */

		appendFile = fopen(fileName, "rb");
		fseek(appendFile, 0, SEEK_END); /* Moves file pointer to end of file */
		fileSize = ftell(appendFile); /* Stores file pointer position in fileSize */
		fseek(appendFile, 0, SEEK_SET); /* Moves file pointer to the beginning of file */

		fileContents = (char*)malloc(sizeof(char) * fileSize); /* Allocating memory for fileContents */
		fread(fileContents, fileSize, 1, appendFile); /* Reading file contents into fileContents */
		fclose(appendFile);

		fileContents = realloc(fileContents, (fileSize + textSize - 1)); /* Re-allocating memory to accomodate new text input (-1 byte due to removal of one '\0' char ) */
		strcat(fileContents, enteredText); /* Copying new string into end of fileContents */
		appendFile = fopen(fileName, "wb");
		fwrite(fileContents, (fileSize + textSize - 1), 1, appendFile); /* Writing new contents to file */

		fclose(appendFile);
		free(enteredText); /* Freeing memory for enteredText */
		free(fileContents); /* Freeing memory for fileContents */
		enteredText = NULL;

		userDatabase = fopen("userDB", "rb+");
		fseek(userDatabase, currentUser * sizeof(struct User), SEEK_SET); /* Moving file pointer to current user */
		fwrite(&users[currentUser], sizeof(struct User), 1, userDatabase); /* Writing new user data (updated key index for current file) */

		fclose(userDatabase);

		system("@cls||clear");
		printf("Text was succesfully added to file. Press return to continue.");
		getchar();
		system("@cls||clear");
	}
}

void viewFile(struct User* users, int currentUser)
{
	FILE* userFile; /* File pointer used for user selected file*/
	
	int currentFile = -1; /* Initializing file selection to -1, in case no file is available in the file explorer */
	int fileSize = 0;
	
	char *fileName;
	char *fileContents;

	fileExplorer(users, currentUser, &currentFile); /* Calling file explorer function to enable user to select file */
	
	if (currentFile != -1) /* If file selection was succesful */
	{
		fileName = users[currentUser].file_paths[currentFile - 1]; /* Gathering file name for selected file, changing fileName to point at selected post */
		userFile = fopen(fileName, "rb");

		fseek(userFile, 0, SEEK_END); /* Moving file pointer to end of file */
		fileSize = ftell(userFile); /* Storing file pointer location (size of whole file) */
		fseek(userFile, 0, SEEK_SET); /* Moving file pointer back to start */

		fileContents = (char*)malloc(fileSize + 1); /* Allocating memory for file content (fileSize + 1 to accomodate '\0'. */
		fread(fileContents, fileSize, 1, userFile); /* Reading file into fileContents */

		decryptText(users, currentUser, fileSize, fileContents); /* Calling the decryption function to decrypt text */

		system("@cls||clear");
		printf("File Locker - View existing file\n");
		printf("-------------------------------------------------------------------\n");
		printf("File: %s\n\n", fileName); /* Printing file name */
		printf("%s", fileContents); /* Printing file contents */
		getchar();

		free(fileContents); /* Freeing memory */
		fclose(userFile);
	}
}

void changePassword(struct User* users, int currentUser)
{
	char newPW[20];
	int inputStatus = NEEDS_CHECKING; /* Initializing inputStatus to NEEDS_CHECKING (0) */

	FILE* userDatabase = fopen("userdb", "rb+");

	fseek(userDatabase, currentUser * sizeof(struct User), SEEK_SET); /* Moving file pointer forward to the user post for the current user (size of user post * currentUser) */

	system("@cls||clear");

	while (inputStatus != ACCEPTED) /* Iterates while input is not ACCEPTED */
	{
		printf("Change your password\n");
		printf("-------------------------------------------------------------------\n\n");
		printf("Please choose a password that matches the following criteria: 5-10 characters long, contains only lower case alphabethic letters and numbers.\n\n");

		printf("Choose a new password: ");
		scanf("%s", &newPW);
		getchar();

		passwordCheck(&inputStatus, newPW);

		if (inputStatus == ACCEPTED) /* If input is accepted */
		{
			memset(users[currentUser].password, 0, MAX_PASSWORD_LEN);
			strcpy(users[currentUser].password, newPW);
		}
		else if (inputStatus == TOO_LONG) /* If input is too long */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("The entered password is too long. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == TOO_SHORT) /* If input is too short */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("The entered password is too short. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
		else if (inputStatus == FORBIDDEN_CHARS) /* If input contains forbidden characters */
		{
			inputStatus = NEEDS_CHECKING;

			system("@cls||clear");
			printf("The entered password contains forbidden characters. Press return to continue.");
			getchar();
			system("@cls||clear");
		}
	}

	fwrite(&users[currentUser], sizeof(struct User), 1, userDatabase); /* Writing the updated contents of selected user post to user database */

	fclose(userDatabase);

	system("@cls||clear");
	printf("Your password was succesfully changed. Press return to continue.");
	getchar();
}

char *signOut(struct User* users, int *loggedIn, char* textPointer)
{
	free(users); /* Freeing memory allocated for users */

	if (textPointer != NULL)
	{
		free(textPointer); /* Freeing memory allocated to entered text only if text has been entered*/
		textPointer = NULL;
	}

	*loggedIn = NOT_LOGGED_IN; /* Setting variable loggedIn to NOT_LOGGED_IN (0) */

	system("@cls||clear");
	printf("You've succesfully signed out. Press return to continue to the log-in screen.");
	getchar();
	system("@cls||clear");

	return textPointer;
}

void quitApp(struct User* users, int *loggedIn, char* textPointer)
{
	free(users); /* Freeing memory allocated for users */

	if (textPointer != NULL)
	{
		free(textPointer); /* Freeing memory allocated to entered text only if text has been entered*/
	}

	*loggedIn = NOT_LOGGED_IN; /* Setting variable loggedIn to NOT_LOGGED_IN (0) */

	system("@cls||clear");
	printf("You've been signed out. Press return to exit application.");
	getchar();
	system("@cls||clear");
}