// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include<fstream>
#include<time.h>
#include<vector>

using namespace std;

string fileName = "";//global variable

class Solve {
private:
	string answer;
	string hint;
	char *answerChar;
	int wrongGuesses;
	bool solved;

public:
	Solve() {
		answerChar = new char[5];
		answer = "";
		hint = "";
		wrongGuesses = 0;
		solved = false;
	}

	Solve(string word, string hint) {//2 argument constructor for multiplayer mode that sets the answer and hint for the game to start
		answerChar = new char[word.length()];
		answer = word;
		this -> hint = hint;
		wrongGuesses = 0;
		solved = false;

		//cout << "answer: " << answer << ", and hint: " << hint << endl;//use for debugging

		for (int i = 0; i < answer.length(); i++) {
			if (answer[i] != ' ') {
				answerChar[i] = '_';
			}
			else if (answer[i] == ' ') {
				answerChar[i] = ' ';
			}

		}
	}

	Solve(string word) {//1 argument constructor for single player mode, seperates the answer from the hint
		wrongGuesses = 0;
		solved = false;
		int position = word.find('-');
		answer = word.substr(0, position);
		hint = word.substr(++position, word.length());
		answerChar = new char[answer.length()];

		//cout << "answer: " << answer << ", and hint: " << hint << endl;//use for debugging

		for (int i = 0; i < answer.length(); i++) {
			if (answer[i] != ' ') {
				answerChar[i] = '_';
			}
			else if (answer[i] == ' ') {
				answerChar[i] = ' ';
			}

		}
	}

	void play() {//one of the most important methods I have, It is what starts and plays the game for any mode single or multiplayer
		cout << "\nYou will only have 6 wrong guesses...\nIf you get 3 wrong guesses a *Hint will be provided" << endl;

		vector<char> used;//keeps track of used chars
		char guess;
		bool found;
		while (wrongGuesses < 6 && !solved) {
			found = false;
			display();
			
			if (wrongGuesses >= 3) {
				cout << "Your *Hint is: " << hint << endl;
			}

			cout << "Enter your character, you have " << wrongGuesses << " wrong guesses so far." << endl;
			cin >> guess;
			if (find(used.begin(), used.end(), guess) != used.end()) {
				cout << "You already guessed this, try again..." << endl;
				continue;
			}

			for (int i = 0; i < answer.length(); i++) {
				if (answer[i] == toupper(guess) || answer[i] == tolower(guess)) {
					found = true;
					answerChar[i] = answer[i];
					
				}
			}
			used.push_back(toupper(guess));//adds uppercase char of the guess
			used.push_back(tolower(guess));//adds lowercase char of the guess
			if (!found) {
				cout << "Wrong guess..." << endl;
				wrongGuesses++;
				//add switch-case method for how to draw person
			}
			else {
				cout << "Good guess!" << endl;
			}

			drawing();//draws the hangman after every right or wrong guess

			if (checkIfCorrect()) {//once solved the game, stops the while loop that keeps the game going
				solved = true;//had == but that was wrong
				break;
			}
		}

		display();//displays the full answer at the end
		if (checkIfCorrect()) {
			cout << "Congrats, you won!";
			exit(0);
		}
		else {
			cout << "You loose...\nThe answer was " << answer << endl;
		}
		
	}

	bool checkIfCorrect() {
		string temp = "";
		for (int i = 0; i < answer.length(); i++) {
			temp += answerChar[i];
		}
		return temp == answer;
	}

	void display() {
		for (int i = 0; i < answer.length(); i++) {
			cout << answerChar[i] << " ";
		}
		cout << endl;
	}

	void drawing() {//switch-case that draws the hangman
		switch (wrongGuesses) {
			case 0: cout << "" << endl;
				break;
			case 1: cout << "\n O \n" << endl;
				break;
			case 2: cout << "\n O\n | \n" << endl;
				break;
			case 3: cout << "\n O\n""\\""| \n" << endl;
				break;
			case 4: cout << "\n O\n""\\""|/ \n" << endl;
				break;
			case 5: cout << "\n O\n""\\""|/\n/ \n" << endl;
				break;
			case 6: cout << "\n O\n""\\""|/\n/ \\ \n" << endl;
				break;
		default:
			break;
		}
	}
	

};

class Word {      
private:
	string wordFromList;

	int numberOfLines() {//private method that gets the number of lines in the file
		string line;
		int linesNumber = 0;
		ifstream myfile(fileName);
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, line);
				linesNumber++;
			}
			myfile.close();
		}
		return linesNumber;
	}

public:
	Word() { //default contrsctor
		setWord();
		playGame();
	}

	string setWord() {
		srand(time(0));
		int randomNumber = rand() % numberOfLines();//gives random line from start of file to the last line possible

		ifstream hangName;
		hangName.open(fileName);
		if (hangName.is_open()) {
			for (int i = 0; i < randomNumber; i++) {//if the file is found keep going down the lines until the right line number is found
				//hangName.ignore(i);
				getline(hangName, wordFromList);
			}
		}
		else {
			cout << "file is not open";
		}
		hangName.close();//closes file
		//cout << wordFromList;
		return wordFromList;//return the word + hint in one line, usually used this return to debug and make sure it works
	}

	void playGame() {
		while (wordFromList == "") {//in-case error with file reading and takes a blank line, then finds a new one...
			setWord();
		}
		Solve match(wordFromList);
		match.play();
	}

};

class Multiplayer {
private:
	string word;
	string hint;
public:
	Multiplayer(){//defualt constructor
		word = "";
		hint = "";
	}
	Multiplayer(string word, string hint) {//2 argument contrsuctor that sets the word and hint
		setWord(word);
		setHint(hint);
		playGame();
	}
	void setWord(string myWord) {
		word = myWord;
	}
	void setHint(string myHint) {
		hint = myHint;
	}

	void playGame() {//method to start the game
		Solve match(word, hint);
		match.play();
	}
};

void pickGenre() {
	cout << "Do you want to play a Sports Category or Movies?\nEnter 's' for sports, or 'm' for movies...";
	char genre;
	cin >> genre;
	while (tolower(genre) != 'm' && tolower(genre) != 's') {
		cout << "Wrong choice, please select 's' for sports or 'm' for movies...";
		cin >> genre;
	}
	if (genre == 'm') {
		fileName = "movies.txt";
	}
	else {
		fileName = "ListOfWords.txt";
	}
}

char pickMode() {
	cout << "Do you want to play simgle player, or aganst a friend?\nEnter s for single player, or m for multiplayer...\n";
	char mode;
	cin >> mode;
	while (tolower(mode) != 'm' && tolower(mode) != 's') {
		cout << "Wrong choice, please select 's' for single player or 'm' for multiplayer...";
		cin >> mode;
	}
	return mode;
}

int main() {
	cout << "Hello, Welcome to Hangman!\n";	
	char recivedMode = pickMode();
	if (recivedMode == 'm') {
		string word;
		string hint;
		string theme;
		cout << "Player 2 enter the Theme...";
		cin.ignore();//used to flush out the cin>> line so all of getline can be used...
		getline(cin, theme);
		cout << "Player 2, enter a word for player 1 to solve...\n";
		getline(cin, word);
		cout << "Player 2, now enter the hint...\n";
		getline(cin, hint);
		system("cls");//clears screen so player 1 can play without seeing the word entered by the 2nd player...
		cout << "Hello player 1, Your theme is " << theme;
		Multiplayer myGame(word, hint);
	}
	else {
		pickGenre();
		Word game;
	}


	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

