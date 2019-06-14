// DSA Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there. //

/*
	Team Member Name: Chee Jing Jie
	Group Name: Team Solo
	Student ID: S10179578J
*/

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>

/* Using specific classes and objects in the standard namespace */
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;

/* Constants */
const int MAX_SIZE = 32;

/* Data Structure */
struct Trie {
	Trie* children[MAX_SIZE];
	bool eow;
};

/* Global Variables */
fstream globalFileHandle;
bool isglobalFileOpen = false;
Trie* rootTrieNode = NULL;

/*Function Declarations*/
void printError(string, string);

/* Creates a Trie Node using memory from the heap, initializes the defaults values and returns a pointer to the newly created TrieNode */
Trie* getNewTrieNode() {
	Trie* newTrieNode = new Trie;
	for (int i = 0; i < MAX_SIZE; i++)
		newTrieNode->children[i] = NULL;
	newTrieNode->eow = false;
	return newTrieNode;
}

/* Returns the offset from a list of pre-defined special characters, -1 otherwise. This offset is added to the value 25 and then references a Trie Node's array of children */
int getOffset(char c) {
	string specialChar = "-.'()/";
	size_t offset = specialChar.find(c);
	if (offset != string::npos) {
		return offset;
	}
	else
		return -1;
}

/* This is the reverse of the 'getOffset' function. It retrieves a special character given an offset */
char getChar(int i) {
	string specialChar = "-.'()/";
	i -= 26;
	return specialChar[i];
}

/* Inserts the specified string into the Trie data structure */
bool insert(string word) {

	Trie* currentNode = rootTrieNode;
	int wordLength = word.length();
	int offset = 0;
	char arrayPos;

	// converts the all letters to lowercase
	for (int i = 0; i < wordLength; i++) {
		word[i] = tolower(word[i]);
	}
	// loops through each character and inserts it into the Trie data structure. If a charcter is invalid, insertion fails and function return false.
	for (int i = 0; i < wordLength; i++) {
		if (!isalpha(word[i])) { // check if special character
			offset = getOffset(word[i]);
			if (offset == -1) {
				printError("Insertion Error", "Word contains invalid characters.");
				return false;
			}
			arrayPos = 26 + offset; // set offset
		}
		else {
			arrayPos = word[i] - 'a'; // set offset
		}
		if (currentNode->children[arrayPos] == NULL) { // check if children node is null
			Trie* newNode = getNewTrieNode();
			currentNode->children[arrayPos] = newNode;
		}
		currentNode = currentNode->children[arrayPos];
	}
	currentNode->eow = true;
	return true;
}

/* Searches for a word given a TrieNode and a string. Search is made from the given node */
bool search(Trie* currentNode, string word) {
	int offset = 0;

	for (char c : word) {
		if (!isalpha(c)) {
			offset = getOffset(c);
			if (offset == -1) {
				printError("Search Error", "Invalid character parsed.");
				return false;
			}
			currentNode = currentNode->children[26 + offset];
		}
		else {
			currentNode = currentNode->children[c - 'a'];
		}
		if (currentNode == NULL) {
			return false;
		}
	}
	if (currentNode->eow)
		return true;
	return false;
}

/* Searches for a TrieNode in the Trie Data Structure given a string, returns the TrieNode if it exists, NULL otherwise */
Trie* getNodeIfExists(Trie* currentNode, string word) {
	int offset = 0;
	for (char c : word) {
		if (!isalpha(c)) {
			offset = getOffset(c);
			if (offset == -1) {
				return NULL;
			}
			currentNode = currentNode->children[26 + offset];
		}
		else {
			currentNode = currentNode->children[c - 'a'];
		}
		if (currentNode == NULL) {
			return NULL;
			// cout << "'" << word << "'" << " was not found in the dictionary." << endl;
		}
	}
	return (currentNode == rootTrieNode) ? NULL : currentNode;
}

/* Prints all the words below a given TrieNode */
void printBelowNode(Trie* currentNode, char s[], int height) {

	// s represents an array of characters, it is used to store and print words
	// height is used to write the current character to the appropriate position in the character array

	if (currentNode->eow) { // base case
		s[height] = '\0'; // this is required for the program to determine where the end of the string is
		if (isglobalFileOpen) {
			globalFileHandle << s << endl;
		}
		else
			cout << s << endl;
	}

	// loops through each children element of currentNode
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (currentNode->children[i] != NULL)
		{
			if (i >= 26) {
				s[height] = (i); // writing the current character to the appropriate position using the height variable
			}
			else {
				s[height] = (i + 'a');
			}
			printBelowNode(currentNode->children[i], s, height + 1);
		}
	}
}

