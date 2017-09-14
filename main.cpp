#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <sstream>

#define MAX_PLAYERS 10

using namespace std;

/*
	Class representing an N-sided die
*/
class Dice{
    private:
        int numSides;

    public:
        // Perform a single roll of the die
        Dice(int n){
            numSides = n;
        }
        int roll() { //randomized roll based on sides
            srand((unsigned)time(0));
            return (random() % numSides) + 1;
        };
};

/*
	Class representing a Game Player
*/
class Player{
    private:
        string name;
        int score;
        bool alive; //variable for tracking alive status for Knockout
    public:
        Player(){ //Constructor for adding a player
            name = "";
            score = 0;
            alive = true;
        }
        Player(string n, int s){ //Constructor with parameters
            name = n;
            score = s;
            alive = true;
        }
        // Getter : Player Name
        string getName() {
            return name;
        }
        // Getter : Score
        int getScore() {
            return score;
        }
        void setName(string n){
            name = n;
        }
        void setScore(int s){
            score = s;
        }
        // Increases the player's score by a specified value
        void addToScore(int val){
            score += val;
        }
        void setAlive(bool status){
            alive = status;
        }
        bool getAlive(){ //check for if player is alive
            return alive;
        }
};

/*
	Abstract class DiceGame
*/
class DiceGame{

    protected:
        Player *players[MAX_PLAYERS];
        Dice *dice;
        int numPlayers, numDice;

    public:
        //	Pure virtual function
        virtual void play() = 0;

        //Get input for all players from the user
        void initPlayers(){
            do {
                cout << "Enter number of players: ";
                cin >> numPlayers;
            }while(numPlayers > 10);

            for(int i = 0; i < numPlayers; i++){
                int score;
                string name;
                cout << "Enter name of Player " << i+1 << ": ";
                cin >> name;
                players[i] = new Player(name,0);
            }
        }

        //Format and output all scores
        void displayScores(){
            for(int i = 0; i < numPlayers; i++){
                cout << players[i]->getName() << " : " << players[i]->getScore() << endl;
            }
        }

        //Allow user to enter number of dice and number of sides
        void requestDice(){
            int numSides;
            cout << "How many dice? : ";
            cin >> numDice;
            cout << "Number of sides on dice? : ";
            cin >> numSides;
            dice = new Dice(numSides);
        }

        //Write all scores to a file for use in the high score
        void writeScorestoFile(){
            ofstream myfile ("score.txt", fstream::app);
            string line1;
            string line2;
            if (myfile.is_open()){
                for(int i = 0; i < numPlayers; i++){ //write to the file
                    line1 = players[i]->getName();
                    line2 = to_string(players[i]->getScore());
                    myfile << line1 << " : " << line2 << endl;
                }
            }
        }

        //Loop through text file for highest player score
        void getHighestScore(){
            string line;
            int score;
            int max = 0;
            string max_name;
            string a,b,c,name;
            ifstream myfile ("score.txt");
            if (myfile.is_open())
            {
                while ( getline (myfile,line) )//read file
                {
                    //trim(line);
                    stringstream ss(line);
                    ss >>name >> b >> score; //b holds the ":"

                    if(score > max) {
                        max = score;
                        max_name  = name;
                    }
                }
                myfile.close();
            }
            else cout << "Unable to open file";

            cout << "___High Score___" << endl;
            cout << max_name << " : " << max << endl;

        }

};
class Knockout : public DiceGame{
    void play(){

        do {
            int knockoutSum = getSum();
             for (int i = 0; i < numPlayers; i++) {
                int x = getSum();
                if (x == knockoutSum and countAlive() > 1) {
                    players[i]->setAlive(false);
                }
                if(players[i]->getAlive()){
                    players[i]->addToScore(1);
                }
            }
            //cout << countAlive();
        }while(countAlive() > 1);
        displayScores();
        printWinner();
    }
    int getSum(){ //get sum of all dice rolls
        int sum = 0;
        for(int i = 0; i < numDice; i ++){
            sum += dice->roll();
        }
        return sum;
    }
    int countAlive(){ //count the number of true in bool alive
        int sum =0;
        for(int i = 0; i < numPlayers; i++){
            if(players[i]->getAlive())
                sum += 1;
        }
        return sum;
    }
    void printWinner(){ //only works when alive count is 1
        if(countAlive() == 1){
            for(int i = 0; i < numPlayers; i++){
                if(players[i]->getAlive())
                    cout << players[i]->getName() <<" wins with a score of: " << players[i]->getScore() << endl;
            }
        }
    }
};

class BostonGame: public DiceGame{
    void play(){

        int max = 0; string max_name;
        for(int i = 0; i < numPlayers; i++){
            players[i]->setScore(getRollSum());
            if(players[i]->getScore() > max) {
                max = players[i]->getScore();
                max_name = players[i]->getName();
            }
        }
        displayScores();
        cout << max_name << " wins with a score of: " << max <<endl;
    }
    //this function loops through the dice and adds the sum while saving the highest of each roll
    //Ex. 3 dice would be rolled, highest saved, then 2 rolled, and so on
    int getRollSum(){
        vector <int> rolls(numDice);
        int sum = 0;
        for (int i = numDice; i > 0; i --){
            for(int k = 0; k < i; k++){
                rolls[k] = dice->roll();
                //cout << rolls[k] << endl;
            }
            //cout << maxVal(rolls);
            sum += maxVal(rolls,i);

        }
        return sum;
    }
    //simulates a max function for determining highest dice
    int maxVal(vector<int> arr, int count){
        int max = 0;
        for(int i = 0; i < count; i++){
            if(arr[i] > max)
                max = arr[i];
        }
        return max;
    }
};

int main() {
    // Base class pointer
    int choice;
    char ans;
    DiceGame *game;
    //Play Again Loop
    do {
        //Input Loop
        do {
            cout << "Which game would you like to play" << endl
                 << "1 - Knockout" << endl
                 << "2 - Boston Game" << endl;
            cout << "Enter Choice: ";
            cin >> choice;
        } while (choice != 1 && choice != 2);

        //Game Initialization
        if (choice == 1) {
            game = new Knockout();
            cout << "____Knockout_____" << endl;
        } else {
            game = new BostonGame();
            cout << "____Boston Dice_____" << endl;

        }
        game->initPlayers();
        game->requestDice();
        game->play();
        game->writeScorestoFile();
        game->getHighestScore();

        cout << "Enter Y to play again: ";
        cin >> ans;
    }while( ans == 'y' || ans == 'Y');
}