#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>
#include <stdlib.h>

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
        int roll() {
            srand((unsigned)time(0));
            return (random() % numSides) + 1;
        };
        void setNumSides(int n){
            numSides = n;
        }
};

/*
	Class representing a Game Player
*/
class Player{
    private:
        string name;
        int score;
        bool alive;
    public:
        Player(){
            name = "";
            score = 0;
            alive = true;
        }
        Player(string n, int s){
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
        bool getAlive(){
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

        void displayScores(){
            for(int i = 0; i < numPlayers; i++){
                cout << players[i]->getName() << " : " << players[i]->getScore() << endl;
            }
        }
        void requestDice(){
            int numSides;
            cout << "How many dice? : ";
            cin >> numDice;
            cout << "Number of sides on dice? : ";
            cin >> numSides;
            dice = new Dice(numSides);
        }

};
class Knockout : public DiceGame{
    void play(){
        initPlayers();
        requestDice();
        //displayScores();
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
    int getSum(){
        int sum = 0;
        for(int i = 0; i < numDice; i ++){
            sum += dice->roll();
        }
        return sum;
    }
    int countAlive(){
        int sum =0;
        for(int i = 0; i < numPlayers; i++){
            if(players[i]->getAlive())
                sum += 1;
        }
        return sum;
    }
    void printWinner(){
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
        initPlayers();
        dice = new Dice(6);
        int max = 0; string max_name;
        for(int i = 0; i < numPlayers; i++){
            players[i]->setScore(getRollSum());
            if(players[i]->getScore() > max) {
                max = players[i]->getScore();
                max_name = players[i]->getName();
            }
        }

        cout << max_name << " wins with a score of: " << max <<endl;
    }
    int getRollSum(){
        vector <int> rolls(3);
        int sum = 0;
        int set = 3;
        for (int i = 3; i > 0; i --){
            for(int k = 0; k < i; k++){
                rolls[k] = dice->roll();
                cout << rolls[k] << endl;
            }
            //cout << maxVal(rolls);
            sum += maxVal(rolls,i);
            cout << "Max: " << maxVal(rolls,i) << endl;
            cout << "Sum: " << sum << endl;
        }
        return sum;
    }
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
    DiceGame *game = new BostonGame();
    game->play();
}