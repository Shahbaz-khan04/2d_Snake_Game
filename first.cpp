#include <iostream> // for input/ output of console
#include <conio.h> // for special functions like sleep(), kbhit(), getch()
#include <windows.h> // for designing like background colors text colors

using namespace std;

struct Node
{
    int x;
    int y;
    Node* next;
}; 
Node* head;

bool gameover;
int width, height;
int fruitX, fruitY;
int score;

/* enum example
    enumeration is a user-defined data type that enables you to have a fixed range of possible values, and the variable can select one value from the set of values.
    enum a {one= 1,two= 2};
    a value;
*/
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };  
eDirecton dir;
int difficultyChoice;
int speed;
int players = 0, turn = 0, scores[4];

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_SCREEN_BUFFER_INFO info;

// Anum
void setup();
void mainMenu();
// Noor
void instructions();
void difficulty(int choice);
void startGame();
// Hamza
void draw();
// Shahrukh
void input();
void gameoverWindow();
// Shahbaz
void algorithm();

void setup()
{
    gameover = false;
    dir = STOP;
    head = new Node;
    if(difficultyChoice == 1 || difficultyChoice == 2){
        width = 20;
        height = 10;
    } 
    else if(difficultyChoice == 3){
        width = 10;
        height = 8;
    }
//  snake head position
    head -> x = width / 2;   
	head -> y =  height / 2;
	head -> next =  NULL;
//  setting randomly generated position of fruit
	fruitX = rand() % width;
    fruitY = rand() % height;
//  Initializing score to 0
	score = 0;
//  Speed initialization
    if(difficultyChoice == 1)
        speed = 200;
    else if(difficultyChoice == 2)
        speed = 100;
    else if(difficultyChoice == 3)
        speed = 70;
}

