#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "newLoadGame.h"
#include <sys/wait.h>
using namespace std;

class Move {
public:
  Move(string name, int power) : name(name), power(power) {}
  string getName() const { return name; }
  int getPower() const { return power; }
private:
  string name;
  int power; 
};

class Pokemon {
public:
  Pokemon(int health) : max_health(health), cur_health(health) {}
  int getHealth() const { return cur_health; }
  virtual int attack() = 0;
  virtual int speed() = 0;
  virtual string getName() = 0;
  void takeDamage(int damage){ cur_health -= damage; }
  void setHealth(int saveDataHealth){ cur_health = saveDataHealth; }

  // Moves section
  void addMove(Move move) { moves.push_back(move); }
  vector<Move> getMoves() const { return moves; }
private:
  int max_health;
  int cur_health;
  vector<Move> moves;
};


class Pikachu : public Pokemon {
public:
  Pikachu() : Pokemon(60) {
    addMove(Move("Tackle", 10));
    addMove(Move("Thunderbolt", 20));
  }
  int attack() override { return 10; }
  int speed() override { return 60; }
  string getName() override { return "Pikachu"; }
};

class Charmander : public Pokemon {
public:
  Charmander() : Pokemon(50) {
    addMove(Move("Tackle", 10));
    addMove(Move("Fire Spin", 20));
  }
  int attack() override { return 15; }
  int speed() override { return 55; }
  string getName() override { return "Charmander"; }
};

class Squirtle : public Pokemon {
public:
  Squirtle() : Pokemon(45) {
    addMove(Move("Tackle", 10));
    addMove(Move("Water Gun", 20));
  }
  int attack() override { return 10; }
  int speed() override { return 65; }
  string getName() override { return "Squirtle"; }
};

class Bulbasaur : public Pokemon {
public:
  Bulbasaur() : Pokemon(60) {
    addMove(Move("Tackle", 10));
    addMove(Move("Leaf Blade", 20));
  }
  int attack() override { return 10; }
  int speed() override { return 50; }
  string getName() override { return "Bulbasaur"; }
};

class Trainer {
public:
  Trainer(string name) : name(name) {}
  void addPokemon(Pokemon* pokemon) { pokemons.push_back(pokemon); }
  string getName() const { return name; }
  Pokemon* getPokemon(int i) const { return pokemons[i]; }
private:
  string name;
  vector<Pokemon*> pokemons;
};

void saveGame(Trainer* trainer1, Trainer* trainer2) {
  // Write the current state of the game to a file
  ofstream saveData("save.txt");
  // Trainer 1 stuff
  saveData << trainer1->getName() << endl; // trainer1 name
  saveData << trainer1->getPokemon(0)->getName() << endl; // trainer 1 pokemon name
  saveData << trainer1->getPokemon(0)->getHealth() << endl; // trainer 1 pokemon's current health
  
  // Trainer 2 stuff (we already know its ash with pikachu)
  saveData << trainer2->getPokemon(0)->getHealth(); // trainer 2 pokemon's current health
}