/* A generic error printing function */
void printError(string errType, string message) {
	cout << errType << ": " << message << endl;
}

/* Prompts the user for a dictionary file to load the list of words into the Trie data structure */
bool initializeDictionary() {
	const std::streamsize MAX_SIZE = 1000;
	char line[MAX_SIZE];
	fstream file;
	string userInput;

	cout << "Enter dictionary text file: ";
	getline(cin, userInput);

	file.open(userInput, fstream::in);
	if (!file.is_open()) {
		cout << "Failed to open '" << userInput << "'." << endl;
	}
	else {
		while (file.getline(line, MAX_SIZE)) {
			insert(line);
		}
		file.close();
		cout << "Dictionary has been stored in RAM." << endl;
		return true;
	}
	return false;

}

/* Checks for a character insertion error given a word */
string checkForInsertionError(string word) {
	int wordLength = word.length();
	string tempWord;
	// loops through the entire word, deletes the current character and use the search function to search for the modified word
	for (int i = 0; i < word.length(); i++) {
		tempWord = word.substr(0, i) + word.substr(i + 1, wordLength - i);
		if (search(rootTrieNode, tempWord)) {
			return tempWord;
		}
	}
	return "";
}


/* Checks for a character deletion error given a word */
void checkForDeletionError(string word) {
	int wordLength = word.length();
	Trie* currentNode = rootTrieNode;
	int offset;

	// first to 2nd last
	for (int i = 0; i < wordLength; i++) {
		if (i == 0) {
			currentNode = rootTrieNode;
		}
		else {
			currentNode = getNodeIfExists(rootTrieNode, word.substr(0, i));
		}
		if (currentNode == NULL)
			return;
		for (int x = 0; x < MAX_SIZE; x++) {
			if (currentNode->children[x] != NULL) {
				if (search(currentNode->children[x], word.substr(i, wordLength - i))) {
					if (x >= 26)
						cout << "Error Type: Deletion, did you mean '" << word.substr(0, i) + getChar(x) + word.substr(i, wordLength - i) << "'?" << endl;
					else
						cout << "Error Type: Deletion, did you mean '" << word.substr(0, i) + (char)(x + 'a') + word.substr(i, wordLength - i) << "'?" << endl;
				}
			}
		}
	}

	// if last letter deleted
	currentNode = getNodeIfExists(rootTrieNode, word);
	if (currentNode == NULL)
		return;
	for (int i = 0; i < MAX_SIZE; i++) {
		if (currentNode->children[i] != NULL) {
			if (currentNode->children[i]->eow) {
				if (i >= 26)
					cout << "Error Type: Deletion, did you mean '" << word + getChar(i) << "'?" << endl;
				else
					cout << "Error Type: Deletion, did you mean '" << word + (char)('a' + i) << "'?" << endl;
			}
		}
	}
}

/* Checks for a character substitution error given a word */
void checkForSubstitutionError(string word) {
	char tempChar;
	string tempString = word;
	for (int i = 0; i < word.length(); i++)
	{
		for (int x = 0; x < MAX_SIZE; x++) {
			if (x >= 26) {
				tempChar = getChar(x);
			}
			else { // normal characters
				tempChar = (char)('a' + x);
			}
			if (tempChar == word[i])
				continue;
			tempString[i] = tempChar;
			if (search(rootTrieNode, tempString)) {
				cout << "Error Type: Substitution, did you mean '" << tempString << "'?" << endl;
			}
		}
		tempString = word;
	}
}

/* Checks for a character transposition error given a word */
string checkForTranspositionError(string word) {
	int wordLength = word.length();
	string tempWord = word;
	char tempChar;
	for (int i = 0; i < wordLength - 1; i++) {
		tempChar = word[i];
		tempWord[i] = word[i + 1];
		tempWord[i + 1] = tempChar;
		if (search(rootTrieNode, tempWord)) {
			return tempWord;
		}
		tempWord = word;
	}
	return "";
}

