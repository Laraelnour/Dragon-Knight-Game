/*
 Program Created on May 20th
 Program Created By Lara Abouelnour
 Registration Number: 200121299

 Purpose of the code:
 It is a game where you are a warrior in the Middle ages and you have to fight Dragons and save Civilians

 All 'scenes' of the game are stored in files and read and outputted to the screen when required
 There are 3 types of scene files: Dragon, Fire breathing dragon, and Pedestrian.
 Each of these has 4 files each for different scenarios :Prepare, Attack, Defend, Save - Fire breathing dragon has 5: Death.

 When the game begins, the user is asked to input a username which is then checked against an existing username database (Usernames text file).
 If the username already exists, the previous high score is displayed (from Scores text file using the row number of the username) and the user is asked
 if they would like to use this username.
 Using the existent username means the score will be updated if the score won in this game is higher than the one recorded. (If it is lower, the old score will remain)
 If they decide to not use the username, they can try another username and if it is new (not in the database) the score will be set as zero until they play,
 then it will be updated with the score won in the game.

 After being asked for the username, the user is asked how many rounds they want to play per game.

 Then user is asked if they want to play on Easy, Medium or Hard difficulty level.
 The difference between them is the reaction time allowed, Easy requires you to react within 5 seconds, Medium within 2.5 seconds, and Hard within 1 second.

 After, the user is asked if they want to view the Rules. If they do, they are also asked if they want to see how the stances of 'Prepare',
 'Attack', 'Defend', and 'Save' look like.

 Then the game begins!
 Scene begins with the 'Prepare' stance. User presses A for 'Attack', S for 'Save', and D for 'Defend'
 Score is calculated and displayed according to the stances picked by the user during each scene.
 Lives are also adjusted according to whether the user 'Defended' against Fire or got burnt.
 Have a look at the Rules for detailed breakdown of these two points.

 At the end of the game, if the user still has lives to spare, they are asked if they want to continue.
 Answering yes will start another game for them with the same difficulty level and round number per game.
 Answering no, will ask them if they want want to start a new game. No will end the program, yes will loop to the beginning.

 But if the user ran out of lives during or at the end of the game, they lose and are asked if they want to play again.
 Answering yes will ask them if they want to use the same username and thus number of rounds per game and difficulty level.
 (Yes, means lives will be restored and points will be reset to zero. No, will loop the program to the beginning.)
 Answering no will end the game.

 Score is always displayed at the end of a game, or when the user loses.

 This code makes use of Files, Structures, Functions, Pointers, Passing by reference/value, Arrays, and many others...

*/


#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<time.h>


///Structures

//stores the info of the user playing currently
struct Userinfo
{
    char username[20];                 //holds the username
    int neg_PedestrianPoints;      //holds points lost from not saving pedestrians
    int pos_PedestrianPoints;      //holds points gained from saving pedestrians
    int DragonPoints;              //holds points from killing dragons
    float WrongmovePoints;
    float TotalPoints;                   //Stores the total points by the end of the game
};

//Stores all the players for checking if the username exists before playing
struct Players
{
    int arraysize;
    float Scores;
    char username[20];
};

/// Function Prototypes
void ReadFile (FILE *myfile);
void ShowingStances (FILE *file1, FILE *file2, FILE *file3, FILE *file4);
int ShowPrepareScene (FILE *file1, FILE *file2, FILE *file3, int Lives);
void Dragon_Action_Score (int Difficulty, struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, int Lives);
void FireDragon_Action_Score (int Difficulty,int *Lives, struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, FILE *file5);
void Pedestrian_Action_Score (int Difficulty,struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, int Lives);
void Username_File (FILE* user, struct Players *Players, int *Players_no);
void compare(struct Players Players[], char Input[], int Players_no, int *exists);
void Scores_File (FILE* score, struct Players *Players, int Players_no);
void Life(int Live);