void mainMenu(){
    int choice;
    bool correctChoice = false;
    system("cls");
    SetConsoleTextAttribute(h, 3);
    string asciiArt = R"(
     ___        _      _____                   _                _____                            
    |__ \      | |    / ____|                 | |              / ____|                           
       ) |   __| |   | (___    _ __     __ _  | | __   ___    | |  __    __ _   _ __ ___     ___ 
      / /   / _` |    \___ \  | '_ \   / _` | | |/ /  / _ \   | | |_ |  / _` | | '_ ` _ \   / _ \
     / /_  | (_| |    ____) | | | | | | (_| | |   <  |  __/   | |__| | | (_| | | | | | | | |  __/
    |____|  \__,_|   |_____/  |_| |_|  \__,_| |_|\_\  \___|    \_____|  \__,_| |_| |_| |_|  \___|                         
                                                                                              )";
    cout << asciiArt << endl;
    SetConsoleTextAttribute(h, 14);
	cout<<"\n\n\n\t\t\t\t\t    1. Quick PLAY";
    SetConsoleTextAttribute(h, 5);
	cout<<"\n\n\n\t\t\t\t\t    2. Competition";
	SetConsoleTextAttribute(h, 11);
	cout<<"\n\n\n\t\t\t\t\t    3. HELP";
	SetConsoleTextAttribute(h, 9);
	cout<<"\n\n\n\t\t\t\t\t    4. EXIT\n";
    // Default Color
	SetConsoleTextAttribute(h, 15);
    bool correctPlayers;
    do{
        cin>>choice;
        switch(choice){
            case 1:
                correctChoice = true;
                players = 1;
                difficulty(choice);
            break;
            case 2:
                correctChoice = true;
                // To check if user has entered correct no of players in do while loop
                correctPlayers = false;
                do{
                    cout << "Enter no of Players(max 4 allowed): ";
                    cin >> players; 
                    if(players >=2 && players <= 4){
                        correctPlayers = true;
                        difficulty(choice);
                        break;
                    }
                    else{
                        cout << "Minumum 2 and Maximum 4 players allowed...";
                    }
                }while(correctPlayers == false);
                break;
            case 3:
                correctChoice = true;
                instructions();
                break;
            case 4:
                gameover = true;
                correctChoice = true;
                break;
            default:
                cout << "Select one of the above options.";
                correctChoice = false;
                break;
        }
    }while(correctChoice == false);
}

void instructions(){
    system("cls");
    int choice = 0;
    SetConsoleTextAttribute(h, 12);
    string asciiart = R"(
     _____              _                       _    _                        
    |_   _|            | |                     | |  (_)                     _ 
      | |   _ __   ___ | |_  _ __  _   _   ___ | |_  _   ___   _ __   ___  (_)
      | |  | '_ \ / __|| __|| '__|| | | | / __|| __|| | / _ \ | '_ \ / __|    
     _| |_ | | | |\__ \| |_ | |   | |_| || (__ | |_ | || (_) || | | |\__ \  _ 
    |_____||_| |_||___/ \__||_|    \__,_| \___| \__||_| \___/ |_| |_||___/ (_)
                                                                           
                                                                           
    )";
    cout << asciiart << endl;
    SetConsoleTextAttribute(h, 15);
    cout << "Controls for the Snake Game are very Simple.\n";
    cout << "Press W to move Upwards.\n";
    cout << "Press A to move Left.\n";
    cout << "Press D to move Right.\n";
    cout << "Press S to move Downwards.\n";
    cout << "Press X to Stop Game at any moment.\n";
    cout << "\nDifficulty Options Details:\n";
    cout << "1. Easy(slow speed of snake, big box to move around, borders of the box are not blocked).\n";
    cout << "2. Normmal(fast speed of snake, big box to move around, borders of the box are blocked).\n";
    cout << "3. Impossible(incredibly fast speed of snake, small box to move around, borders of the box are blocked).\n";
    cout << "Note: In all difficulties speed of the snake will increase after eating food.\n";
    while(choice != 1) {
        cout << "\nGo Back to Main Menue? (Press 1) ";
        cin >> choice;
        if(choice == 1){
            mainMenu();
            break;
        }
        else{
            cout << "Not a valid option. ";
        }
    }
}

void difficulty(int mainMenueChoice){
    bool correctChoice = false;
    system("cls");
    SetConsoleTextAttribute(h, 12);
    string asciiart = R"(
      _____        _              _     _____   _   __   __  _               _  _               
     / ____|      | |            | |   |  __ \ (_) / _| / _|(_)             | || |           _  
    | (___    ___ | |  ___   ___ | |_  | |  | | _ | |_ | |_  _   ___  _   _ | || |_  _   _  (_) 
     \___ \  / _ \| | / _ \ / __|| __| | |  | || ||  _||  _|| | / __|| | | || || __|| | | |     
     ____) ||  __/| ||  __/| (__ | |_  | |__| || || |  | |  | || (__ | |_| || || |_ | |_| |  _  
    |_____/  \___||_| \___| \___| \__| |_____/ |_||_|  |_|  |_| \___| \__,_||_| \__| \__, | (_) 
                                                                                      __/ |     
                                                                                     |___/      
    )";
    cout << asciiart << endl;
    SetConsoleTextAttribute(h, 15);
    cout << "Press 1 for Easy\n";
    cout << "Press 2 for normal\n";
    cout << "Press 3 for Impossible\n";
    while(correctChoice == false){
        cout << "Enter: ";
        cin >> difficultyChoice;
        if(difficultyChoice == 1 || difficultyChoice == 2 || difficultyChoice == 3){
            correctChoice = true;
            if(mainMenueChoice == 1){
                startGame();
                gameoverWindow();
            }
            else if(mainMenueChoice == 2){
                turn = 0;
                int ready = 0;
                for(int i = 0; i < players; i++){
                    do{
                        cout << "\nEnter 1 if Player: " << i+1 << " is ready...\n";
                        cin >> ready;

                    }while(ready != 1);
                    startGame();
                    system("cls");
                    SetConsoleTextAttribute(h, 12);
                    system("cls");
                    string asciiart = R"(
     _______                    ______             _            _          
    |__   __|                  |  ____|           | |          | |         
       | | _   _  _ __  _ __   | |__    _ __    __| |  ___   __| |         
       | || | | || '__|| '_ \  |  __|  | '_ \  / _` | / _ \ / _` |         
       | || |_| || |   | | | | | |____ | | | || (_| ||  __/| (_| | _  _  _ 
       |_| \__,_||_|   |_| |_| |______||_| |_| \__,_| \___| \__,_|(_)(_)(_)
                                                                                                                                                                                                 
                    )";
                    cout << asciiart << endl;
                    SetConsoleTextAttribute(h, 15);
                    turn++; 
                }
                gameoverWindow();
            }
            break;
        }
        else
            correctChoice = false;
            cout << "Not a valid Option\n";
    }
}

void startGame(){
    setup();
    GetConsoleScreenBufferInfo(h, &info);
    SetConsoleTextAttribute(h, 0xA0);
    while (gameover == false)
    {
        draw();
        input();
        algorithm();
        Sleep(speed);
    }
    scores[turn] = score; 
    SetConsoleTextAttribute(h, info.wAttributes); // restore console info
    
}

void draw()
{
	system("cls");
    if(difficultyChoice == 1)
        cout << "Difficulty Selected: Easy\n\n";
    else if(difficultyChoice == 2)
        cout << "Difficulty Selected: Normal\n\n";
    else if(difficultyChoice == 3)
        cout << "Difficulty Selected: Impossible\n\n";

    if(players != 1){
        cout << "It is Player: " << turn + 1 << " turn to play." << endl;
    }

//  Printing top border of the box
    for (int i = 0; i < width+2; i++){
        if(i == 0 || i == width+1)
            cout << "+"; 
        else     
            cout << "-";
    }
    cout << endl;
//  printing right, left border and snake, fruit 
    for (int i = 0; i < height; i++)
    {
        cout << "|";
        for (int j = 0; j < width; j++)
        {
//          printing O to make snake's head
            if (i == head->y && j == head->x)
                cout << "O";
//          printing F to make fruit
            else if (i == fruitY && j == fruitX){
                GetConsoleScreenBufferInfo(h, &info);
                SetConsoleTextAttribute(h, (info.wAttributes) | FOREGROUND_RED);
                cout << "#";
                SetConsoleTextAttribute(h, info.wAttributes);
            }
            else
            {
                bool printTail = false;
                Node* tailSegment = head->next;

                while (tailSegment != NULL)
                {
                    if (i == tailSegment->y && j == tailSegment->x)
                    {
                            cout << "o";
                            printTail = true;
                            break;
                    }
                    tailSegment = tailSegment->next;
                }
                if (printTail == false)
                    cout << " ";
            }
        }
        cout << "|";
        cout << endl;
    }
//  Printing bottom border of the box
    for (int i = 0; i < width+2; i++){
        if(i == 0 || i == width+1)
            cout << "+";
        else
            cout << "-";
    }
    cout << endl;
//  Printing Score
    cout << "\tScore:" << score << endl;
}

void input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameover = true;
                break;
            default:
                break;
        }
    }
}