class Battle {
public:
  Battle(Trainer* trainer1, Trainer* trainer2) : trainer1(trainer1), trainer2(trainer2) {}
  void start() {  
    // Determine who goes first (Compare both Pokemons speeds)
    
    // User is faster
    if (trainer1->getPokemon(0)->speed() > trainer2->getPokemon(0)->speed()){
      cout << endl << "Your Pokemon is faster!" << endl << "You move first." << endl;
      sleep(2);
    }
      
    // Opponent is faster
    else{
      cout << endl << "The opponent's Pokemon is faster!" << endl << "They move first." << endl;
      sleep(2);
    }
    
    // Determine whose turn it is
    bool userTurn = trainer1->getPokemon(0)->speed() > trainer2->getPokemon(0)->speed();

    // Continue until one of the Pokemon faints
    while (trainer1->getPokemon(0)->getHealth() > 0 && trainer2->getPokemon(0)->getHealth() > 0) {

      // User's turn
      if (userTurn) {
        cout << endl << "It's " << trainer1->getName() << "'s turn!" << endl;
        cout << "What will " << trainer1->getPokemon(0)->getName() << " do? (1 or 2):" << endl;
        vector<Move> moves = trainer1->getPokemon(0)->getMoves();
        for (int i = 0; i < moves.size(); i++) {
          cout << i + 1 << ") " << moves[i].getName() << endl;
        }
        cout << "3) Save game and exit" << endl;
  
        int attack;
        cin >> attack;
        //if (!isdigit(attack))
          //attack = 0;
        int damage = 0;
        switch (attack) {
          case 1:
            damage = trainer1->getPokemon(0)->attack();
            cout << endl << trainer1->getName() << "'s " << trainer1->getPokemon(0)->getName() << " used "<< moves[0].getName() << "!" << endl;
            sleep(2);
            break;
          case 2:
            damage = trainer1->getPokemon(0)->attack() * 2;
            cout << endl << trainer1->getName() << "'s " << trainer1->getPokemon(0)->getName() << " used "<< moves[1].getName() << "!" << endl;
            sleep(2);
            break;
          case 3:
            cout << endl << "Understood! Saving game please wait." << endl;
            saveGame(trainer1,trainer2);
            sleep(5);
            cout << "Game Saved, See you later!" << endl;
            exit(0);
          default:
            cout << endl << "Invalid input. Please choose again." << endl;
            continue;
        }

        trainer2->getPokemon(0)->takeDamage(damage); // Reduce opponent's Pokemon's health
        cout << "It caused " << damage << " damage!" << endl;
        if (trainer2->getPokemon(0)->getHealth() < 0)
          trainer2->getPokemon(0)->setHealth(0);
        cout << trainer2->getName() << "'s " << trainer2->getPokemon(0)->getName() << " has " << trainer2->getPokemon(0)->getHealth() << " health left." << endl;
        sleep(2);
        userTurn = false;
      }

      // Opponent's turn
      else{
        cout << endl << "It's " << trainer2->getName() << "'s turn!" << endl;
        int attack = rand() % 2 + 1;
        int damage = 0;
        switch (attack) {
          case 1:
            damage = trainer2->getPokemon(0)->attack();
            cout << trainer2->getName() << "'s " << trainer2->getPokemon(0)->getName() << " used Quick Attack!" << endl << endl;
            sleep(2);
            break;
          case 2:
            damage = trainer2->getPokemon(0)->attack() * 2;
            cout << trainer2->getName() << "'s " << trainer2->getPokemon(0)->getName() << " used Thunderbolt!" << endl << endl;
            sleep(2);
            break;
          default:
            continue;
          
        }
        trainer1->getPokemon(0)->takeDamage(damage); // Reduce opponent's Pokemon's health
        cout << "It caused " << damage << " damage!" << endl;
        if (trainer1->getPokemon(0)->getHealth() < 0)
          trainer1->getPokemon(0)->setHealth(0);
        cout << trainer1->getName() << "'s " << trainer1->getPokemon(0)->getName() << " has " << trainer1->getPokemon(0)->getHealth() << " health left." << endl;
        sleep(2);
        userTurn = true;
      }
    }
    // You won the battle
    if (trainer2->getPokemon(0)->getHealth() == 0 ){
      cout << endl << "Ash's Pikachu has fainted." << endl;
      sleep(1);
      cout << "Ash: \"Your a pretty strong trainer! Keep battling and you'll be the best.\"" << endl << endl;
      sleep(3);
      cout << trainer1->getPokemon(0)->getName() << " has gained 19 Exp. Points!"; // exp not implemented yet
      
      // add clear txt feature here
      std::ofstream clearFile("save.txt", std::ios::trunc);
      clearFile.close();
    }
    // You lost the battle
    else{
      cout << endl << "Your " << trainer1->getPokemon(0)->getName() << " has fainted." << endl;
      sleep(1);
      cout << "You have no more usable Pokemon." << endl;
      sleep(2);
      cout << "You blacked out.";
      
      // add clear txt feature here
      std::ofstream clearFile("save.txt", std::ios::trunc);
      clearFile.close();
      
    }
  }
private:
  Trainer* trainer1;
  Trainer* trainer2;
};