///Main
int main()
{
    FILE* introFile, *outroFile, *endFile, *deathFile, *conti_outro;
    FILE* rulesFile, *scoreFile;
    FILE* PrepareStance, *AttackStance, *DefendStance, *SaveStance;
    FILE* user, *score, *userinfo;
    FILE* D_P, *D_A, *D_D, *D_S;
    FILE* FD_P ,*FD_A, *FD_D, *FD_S, *FD_Died;
    FILE* P_P ,*P_A, *P_D, *P_S;

    struct Userinfo CurrentPlayer;
    struct Players AllPlayers[100];

    int exists[100]={};
    int Index_no;
    int NumberofPlayersinDatabase;
    char usernameinput[20];
    int NewPlayer=0;
    char EnterKey, Spacebar;
    char continueafterRules, EndGame;
    int Exit=0;
    char heart=3;//////////////////////////////////////////////////
    int Lives;
    int h=0;///////////////////////////////////////////////////////
    int PrepareSceneno;
    int Difficulty, Easy=5, Medium=2.5, Hard=1, PickDifficulty;
    int n=0, count=0, Again=0;
    int Rules=0;
    int RepeatLoop=0;
    int loop=0;
    int Plays=0;
    int Sameusername=0;

    do
    {
        //Setting the score to zero
        CurrentPlayer.DragonPoints=0;
        CurrentPlayer.neg_PedestrianPoints=0;
        CurrentPlayer.pos_PedestrianPoints=0;
        CurrentPlayer.TotalPoints=0;
        CurrentPlayer.WrongmovePoints=0;
        Lives=4; //setting up the lives
        RepeatLoop++;

        //opening all the files
        PrepareStance= fopen("Prepare.txt","r"); //Stances
        AttackStance= fopen("Attack.txt","r");
        DefendStance=fopen("Defend.txt","r");
        SaveStance= fopen("Save.txt","r");
        outroFile= fopen("outro.txt","r");
        conti_outro=fopen("Cointinueplaying_outro.txt","r");
        D_P= fopen("Dragon+Prepare.txt","r");            //Dragon scenes
        D_A= fopen("Dragon+Sword.txt","r");
        D_D= fopen("Dragon+Shield.txt","r");
        D_S= fopen("Dragon+Nothing.txt","r");
        FD_P= fopen("DragonFire+Prepare.txt","r");       //Fire Breathing Dragon scenes
        FD_A= fopen("DragonFire+Sword.txt","r");
        FD_D= fopen("DragonFire+Shield.txt","r");
        FD_S= fopen("DragonFire+Nothing.txt","r");
        FD_Died= fopen("DragonFire+Dead.txt","r");
        P_P= fopen("Pedestrian+Prepare.txt","r");        //Pedestrian scenes
        P_A= fopen("Pedestrian+Sword.txt","r");
        P_D= fopen("Pedestrian+Shield.txt","r");
        P_S= fopen("Pedestrian+Nothing.txt","r");
        rulesFile=fopen("GameRules.txt","r");            //Rules
        deathFile= fopen("Dead.txt","r");
        user=fopen("Usernames.txt","a+");                //Usernames Database
        score=fopen("Scores.txt","r+");                  //Scores Database
        userinfo=fopen("Userinfo.txt","r");
        scoreFile=fopen("ScoreTemplate.txt","r");

        //opening and reading the intro file
        introFile=fopen("intro.txt","r");
        system("cls");
        ReadFile(introFile);
        //in the file, it says "Press Enter to proceed"
        do// do not continue with the code until you get a suitable (asked for) input from the user. In this case the Enter Key
        {
          EnterKey= getch();
        }
        while (EnterKey!=13);

        do
        {
            //resetting the count incase the user decided to retry inputting the username
            count=0;
            system("cls");//clearing the screen before reading the next file
            ReadFile(userinfo);
            printf("\n\t\t\t\t\t\t\t   Enter username: "); //asking for the username
            scanf("%s",usernameinput);// or gets(usernameinput); but then skips input on when the program loops to the beginning of the code

            //Calling a function that reads the username database and store the usernames into an array.
            //Also returns the Number of players in the datadase from the number of rows in the database.
            Username_File (user, AllPlayers, &NumberofPlayersinDatabase);
            //Calling a function that compares the user's username input to the username database & fills in the exists array with 3 if does not exist
            // and 2 if exists. This is then used to know the row number of the matched username (from the indexing number of exits), if any.
            compare(AllPlayers, usernameinput, NumberofPlayersinDatabase, exists);
            //Calling a function that  reads the scores database and store the scores into an array accoring to the player number, which is
            //taken from the row number from the above function
            Scores_File (score, AllPlayers, NumberofPlayersinDatabase);

            //going through all the rows and checking if exits for that specific row is 3 or 2
            for (n=0; n<NumberofPlayersinDatabase; n++)
            {
                //if the username exists print that it exists and display its score
                if(exists[n]==2)
                {
                    printf("\n\tUsername already exits\n");
                    Index_no=n;
                    printf("\t High Score is : %.0f\n", AllPlayers[Index_no].Scores*1000);
                }
                else {count++;} //otherwise just increment count
            }
            //if count has been incremented for all the rows (no username match), print the following statements
            if (count==NumberofPlayersinDatabase)
            {
                printf("Welcome New player!\n");
                Index_no=n+1; //small adjustments to fit its use later //might mess up info in the CurrentPlayer structure data, but we do not really care for that
                printf("\tCreate a new username ");
                NewPlayer=1; //inform that this is a new player. Used later
            }
            else //if a username does match
            {
                printf("\tUse this username ");
                NewPlayer=0; //not a new player
            }

            printf("or Try again?");
            //asks user if they want to re input their username
            printf("\n\t\t\t\t\t\tContinue: Y \t\t\t Try Again: N ");
            do
            {
              Again= getch();
            }
            while (!(Again==89 || Again==121 || Again==78 || Again==110)); // Y, y, N, &n

        }while(Again==78 || Again==110);//repeat if voted N/n --> Try again

        //asking for rounds and defending against values less than 0
        printf("\n\n\n");
        do
        {
            printf("\t\t\t\t\t\t   How many rounds would you like to play? : ");
            scanf("%d",&Plays);
        }while(Plays<=0); //not defended against non integer inputs!

        //Asking for difficulty level and not accepting inputs other than E,e,M,m,H,or h
        printf("\n\n\t\t\t\t\t\t\t      Pick Difficulty Level");
        printf("\n\t\t\t\t\t\t   Easy: E  \t  Medium: M   \t  Hard: H");
        do
        {
            PickDifficulty=getch();
        }
        while (!(PickDifficulty==69 || PickDifficulty==101 || PickDifficulty==77 || PickDifficulty==109 || PickDifficulty==72 || PickDifficulty==104)); //E,e,M,m,H,or h
        //setting the difficulty level accordingly
        if (PickDifficulty==69 || PickDifficulty==101) //Easy
        {
            Difficulty=Easy;
        }
        else if (PickDifficulty==77 || PickDifficulty==109)//Medium
        {
            Difficulty=Medium;
        }
        else //Hard
        {
            Difficulty=Hard;
        }

        //asking to view rules
        printf("\n\n\n\t\t\t\t\t\t       Would you like to view the Rules?\n");
        printf("\n\t\t\t\t\t\t    Yes: Y \t\t\t   No: N ");
        do
        {
          Rules= getch();
        }
        while (!(Rules==89 || Rules==121 || Rules==78 || Rules==110)); // Y, y, N, &n

        if (Rules==89 || Rules==121)
        {
            //Display Rules
            system("cls");//clearing the screen before reading the next file
            ReadFile(rulesFile);

            do //asking for either a space bar or Enter Key input
            {
                continueafterRules= getch();
            }
            while(!(continueafterRules==32 || continueafterRules==13));
            if (continueafterRules==32) //spacebar is pressed
            {
                //the user wants to see the stances & this is a function that does just that
                ShowingStances (PrepareStance , AttackStance, DefendStance, SaveStance);
                do
                {
                    continueafterRules= getch();
                }
                while(!(continueafterRules==13));
            }
        }

        do //looping because player said they wanted to keep playing with the same username
        {
            //if user is out of lives, but wants to play again, restore the lives and reset the score to 0
            if(Lives<=0)
            {
               Lives=4;
               CurrentPlayer.TotalPoints=0;
            }
            if(Sameusername==1) //if still playing with the same username, make the user interact before the game begins to make sure they are ready
            {
                printf("\n\t\t\t\t\t\t\t      Press Enter to Begin!");
                do
                {
                    continueafterRules= getch();
                }
                while(!(continueafterRules==13));
            }

            //proceeding after rules, viewing stances or after saying no to viewing rules
            if (continueafterRules==13 || Rules==78 || Rules==110)
            {
                //Begining the game and going the amount of rounds (Plays) the user picked
                for(loop=0; loop<Plays; loop++)
                {
                    //But if the user loses all their lives, break out of the loop! (skip the scenes until the for loop ends) (didn't want to use break)
                    if(!(Lives<=0))
                    {
                        //Function that randomly picks a prepare scene and returns the number of the scene picked
                        PrepareSceneno=ShowPrepareScene (D_P, FD_P, P_P, Lives);
                        //according to the number of scene picked, the user's input/action is evaluated
                        if (PrepareSceneno==1)
                        {
                            Dragon_Action_Score (Difficulty, &CurrentPlayer, D_P, D_A, D_D, D_S, Lives);
                            sleep(2);
                        }
                        else if(PrepareSceneno==2)
                        {
                            FireDragon_Action_Score (Difficulty, &Lives, &CurrentPlayer, FD_P, FD_A, FD_D, FD_S, FD_Died);
                            sleep(2);
                        }
                        else
                        {
                            Pedestrian_Action_Score (Difficulty, &CurrentPlayer, P_P, P_A, P_D, P_S,Lives);
                            sleep(2);
                        }
                    }
                }

            }
            //if user lost all their lives during the game and are dead, show the 'Dead Game over' scene
            if (Lives<=0)
            {
               system("cls");
               ReadFile(deathFile);
               printf("\n\t\t\t\t\t\t\t\t    Press Enter to Proceed");
                do
                {
                    EnterKey= getch();
                }
                while(!(EnterKey==13));
            }
            //Show the Highest score, whether the high score was the one just gained or the one scored in the database
            system("cls");
            ReadFile(scoreFile); //reading the score template
            //If username used is new, add the username to the database and add a new score line and set it to zero temporarily
            if (NewPlayer==1)
            {
                //Store the info of the current player
                strcpy(CurrentPlayer.username, usernameinput);
                strcpy(AllPlayers[Index_no].username,CurrentPlayer.username);

                //Store the new username in database
                fseek(user,0,SEEK_END);
                fprintf(user,"%s\n",AllPlayers[Index_no].username);//THERE MAUST ALWAYS BE A NEW LINE IN THE FILE!!!
                fclose(user);
                user=fopen("Usernames.txt","a+");

                //Storing a score of 0
                Index_no--;
                fclose(score);
                score=fopen("Scores.txt","r+");
                AllPlayers[Index_no].Scores=0;
                fseek(score,0,SEEK_END);
                fprintf(score,"\n%f",AllPlayers[Index_no].Scores);
                fclose(score);
            }

            //if the current score is greater than the recorded one
            if((CurrentPlayer.TotalPoints/1000)>AllPlayers[Index_no].Scores)
            {
                AllPlayers[Index_no].Scores = CurrentPlayer.TotalPoints/1000;//overwrite it in the database
            }
            //here is the code for overwriting it in the database
            score=fopen("Scores.txt","r+");
            fseek(score,+10*(Index_no),SEEK_SET); //going to the equivalent row of the username (actually, before it by a row, because then it overwrites the row after)
            fprintf(score,"%f",AllPlayers[Index_no].Scores);
            fclose(score);

            //printing the high score to the screen (useful text on screen accompanying this is already in the template file that was read and inputted to the screen)
            printf("%.0f",AllPlayers[Index_no].Scores*1000);
            printf("\n\t\t\t\t\t\t\t  Press Enter to Proceed");
            do
            {
                EnterKey= getch();
            }
            while(!(EnterKey==13));

            //if the user runs out of lives and have been shown the 'Dead Game over' scene and their score, they are asked if they
            //want to play again (instead of being asked if they want to continue)
            if (Lives<=0)
            {
                system("cls");
                ReadFile(outroFile);
                do
                {
                    EndGame= getch();

                }
                while(!(EndGame==89 || EndGame==121 || EndGame==78 || EndGame==110 ||EndGame==27 )); //Y ,y , N, n, or Esc buttons

                //play again with the same username, difficulty lever and number of rounds per game?
                if(EndGame==89 || EndGame==121)
                {
                    printf("\n\n\n\t\t\t\t\t\t       Using the same username and Difficulty level?");
                    printf("\n\t\t\t\t\t\t\t  Yes: Y \t\t\t   No: N ");
                    do
                    {
                      Sameusername= getch();
                    }
                    while (!(Sameusername==89 || Sameusername==121 || Sameusername==78 || Sameusername==110)); // Y, y, N, &n
                }
            }
            else //but if the user has not run out of lives, they are asked if they want to continue playing
            {
                system("cls");
                ReadFile(conti_outro);
                //continue playing?
                do
                {
                    Sameusername= getch();

                }
                while(!(Sameusername==89 || Sameusername==121 || Sameusername==78 || Sameusername==110)); //Y, y,N and n

            }

             if (Sameusername==89 || Sameusername==121) //If yes
            {
                NewPlayer=0;
                Sameusername=1;
                printf("\n\t\t\t\t\t\t\t    Prepare for %d more rounds!", Plays);
            }
            else if (Sameusername==78 || Sameusername==110)//if no, do you want to start a new game then?
            {
                Sameusername=0;
                printf("\n\t\t\t\t\t\t\t\t   New Game?");
                printf("\n\t\t\t\t\t\t\t  Yes: Y \t\t  No: N ");
                do
                {
                    EndGame= getch();

                }
                while(!(EndGame==89 || EndGame==121 || EndGame==78 || EndGame==110 ||EndGame==27 )); //Y ,y , N, n, or Esc buttons
            }
        }while(Sameusername==1);

        // Closing all the files
        fclose(introFile);
        fclose(rulesFile);
        fclose(deathFile);
        fclose(scoreFile);
        fclose(AttackStance);
        fclose(PrepareStance);
        fclose(DefendStance);
        fclose(SaveStance);
        fclose(outroFile);
        fclose(conti_outro);
        fclose(D_P);
        fclose(D_A);
        fclose(D_D);
        fclose(D_S);
        fclose(FD_P);
        fclose(FD_A);
        fclose(FD_D);
        fclose(FD_S);
        fclose(FD_Died);
        fclose(P_P);
        fclose(P_A);
        fclose(P_D);
        fclose(P_S);


    }
    while(EndGame==89 || EndGame==121 ); //restarts game if user presses on Y or y (picks yes)

    //if picked no, show the 'Game ended' scene and end the program
    endFile=fopen("EndGame.txt","r");
    system("cls");
    ReadFile(endFile);
    fclose(endFile);
}


