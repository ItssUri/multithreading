#include <iostream>  // Includes the standard input-output stream library, allowing the use of cout, cin, and other I/O functions.
#include <thread>    // Includes the thread library, enabling the creation and management of threads for concurrent programming.
#include <time.h>    // Includes the C standard library for time-related functions, providing tools to work with time and date.
#include <windows.h> // Includes the Windows API header, allowing access to Windows-specific functions, data types, and constants.
#include <mutex>     // Includes the mutex library, providing mechanisms for mutual exclusion to protect shared data in multi-threaded environments.

const int TRACK_LEN = 100;                           // This can be modified to make the race faster/shorter.
bool finishedRace = false;                           // This will keep track
std::string winningChar = "";                        // This variable will collect the winning Character
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);   // We will use a console handle to move our cursor between lines in the console.
std::mutex m;                                        // A mutex is a class that prevents multiple threads from accessing shared resources simultaneously, ensuring thread safety.

void racer1() {
    COORD coord;                                      // We create a COORD type variable to set our console position
    coord.X = 0;                                      // Start of the line
    coord.Y = 2;                                      // In line 2
    m.lock();                                         // This will ensure other threads from doing the same, thus preventing any cout errors in incorrect lines
    SetConsoleCursorPosition(hConsole, coord);        // We set our cursor to the previously defined coord variable
    std::cout << "(Johnny Joestar) TRACK 1: ";       
    coord.X = 30;                                     // Since we printed the participant name, we must advance our X coordinate to avoid overlapping characters
    m.unlock();                                       // We unlock other threads from printing since not unlocking could lead to fatal errors
    std::string msg = "@";                            // This is the character the racer will use on the track
    srand(time(NULL));                                // To ensure generation of different pseudo-random ints
    int vel = rand()%(400 + 1) + 250;                 // We generate a random velocity for the racer
    for (int i = 0; i < TRACK_LEN; i++)               // Modifying the TRACK_LEN variable will change this
    {
        m.lock();                                      // We lock again to print our character in the desired coord
        SetConsoleCursorPosition(hConsole, coord);
        std::cout << msg; 
        m.unlock();
        Sleep(vel);                                    // We call Sleep to avoid insta-finishing the race and always having the same winner. This will determine our "velocity"
        coord.X++;                                     // Finally, we advance
        if(finishedRace) {
            break;                                     // We break from loop to avoid printing messages after another runner has finished the race
        }
    }
    if (!finishedRace)
    {
        winningChar = "Johnny Joestar";                // We set the global winningChar var to be our racer's name
        finishedRace = true;                           // We change the state of finishedRace to avoid other runners from keeping on printing their characters
    }
}

void racer2() {
    COORD coord;
    coord.X = 0;
    coord.Y = 3;
    m.lock();
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "(Gyro Zeppeli)   TRACK 2: ";
    coord.X = 30;
    m.unlock();
    std::string msg = "#";
    srand(time(NULL));
    int vel = rand()%(400 + 1) + 160;
    for (int i = 0;i < TRACK_LEN; i++)
    {
        m.lock();
        SetConsoleCursorPosition(hConsole, coord); // THIS FUNCTION'S CODE IS EQUIVALENT TO FUNCTION1, PLEASE REFER TO IT FOR COMMENTS
        std::cout << msg;
        m.unlock();
        Sleep(vel);
        coord.X++;
        if(finishedRace) {
            break;
        }
    }
    if (!finishedRace)
    {
        winningChar = "Gyro Zeppeli";
        finishedRace=true;
    }
}

void racer3() {
    COORD coord;                                                  
    coord.X = 0;                                                  
    coord.Y = 4;                                                  
    m.lock();                                                   
    SetConsoleCursorPosition(hConsole, coord);                 
    std::cout << "(Diego Brando)   TRACK 3: ";                 
    coord.X = 30;                                              
    m.unlock();                                                 
    std::string msg = "~";                                     
    bool canGo = true;                                       // This function works slightly different than the others. We use a bool variable to alternate between fast/slower sleep times.
    srand(time(NULL));                                         
    int vel = rand()%(400 + 1) + 160;                          
    for (int i = 0; i < TRACK_LEN; i++)                       
    {
        m.lock();                                               
        SetConsoleCursorPosition(hConsole, coord);           // THIS FUNCTION'S CODE IS EQUIVALENT TO FUNCTION1, PLEASE REFER TO IT FOR COMMENTS
        std::cout << msg;                                     
        m.unlock();                                           
        if (!canGo)                                          // Even loops will Sleep for 250 while Odd loops will Sleep for 350
        {
            Sleep(350);
        } else { 
            Sleep(250);
        }
        canGo = !canGo;                                      // This will alternate the variable state every loop
        // Sleep(vel); <== This can be uncommented to have more equal results between racers. If uncommented, comment previous sleep statements.
        coord.X++;                                            
        if (finishedRace) {                                   
            break;                                            
        }
    }

    if (!finishedRace)
    {
        winningChar = "Diego Brando";
        finishedRace=true;
    }
}

int main() {
    srand(time(NULL));                                                   
    system("cls");                                            // Since we are using custom lines, we must clear the terminal to get rid of our execution command and start from fresh. cls in windows stands for Clear Screen
    std::cout << "Hello and Welcome to the Seventh Edition of the procclaimed Steel Ball Run! Who is gonna win this spectacular race?\n";
    std::thread thread1(racer1);
    std::thread thread2(racer2);                             // We initialize all of our threads each with their own racer function
    std::thread thread3(racer3);
    while (!finishedRace)                                     // This will end all other threads when one of them has finished the race
    {
        thread1.join();
        thread2.join();                                      // We make sure we don't end threads before they are done, so we use the join function to avoid that from happening.
        thread3.join();
    }
    COORD coord;                                              
    coord.X = 0;                                              
    coord.Y = 6;                                            // We jump to line 6 to announce our winner.
    SetConsoleCursorPosition(hConsole, coord);
    std::cout << "Ladies and Gentlemen, we have a winner! " << winningChar << " is the new Steel Ball Run winner!" << std::endl;
    std::cout << std::endl;
    return 0; 
}