void loadGame() {
  // Load the saved state of the game from a file
  ifstream readSave ("save.txt");
  
  // Trainer 1 stuff
  string data;
  readSave >> data;
  Trainer trainer1 (data); // trainer1 name
  readSave >> data;
  if (data == "Charmander"){
    Pokemon* charmander = new Charmander();
    trainer1.addPokemon(charmander); //trainer1 had charmander
  }
  else if(data == "Squirtle"){
    Pokemon* squirtle = new Squirtle();
    trainer1.addPokemon(squirtle); // trainer1 had squirtle
  }
  else if(data == "Bulbasaur"){
    Pokemon* bulbasaur = new Bulbasaur();
    trainer1.addPokemon(bulbasaur); // trainer1 had bulbasaur
  }
  readSave >> data;
  trainer1.getPokemon(0)->setHealth(stoi(data)); // trainer1 health set

  // Trainer 2 stuff
  readSave >> data;
  Trainer trainer2 ("Ash"); // trainer2 name
  Pokemon* pikachu = new Pikachu(); // trainer2 has pikachu
  trainer2.addPokemon(pikachu);
  trainer2.getPokemon(0)->setHealth(stoi(data)); // trainer2 health set

  cout << "Hey " << trainer1.getName() << "! The last time you played you selected " << trainer1.getPokemon(0)->getName() << " as your pokemon." << endl;
  sleep(3);
  cout << trainer1.getPokemon(0)->getName() << " currently has " << trainer1.getPokemon(0)->getHealth() << " hp." << endl;
  sleep(2);
  cout << "You were facing Ash's Pikachu and it currently has " << trainer2.getPokemon(0)->getHealth() << " hp." << endl;
  sleep(2);
  cout << "Get ready for battle trainer!" << endl;
  sleep(2);
  cout << "---------------------------------------------------------------" << endl;
    
  //start battle
  Battle battle (&trainer1, &trainer2);
  battle.start();
}

int main() { // make sure you account for lowercase inputs
  
  //iterator for the do while loop
  int i = 0;
  
  do{
    cout << "New Game: (N)" << endl << "Load Game: (L)" << endl;
    string c;
    cin >> c;
    
    // New game (call function)
    if (c == "N"){
      i++;
      cout << endl << "You selected new game." << endl << "Hi, trainer. What is your name?" << endl;
      
      string name; cin >> name;
      Trainer trainerUser(name);
      cout << endl << "Hello " << name << "." << endl << "Which Pokemon do you want to use for battle!" << endl;
    
      int n = 0;
      do{
        cout <<  endl << "Charmander (HP: 50, Attack: 15, Speed: 55)" << endl
        << "Squirtle (HP:45, Attack: 10, Speed: 65)" << endl
        << "Bulbasaur (HP:60, Attack: 10, Speed: 50)" << endl;

        cout << "Choosing: ";
        string poke; cin >> poke;
        for(int i=0; i<poke.size(); i++){
          poke[i] = tolower(poke[i]);
        }
        
        if (poke == "charmander"){
          n++;
          Pokemon* charmander = new Charmander();
          trainerUser.addPokemon(charmander);
        }
        else if(poke == "squirtle"){
          n++;
          Pokemon* squirtle = new Squirtle();
          trainerUser.addPokemon(squirtle);
        }
        else if(poke == "bulbasaur"){
          n++;
          Pokemon* bulbasaur = new Bulbasaur();
          trainerUser.addPokemon(bulbasaur);
        }
        else{
          cout << endl << "Seems you choose an unknown pokemon, try again." << endl;
        }
      }
      while(n==0);

      Trainer trainerAsh("Ash");
      Pokemon* pikachu = new Pikachu;
      trainerAsh.addPokemon(pikachu);

      cout << endl << "You will be facing Ash Ketchum with his famous pokemon Pikachu!" << endl;
      cout << trainerUser.getName() << " & " << trainerUser.getPokemon(0)->getName() << " vs. " 
      << trainerAsh.getName() << " & " << trainerAsh.getPokemon(0)->getName() << endl;
      cout << "---------------------------------------------------------------" << endl;
      sleep(2);
      
      //Initate the battle
      Battle battle (&trainerUser, &trainerAsh);
      battle.start();
    }
      
    // Load Game (call function)
    else if (c == "L"){
      i++;
      cout << endl << "You selected load game." << endl << endl;
      sleep(2);
      loadGame();
      
    }
    else{
      cout << endl << "Please select a valid option." << endl << endl;
    }
    
  }
  while(i==0);

  return 0;
}