///Function Definitions

//Function for reading Files and printing their data to the screen //mostly used with the scene files
void ReadFile (FILE *myfile)
{
    char i;
    rewind(myfile);//setting cursor to beginning of the file!

    if (myfile ==NULL)//if file does not exist display message and end program
    {
        printf("File does not exist. \nEnding Program.\n");
        exit(0);
    }
    else//otherwise scan it and print it to the screen
    {
        while (fscanf(myfile,"%c",&i) !=EOF)
        {
            printf("%c",i);
        }
    }
}

//Function for showing all three stances of the player
void ShowingStances (FILE *file1, FILE *file2, FILE *file3, FILE *file4)
{
    char Spacebar;

    //1st file
    system("cls");
    ReadFile (file1); //showing the 1st stance

    // do while used so that it is still possible for the user to press the spacebar
    //and move on if they pressed on any other key by mistake

    //2nd file
    do
    {
        Spacebar= getch();

    }
    while (Spacebar!=32); //spacebar acsii code
    system("cls");//clearing the screen before reading the next file
    ReadFile(file2);//showing the 2nd stance

    //3rd file
    do
    {
        Spacebar= getch();

    }
    while (Spacebar!=32); //spacebar acsii code
    system("cls");//clearing the screen before reading the next file
    ReadFile(file3);//showing the 3rd stance

    //4th file
    do
    {
        Spacebar= getch();

    }
    while (Spacebar!=32); //spacebar acsii code
    system("cls");//clearing the screen before reading the next file
    ReadFile(file4);//showing the 4th stance
}