void algorithm()
{
    // head positions
    int prevX = head->x; 
    int prevY = head->y;
    int prev2X, prev2Y;
    Node* current = head->next;
//  This while loop updates tail segment's positions
    while (current != NULL)
    {
        prev2X = current->x;
        prev2Y = current->y;

        current->x = prevX;  
        current->y = prevY;

        prevX = prev2X;
        prevY = prev2Y;
//      now we move to the next tail segment.
        current = current->next;  
    }

//	This Switch Case updates position of the head of snake
    switch (dir)
    {
    case LEFT:
        head->x--;
        break;
    case RIGHT:
        head->x++;
        break;
    case UP:
        head->y--;
        break;
    case DOWN:
        head->y++;
        break;
    default:
        break;
    }
//	condition for when snake goes through right border
    if (head->x >= width){
        if(difficultyChoice == 1)
            head->x = 0;
        else if(difficultyChoice == 2 || difficultyChoice == 3) 
            gameover = true;
    }        
//	condition for when snake goes through left border
    else if (head->x < 0){
        if(difficultyChoice == 1)
            head->x = width - 1;
        else if(difficultyChoice == 2 || difficultyChoice == 3) 
            gameover = true;
    }
//	condition for when snake goes through bottom border
    if (head->y >= height){
        if(difficultyChoice == 1)
            head->y = 0;
        else if(difficultyChoice == 2 || difficultyChoice == 3) 
            gameover = true;
    }        
//	condition for when snake goes through top border
    else if (head->y < 0){
        if(difficultyChoice == 1)
            head->y = height - 1;
        else if(difficultyChoice == 2 || difficultyChoice == 3) 
            gameover = true;
    }        
        
//	Condition for when snake eats its tail
    current = head->next;
    while (current != NULL)
    {
        if (current->x == head->x && current->y == head->y)
            gameover = true;
        current = current->next;
    }
//	Condition for when snake eats a fruit
    if (head->x == fruitX && head->y == fruitY)
    {
        score += 10;

        fruitX = rand() % width;
        fruitY = rand() % height;

        Node* newTail = new Node{prevX, prevY, NULL};
        current = head;

        while (current->next != NULL)
            current = current->next;
        current->next = newTail;

        if(speed > 10){
            if(difficultyChoice == 1 || difficultyChoice == 2){
                    speed -= 10;
            }
            else if(difficultyChoice == 3){
                    speed -= 30;
            }
        }
    }
}

