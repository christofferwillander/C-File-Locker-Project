/*
* Programmer: Christoffer Willander		Date completed: January 14th, 2018
* Instructor: Carina Nilsson			Class: DV1550
*
* File locker project
*/

#pragma warning(disable:4996)
#include "studentFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int dashSelection = NO_SELECTION; /* Holds selection on welcome screen */
	int menuSelection = NO_SELECTION; /* Holds menu selection in user menu */
	int userAmount = NO_USERS; /* Holds total amount of registered users - initialized to NO_USERS (-1) */
	int textSize = 0; /* Holds amount of characters entered in text buffer */

	char *enteredText; /* Char pointer to store entered text */

	FILE* userCount; /* File pointer to file holding amount of registered users */
	FILE* userDatabase; /* File pointer to file storing user information */

	userCount = fopen("userAmount", "r");

	if (userCount == NULL) /* If user count file does not exist */
	{
		userCount = fopen("userAmount", "w");
		fprintf(userCount, "%d", 0); /* Set user count to 0 */
	}
	
	fclose(userCount);
	
	userDatabase = fopen("userDB", "rb");

	if (userDatabase == NULL) /* If user database file does not exist */
	{
		userDatabase = fopen("userDB", "wb");
	}
	
	fclose(userDatabase);

	while (menuSelection != EXIT_APP) /* Iterates while user has not chosen to exit application */
	{
		int currentUser = 0; /* Holds the id of the current user*/
		int loggedIn = NOT_LOGGED_IN;

		menuSelection = NO_SELECTION;
		
		userCount = fopen("userAmount", "r"); 
		fscanf(userCount, "%d", &userAmount); /* Reading user amount, storing in userAmount */
		fclose(userCount);

		struct User* users;
		users = (struct User*) malloc((sizeof(struct User)* (userAmount + 1))); /* Allocating memory for amount of registered users + 1 (in order to register a new user, if necessary) */

		welcomeDash(&dashSelection, userAmount);

		if (dashSelection == LOGIN)
		{
			dashSelection = NO_SELECTION;
			userLogin(users, &currentUser, &userAmount, &loggedIn);
		}
		else if (dashSelection == REGISTER)
		{
			dashSelection = NO_SELECTION;
			userRegistration(users, &userAmount);
			userLogin(users, &currentUser, &userAmount, &loggedIn);
		}

		enteredText = NULL;

		while (loggedIn == LOGGED_IN && menuSelection != EXIT_APP) /* Iterates while user is logged in and has not chosen to exit application */
		{
			system("@cls||clear");
			userMenu(users, currentUser, userAmount, &menuSelection, enteredText);

			if (menuSelection == ENTER_NEW_TXT && enteredText == NULL) /* Selection only available if text has not already been entered */
			{
				enteredText = enterText(&textSize, 0);
			}
			else if (menuSelection == SAVE_TEXT)
			{
				enteredText = saveText(users, &textSize, enteredText, currentUser); /* Setting enteredText pointer to NULL after succesful save */
			}
			else if (menuSelection == ADD_TEXT)
			{
				appendText(users, currentUser);
			}
			else if (menuSelection == VIEW_FILE)
			{
				viewFile(users, currentUser);
			}
			else if (menuSelection == CHANGE_PW)
			{
				changePassword(users, currentUser);
			}
			else if (menuSelection == SIGN_OUT)
			{
				enteredText = signOut(users, &loggedIn, enteredText); /* Setting enteredText pointer to NULL if text was entered before signing out */
			}
			else if (menuSelection == EXIT_APP)
			{
				quitApp(users, &loggedIn, enteredText);
			}
		}
	}

	return 0;
}