//Function that randomly generates a number between 1 and 3 (there are 3 'Prepare' scenes) and then according to the number,
//displays one of the scenes to the user and returns the scene number generated
int ShowPrepareScene (FILE *file1, FILE *file2, FILE *file3, int Lives)
{
    int Scene_no;

    srand(time(NULL));
    Scene_no= (rand() %(3-1+1))+1;

    switch (Scene_no)
    {
        case 1:
            system("cls");
            Life(Lives);
            ReadFile(file1);
            break;
        case 2:
            system("cls");
            Life(Lives);
            ReadFile(file2);
            break;
        case 3:
            system("cls");
            Life(Lives);
            ReadFile(file3);
            break;
    }

    return Scene_no;
}

//Function that evaluates the input/action of the user to the Dragon prepare scene
//It sets a timer for the user to react according to the difficulty and checks if the user reacts in time
//if the user attacks (in time), +100 points and are told how quickly they reacted
// if they defend or save (in time), -50 points and are told how quickly they reacted
//if they dont react in time, -50 and are told they did not react in time
void Dragon_Action_Score (int Difficulty, struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, int Lives)
{
    int Time=0;
    int Action=0;
    float Timeinsec;
    clock_t StartTime, EndTime, TimeElapsed; //setting timer
    rewind(file1);
    rewind(file2);
    rewind(file3);
    rewind(file4);

    printf("\t\t\tReact!\n"); //////////////////////////////////////////new file?////////////////////////////////////////////////////////////////////////////////////
    StartTime=clock();//activating timer
    do
    {
        if (kbhit()) //if there was a key stroke
        {Action= getch();} //assign its value to action and check it against the while condition below
        EndTime=clock();
        TimeElapsed = (double) (EndTime-StartTime);//calculating the time elapsed
    //keep asking for a user's input until they give you one or time runs out
    }while(!(Action==65 || Action==97 || Action==68 || Action==100|| Action==83 || Action==115 ||TimeElapsed>=Difficulty*1000)); //time in millisecond

    switch (Action)
    {
        //Attack Stance: A or a
        case 65:
        case 97:
            system("cls");
            Life(Lives); //keeps the lives up to date
            ReadFile(file2);
            User->DragonPoints++;   //change to 5 or somth if the dragon has 3 lives//////////////////////////////////////////////////////////////////////////////////////
            printf("\n\n\tGained a 100 points!");
            break;

        //Defend Stance: D or d
        case 68:
        case 100:
            system("cls");
            Life(Lives);
            ReadFile(file3);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
            break;

        //Save Stance: S or s
        case 83:
        case 115:
            system("cls");
            Life(Lives);
            ReadFile(file4);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
            break;

        // Prepare Stance //not reacting
        default:
            system("cls");
            Life(Lives);
            ReadFile(file1);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
    }
    //calculating and updating points
    User->TotalPoints = 100*(User->DragonPoints+ User->pos_PedestrianPoints -User->neg_PedestrianPoints - User->WrongmovePoints);
    if(User->TotalPoints/1000<0) //if score drops below zero, just keep it as zero
    {
        User->TotalPoints=0;
    }
    Timeinsec= (float) TimeElapsed/1000; //converting time to seconds
    printf("\nTotal Points = %.0f",User->TotalPoints); //printing the score
    if (Timeinsec<Difficulty) //if the user reacted in time
    {
       printf("\nYou Reacted in %.2f seconds\n", Timeinsec ); //print how fast they reacted
    }
    else //if time run out before user reacts
    {
        printf("\nTimes up! \nYou did not react fast enough!");//tell them that time had run out
    }



}

