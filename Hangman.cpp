/*
 * Author: Nabi Ozberkman
 * Filename: Hangman.cpp
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

bool orderFunction (pair<string,float>& i, pair<string,float>& j) {
	return i.second < j.second;
}
//Main method that contains all the functionality
int main(int argc, char* argv[]) {	
	char L1, L2, L3, L4, L5 = '\0';
	int foundI[] = {0, 0, 0, 0, 0};
	bool found5 = false;
	bool found4 = false;
	bool found3 = false;
	bool found2 = false;
	bool found1 = false;
	if (argc != 7) {
		cout << "Usage: ./a.out L _ O _ K DJGH" << endl;
		cout << "Use '_' for undiscovered letters and list all the incorrect letters appended as a single word as the last argument. If no incorrect letters have been guessed, enter '\0' as the last argument." << endl;
		exit(1);
	}

	//Importing command line arguments into respective variables
	string foundLetters = argv[6];
	if (*argv[1] != '_') {
		L1 = *argv[1];
		found1 = true;
		foundI[0] = 1;
		foundLetters += L1;
	}
	if (*argv[2] != '_') {
		L2 = *argv[2];
		found2 = true;
		foundI[1] = 1;
		foundLetters += L2;
	}
	if (*argv[3] != '_') {
		L3 = *argv[3];
		found3 = true;
		foundI[2] = 1;
		foundLetters += L3;
	}
	if (*argv[4] != '_') {
		L4 = *argv[4];
		found4 = true;
		foundI[3] = 1;
		foundLetters += L4;
	}
	if (*argv[5] != '_') {
		L5 = *argv[5];
		found5 = true;
		foundI[4] = 1;
		foundLetters += L5;
	}
	//Scan the file for words and counts
	ifstream inFile;
	string line;
	int x;
	vector<pair<string, int> > arr;
	stringstream myWord;
	inFile.open("hw2_word_counts_05.txt");
	while(true) {
		inFile >> line;
		inFile >> x;
		if(inFile.eof())
			break;
		arr.push_back(make_pair(line, x));
	}
	float k;
	vector<pair<string, float> > arr2;
	for (auto it2 = arr.begin(); it2 != arr.end(); it2++) {
		k += it2->second;
	}

	//Calculating probability of each word
	for (auto it = arr.begin(); it != arr.end(); it++) {
		arr2.push_back(make_pair(it->first, (((float)(it->second)))/k));
	}

	sort(arr2.begin(), arr2.end(), orderFunction);
	auto iter = arr2.begin();
	iter += 13;
	//Prints least probable words
	for(int x = 0; x < 14; iter--, x++);
	//Prints most probable words
	for(int x = 0; x < 15; x++);

	vector<pair<string, float> > arr3 = arr2;
	vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

	unordered_map<char, float> letterMap;
	
	//Establishing unused letters and each one's probability
	for (auto itL = letters.begin(); itL != letters.end(); itL++) {
		letterMap[*itL] = 0.0;
	}


	for (int k = 0; k < foundLetters.size(); k++) {
		letterMap.erase(foundLetters.at(k));
	}
	
	//Calculate probability of evidence given the word
	float p1;
	for(auto it = arr3.begin(); it != arr3.end(); it++) {
		if(found1) {
			if(it->first.at(0) != L1) {
				it->second = 0;
				goto nextElement;
			}
		}
		else if (letterMap.find(it->first.at(0)) == letterMap.end()) {
			it->second = 0;
			goto nextElement;
		}	
		if(found2) {
			if(it->first.at(1) != L2) {
				it->second = 0;
				goto nextElement;
			}
		}
		else if (letterMap.find(it->first.at(1)) == letterMap.end()) {
			it->second = 0;
			goto nextElement;
		}	
		if(found3) {
			if(it->first.at(2) != L3) {
				it->second = 0;
				goto nextElement;
			}
		}
		else if (letterMap.find(it->first.at(2)) == letterMap.end()) {
			it->second = 0;
			goto nextElement;
		}	
		if(found4) {
			if(it->first.at(3) != L4) {
				it->second = 0;
				goto nextElement;
			}
		}
		else if (letterMap.find(it->first.at(3)) == letterMap.end()) {
			it->second = 0;
			goto nextElement;
		}	
		if(found5) {
			if(it->first.at(4) != L5) {
				it->second = 0;
				goto nextElement;
			}
		}
		else if (letterMap.find(it->first.at(4)) == letterMap.end()) {
			it->second = 0;
			goto nextElement;
		}
		it->second = 1;
nextElement:;
	}
	
	//Calculating the probability of evidence
	vector<pair<string, float> > arr4 = arr2;
	auto iter1 = arr4.begin();
	auto iter2 = arr2.begin();
	auto iter3 = arr3.begin();
	for (; iter1 != arr4.end(); iter1++, iter2++, iter3++) {
		iter1->second = iter2->second * iter3->second;	
	}

	//Still calculating probability of evidence
	vector<pair<string, float> > arr5 = arr2;
	float probE;
	auto iter4 = arr4.begin();
	for (; iter4 != arr4.end(); iter4++) {
		probE += iter4->second;
	}

	//Calculating the probability of each word given the evidence
	auto iter5 = arr5.begin();
	auto iter6 = arr4.begin();
	float Guess = 0;
	string wordGuess;
	float totalProbWord;
	for (; iter5 != arr5.end(); iter5++, iter6++) {
		iter5->second = iter6->second / probE;
		totalProbWord += iter5->second;
		if (iter5->second > Guess) {
			Guess = iter5->second;
			wordGuess = iter5->first;
		}
	}

	if (Guess != 0 && Guess/totalProbWord > 0.80) {
		cout << "Guessed word: " << wordGuess << " with probability: " << Guess/totalProbWord << endl;
	}

	vector<pair<char,float> > decision;
	float totalProb;

	//Calculating the probability of each letter given the evidence
	for (auto iter8 = letterMap.begin(); iter8 != letterMap.end(); iter8++){
		totalProb = 0;
		for (auto iter7 = arr5.begin(); iter7 != arr5.end(); iter7++) {
			for (int j=0; j<5; j++) {
				if (foundI[j] == 0) {
					if ((*iter8).first == iter7->first.at(j)) {
						totalProb += iter7->second;
						goto nextWord;
					}
				}	
			}
			nextWord:;
		}
		(*iter8).second = totalProb;
	}
	char c = '\0';
	float t = 0;
	//Printing the most probable letter
	for (auto iterF = letterMap.begin(); iterF != letterMap.end(); iterF++) {
		if ((*iterF).second >= t) {
			c = (*iterF).first;
			t = (*iterF).second;
		}	
	}
	cout << "Letter is: " << c << " with probability: " << t << endl;
}
