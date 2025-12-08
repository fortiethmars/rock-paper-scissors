#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    string playerChoice;
    string computerChoice;
    string choices[3] = {"rock","paper","scissors"};

    srand(time(0));
    int randomNum = rand() % 3;
    computerChoice = choices[randomNum];


    cout << "Welcome to Rock,Paper,Scissors!\n";
    cout << "Please choose one (Rock,Paper or Scissors): ";

    cin >> playerChoice;



    bool isValid = false;
    if (playerChoice == "rock" || playerChoice == "paper" || playerChoice == "scissors") {
        isValid = true;
    }

    if (!isValid) {
        cout << "Your entry is invalid, please try again!\n";
    } else {
        cout << "You chose: " << playerChoice << endl;
        cout << "Computer chose: " << computerChoice << endl;
    }
    
    
    

}