//Function that evaluates the input/action of the user to the Fire Breathing Dragon prepare scene
//It sets a timer for the user to react according to the difficulty and checks if the user reacts in time
//user looses a life if they do anything other than 'Defend'
//if the user attacks they dont gain or lose any points and are told how quickly they reacted
// if they defend (in time), +100 points and are told how quickly they reacted
//if they save or dont react in time, -50 and are told how quickly they reacted or that they did not react in time accordingly
//rest of the function is similar to the one above
void FireDragon_Action_Score (int Difficulty,int *Lives, struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, FILE *file5)
{
    int Time=0;
    int Action=0;
    int Lives_here=*Lives;//had to use a local value for lives temporarily. Updates *Lives at the end
    float Timeinsec;
    clock_t StartTime, EndTime, TimeElapsed;
    rewind(file1);
    rewind(file2);
    rewind(file3);
    rewind(file4);

    printf("\t\t\tReact!\n");
    StartTime=clock();
    do
    {
        if (kbhit())
        {Action= getch();}
        EndTime=clock();
        TimeElapsed = (double) (EndTime-StartTime);

    }while(!(Action==65 || Action==97 || Action==68 || Action==100|| Action==83 || Action==115 ||TimeElapsed>=Difficulty*1000)); //time in millisecond

    switch (Action)
    {
        //Attack Stance: A or a
        case 65:
        case 97:
            Lives_here--;
            system("cls");
            Life(Lives_here);
            ReadFile(file5);
            break;

        //Defend Stance: D or d
        case 68:
        case 100:
            system("cls");
            Life(Lives_here);
            ReadFile(file3);
            User->DragonPoints++;   //change to 5 or somth if the dragon has 3 lives//////////////////////////////////////////////////////////////////////////////////////
            printf("\n\n\tGained a 100 points!");
            break;

        //Save Stance: S or s
        case 83:
        case 115:
            Lives_here--;
            system("cls");
            Life(Lives_here);
            ReadFile(file5);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
            break;

        // Prepare Stance
        default:
            Lives_here--;
            system("cls");
            Life(Lives_here);
            ReadFile(file5);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
    }
    *Lives=Lives_here; //Updating *Lives (updating the Lives value in main)
    User->TotalPoints = 100*(User->DragonPoints+ User->pos_PedestrianPoints -User->neg_PedestrianPoints - User->WrongmovePoints);
    if(User->TotalPoints/1000<0)
    {
        User->TotalPoints=0;
    }
    Timeinsec= (float) TimeElapsed/1000;
    printf("\nTotal Points = %.0f",User->TotalPoints);
    if (Timeinsec<Difficulty)
    {
       printf("\nYou Reacted in %.2f seconds\n", Timeinsec );
    }
    else
    {
        printf("\nTimes up! \nYou did not react fast enough!");
    }



}