/* Checks if a word exists in the dictionary */
void checkWord() {
	string wordToCheck;
	string errorResult;
	bool searchResult;

	cout << "\nEnter a word to be checked: ";
	getline(cin, wordToCheck);
	searchResult = search(rootTrieNode, wordToCheck);
	if (searchResult)
		cout << "\n'" << wordToCheck << "'" << " was found in the dictionary.\n" << endl;
	else
	{
		cout << "\n'" << wordToCheck << "'" << " was not found in the dictionary.\n" << endl;
		errorResult = checkForInsertionError(wordToCheck);
		if (errorResult != "") {
			cout << "Error Type: Insertion, did you mean " << "'" << errorResult << "'?" << endl;
		}
		errorResult = checkForTranspositionError(wordToCheck);
		if (errorResult != "") {
			cout << "Error Type: Transposition, did you mean " << "'" << errorResult << "'?" << endl;
		}
		checkForDeletionError(wordToCheck);
		checkForSubstitutionError(wordToCheck);
	}
}

/* Add a new word to the dictionary */
void addNewWord() {
	string newWord;
	cout << "Enter a word to enter into the dictionary: ";
	getline(cin, newWord);
	bool insertResult = insert(newWord);
	if (insertResult)
		cout << "Added " << "'" << newWord << "'" << " succesfully." << endl;
	else
		cout << "Failed to add " << "'" << newWord << "'" << endl;
}

/* Outputs all the word that starts with a user-specified string */
void printWordsThatStartWith() {
	char s[100];
	string startsWith;
	cout << "Enter the starting letter(s): ";
	getline(cin, startsWith);
	Trie* tempNode = getNodeIfExists(rootTrieNode, startsWith);
	if (tempNode != NULL) {
		for (int i = 0; i < startsWith.length(); i++) {
			s[i] = startsWith[i];
		}
		printBelowNode(tempNode, s, startsWith.length());
	}
	else {
		cout << "There are no words that start with: " << "'" << startsWith << "'" << endl;
	}
}

/* Prompts the user for a file name and saves all the words in dictionary in the file */
void saveDictionary() {
	string dictName;
	char s[1000];
	cout << "Enter dictionary's file name: ";
	getline(cin, dictName);
	globalFileHandle.open(dictName, std::fstream::out | std::fstream::trunc);
	if (globalFileHandle.is_open()) {
		isglobalFileOpen = true;
		printBelowNode(rootTrieNode, s, 0);
		globalFileHandle.close();
		isglobalFileOpen = false;
		cout << "Successfully saved dictionary to " << "'" << dictName << "'." << endl;
	}
	else {
		cout << "Failed to open file " << "'" << dictName << "'" << endl;
	}
}

/* Prompts user for a file name, loads all the words in the file and cross-check it against the list of words in the dictionary currently loaded in RAM.*/
void checkAgainstFile() {
	string fileName;
	const std::streamsize MAX_SIZE = 1000;
	char line[MAX_SIZE];
	fstream file;
	cout << "Specify a file to check: ";
	getline(cin, fileName);
	file.open(fileName);
	if (file.is_open()) {
		cout << "\nList of words that were NOT found: " << endl;
		while (file.getline(line, MAX_SIZE)) {
			if (!search(rootTrieNode, line)) {
				cout << line << endl;
			}
		}
		cout << "=================== End of Words Not Found in Dictionary ===================\n";
		file.close();
	}
	else {
		cout << "Failed to open file" << "'" << fileName << "'" << endl;
	}

}

/* Prints the menu in a while loop, retrieve user input (option), validates input, calls the appropriate function */
int main()
{
	bool isDictLoaded = false;
	string menuItems[6] = { "Check if word exists", "Load a dictonary", "Add a new word to dictionary", "Save the dictionary", "Display words that start with...", "Specify a file to check" };
	rootTrieNode = getNewTrieNode();

	string userInput;
	string validation = "012345";

	while (true) {
		cout << "\n";
		for (int i = 0; i < 6; i++) {
			cout << "========================\n";
			cout << "[" << i << "]" << " " << menuItems[i];
			cout << "\n========================";
		}
		cout << "\n\nEnter an option: ";
		getline(cin, userInput);

		if (userInput.length() > 1 || !isdigit(userInput[0]) || validation.find(userInput[0]) == string::npos) {
			cout << "Invalid argument!" << endl;
			continue;
		}

		if (!isDictLoaded && userInput[0] != '1') {
			cout << "\nNo dictionary loaded.";
			continue;
		}

		switch (userInput[0]) {
		case '0':
			checkWord();
			break;
		case '1':
			if (initializeDictionary())
				isDictLoaded = true;
			break;
		case '2':
			addNewWord();
			break;
		case '3':
			saveDictionary();
			break;
		case '4':
			printWordsThatStartWith();
			break;
		case '5':
			checkAgainstFile();
			break;
		default:
			cout << "Invalid option!" << endl;
		}
	}

}