void gameoverWindow(){
    int choice;
    SetConsoleTextAttribute(h, 12);
    system("cls");
    string asciiart = R"(
      _____                           ______             _            _          
     / ____|                         |  ____|           | |          | |         
    | |  __   __ _  _ __ ___    ___  | |__    _ __    __| |  ___   __| |         
    | | |_ | / _` || '_ ` _ \  / _ \ |  __|  | '_ \  / _` | / _ \ / _` |         
    | |__| || (_| || | | | | ||  __/ | |____ | | | || (_| ||  __/| (_| | _  _  _ 
     \_____| \__,_||_| |_| |_| \___| |______||_| |_| \__,_| \___| \__,_|(_)(_)(_)
                                                                                                                                                  
    )";
    cout << asciiart << endl;
    SetConsoleTextAttribute(h, 15);
    if(players!= 1){
        for(int i= 0; i < players; i++){
            cout << "Final Score of player: " << i+1  << " is "<< scores[i] << endl;
        }

        int playerIds[4];
        int playerId = 0;
        playerIds[0] = -1;
        int max = scores[0];
        // Finding Higest score amongst players
        for (int i = 1; i < players; ++i) {
            if (scores[i] > max) {
                max = scores[i];
                playerId = i;
            }
        }

        // Finding if other players scored same as highest
        int j = 0; // no of players that scored max.
        for(int i = 0; i < players; i++){
            if(max == scores[i]){
                playerIds[j] = i+1;
                j++;
            }
        }
        if(j==1){
            cout << "Player: " << playerId+1 <<  " WON with Score: " << max << endl;
        }
        else{
            cout <<  "Player: " ;
            for(int i = 0; i<j; i++){
                cout << playerIds[i] << " ";
            }
            cout <<  "TIED with Score: " << max << endl;
        }
    }
    else
        cout << "Final score was: " << scores[0] << endl;
    while(gameover == true){
        cout << "\nPress 1 for Play again\n";
        cout << "Press 2 for Change Difficulty\n";
        cout << "Press 3 to go back to main menu\n";
        cin >> choice; 
        if(choice == 1 ){
            gameover = false;
            if(players!= 1){
                turn = 0;
                for(int i = 0; i < players; i++){
                    startGame();
                    turn++; 
                }
                gameoverWindow();
            }
            else{
                startGame();
                gameoverWindow();
            }    
        }
        else if(choice == 2){
            gameover = false;
            if(players!=1)
                difficulty(2);
            else
                difficulty(1);
        }
        else if(choice == 3){
            mainMenu();
            gameover = false;
        }
        else
            cout << "Not a valid Option. \n";
    }
}

int main()
{
    mainMenu();
    system("cls");
    string asciiArt2 = R"(
     _______   _                       _       __     __                                     
    |__   __| | |                     | |      \ \   / /                                     
       | |    | |__     __ _   _ __   | | __    \ \_/ /    ___    _   _                      
       | |    | '_ \   / _` | | '_ \  | |/ /     \   /    / _ \  | | | |                     
       | |    | | | | | (_| | | | | | |   <       | |    | (_) | | |_| |                     
       |_|    |_| |_|  \__,_| |_| |_| |_|\_\      |_|     \___/   \__,_|                     

      __                    _____    _                   _                     _     _     _ 
     / _|                  |  __ \  | |                 (_)                   | |   | |   | |
    | |_    ___    _ __    | |__) | | |   __ _   _   _   _   _ __     __ _    | |   | |   | |
    |  _|  / _ \  | '__|   |  ___/  | |  / _` | | | | | | | | '_ \   / _` |   | |   | |   | |
    | |   | (_) | | |      | |      | | | (_| | | |_| | | | | | | | | (_| |   |_|   |_|   |_|
    |_|    \___/  |_|      |_|      |_|  \__,_|  \__, | |_| |_| |_|  \__, |   (_)   (_)   (_)
                                                  __/ |               __/ |                  
                                                 |___/               |___/                   
                                                                                              )";
    SetConsoleTextAttribute(h, 3);
    cout << asciiArt2 << endl; 
    SetConsoleTextAttribute(h, 15);
    return 0;
}