//Function that evaluates the input/action of the user to the Pedestrian prepare scene
//It sets a timer for the user to react according to the difficulty and checks if the user reacts in time
//if the user attacks (in time), -100 points and are told how quickly they reacted
// if they save (in time), +100 points and are told how quickly they reacted
//if they defend or dont react in time, -50 and are told how quickly they reacted or that they did not react in time accordingly
//rest of the function is similar to the two above
void Pedestrian_Action_Score (int Difficulty,struct Userinfo *User, FILE *file1, FILE *file2, FILE *file3, FILE *file4, int Lives)
{
    int Time=0;
    int Action=0;
    float Timeinsec;
    clock_t StartTime, EndTime, TimeElapsed;
    rewind(file1);
    rewind(file2);
    rewind(file3);
    rewind(file4);

    printf("\t\t\tReact!\n"); //////////////////////////////////////////new file?////////////////////////////////////////////////////////////////////////////////////
    StartTime=clock();
    do
    {
        if (kbhit())
        {Action= getch();}
        EndTime=clock();
        TimeElapsed = (double) (EndTime-StartTime);

    }while(!(Action==65 || Action==97 || Action==68 || Action==100|| Action==83 || Action==115 ||TimeElapsed>=Difficulty*1000)); //time in millisecond

    switch (Action)
    {
        //Attack Stance: A or a
        case 65:
        case 97:
            system("cls");
            Life(Lives);
            ReadFile(file2);
            User->neg_PedestrianPoints++;
            printf("\n\n\tLost a 100 points!");
            break;

        //Defend Stance: D or d
        case 68:
        case 100:
            system("cls");
            Life(Lives);
            ReadFile(file3);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
            break;

        //Save Stance: S or s
        case 83:
        case 115:
            system("cls");
            Life(Lives);
            ReadFile(file4);
            User->pos_PedestrianPoints++;
            printf("\n\n\tGained a 100 points!");
            break;

        // Prepare Stance
        default:
            system("cls");
            Life(Lives);
            ReadFile(file1);
            User->WrongmovePoints = User->WrongmovePoints+0.5;
            printf("\n\n\tLost a 50 points!");
    }
    User->TotalPoints = 100*(User->DragonPoints+ User->pos_PedestrianPoints -User->neg_PedestrianPoints - User->WrongmovePoints);
    if(User->TotalPoints/1000<0)
    {
        User->TotalPoints=0;
    }
    Timeinsec= (float) TimeElapsed/1000;
    printf("\nTotal Points = %.0f",User->TotalPoints);
    if (Timeinsec<Difficulty)
    {
        printf("\nYou Reacted in %.2f seconds\n", Timeinsec );
    }
    else
    {
        printf("\nTimes up! \nYou did not react fast enough!");
    }



}

//Function to read the username database and store the usernames into an array and update the number of players where it is being called
//(in main) (by using the number of rows)
void Username_File (FILE* user, struct Players *Players, int *Players_no)
{
    int n=0, u=0;
    char i;

    rewind(user);

    if (user==NULL)
    {
        printf("File does not exist. \nEnding Program.\n");
        exit(0);
    }
    else
    {
         while (fscanf(user,"%c",&i) !=EOF)
        {
            if (i !='\n')
            {
               (*(Players+n)).username[u]=i;
               u++;

            }
            else
            {
                (*(Players+n)).arraysize=u;
                u=0;
                n++;
            }
        }
        *Players_no=n;
    }

}

//Function to compare the user's username input to a database and check if it already exists. if it does, return 2, if not return 3.
//and by return, it is meant that the array element according the the row number or the username will be updated
void compare(struct Players Players[], char Input[], int Players_no, int *exists)
{
    for (int n=0; n<Players_no; n++)  //size of players array
    {
        //Checking both ways to insure the usernames only match if they EXACTLY match, letter for letter
        if(!(strncmp(Players[n].username,Input,Players[n].arraysize )))
        {
            if(!(strncmp(Players[n].username,Input,strlen(Input) )))
            {
                (*(exists+n))=2;
            }
            else
            {
                exists[n]=3;
            }
        }
        else
        {
            exists[n]=3;
        }
    }
}

//Function to read the scores database and store the scores into an array according to the player number
void Scores_File (FILE* score, struct Players *Players, int Players_no)
{
    float i;
    int n;
    rewind(score);

    if (score ==NULL)
    {
        printf("File does not exist. \nEnding Program.\n");
        exit(0);
    }
    else
    {
         for(n=0; n<Players_no; n++) //going through all the rows
        {
            fscanf(score,"%f", &i);
            (*(Players+n)).Scores= i;
        }
    }

}

//Function that just prints a bar of hearts on the top right of the screen to show the user how many lives they have left.
void Life(int Live)
{
    char heart=3;
    int h;

    printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t Lives: ");
    for(h=0; h<Live; h++)
    {
        printf(" %c ",heart);
    }
    printf("\n");
}

