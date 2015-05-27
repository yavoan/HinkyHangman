#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <time.h> 
#include <string.h>
#include <stdlib.h> 
using namespace std;

list<string> wordlist;
list<string>::iterator count;
int randlength;
string theword;
string playagain="haha";

list<string> used;
//Makes the dictionary
//and chooses the random length
void make(char* filename) 
{
	
	ifstream input(filename);
	if (input.is_open())
	{
    	string line;
	
		//Random Generator
		srand (time(NULL));
		randlength = rand() % 10;
		//Filling the dictionary with words that are of randlength
    	while( getline( input, line ) ) 
		{
			if (line.length()==randlength )
			{
				wordlist.push_back(line);
			}
    	}
	}
	else
	{
		cout<<"File doesn't exist\n";
		exit(0);
	}
	input.close();
	if (wordlist.size()==0)
	{
		cout<<"In this dictionary there are no words with size "
			<<randlength<<".\n";
		exit(0);
	}
	cerr<<"Wordlist size: "<<wordlist.size()<<endl;
}

//Gets random word from list
void randomword()
{
	srand(time(NULL));
	int z =0;
	int random = rand() % wordlist.size();
	for (count=wordlist.begin(); count!=wordlist.end() ; ++count )
	{
		if (z == random)
			theword = *count;
		++z;
	}
}
/*
 *The main algorithm. 
 *This checks how many words in the list
 *contain the letter and removes words
 *from the list accordingly. Plus a few extra checks
 */
int remove(string g)
{
	string temp;
	int contains =0;
	for (count=wordlist.begin(); count!=wordlist.end(); ++count)
	{
		temp = *count;
		if (  temp.find(g) != string::npos )
			++contains;
	}

	if (contains==0)
	{
		randomword();
		return 0;
	}
	for (count=wordlist.begin(); count!=wordlist.end(); ++count)
	{	
		temp = *count;
		//cout<<"contains: "<<contains<<"wordlistsize "<<wordlist.size()<<"\n";;
		if ( contains <= (wordlist.size()-contains) )
		{
			if (  temp.find(g) !=string::npos )
			{
				wordlist.erase(count);
				--count;
				--contains;
				randomword();
			}
		}
		else if ( contains > (wordlist.size()-contains)  )
		{
			if (  temp.find(g) == string::npos )
			{
				wordlist.erase(count);
				--count;
				randomword();
			}
		}
	}
	/*
	 *Looking to see which one has the most of the guessed letter
	 *For example if the words are beer bear and date and letter is e
	 *then this code snippet would remove beer
	 */
	if (wordlist.size() == contains)
	{
		randomword();
		int lettercount;
		int lettermin=99;
		int z;
		for (count=wordlist.begin(); count!=wordlist.end(); ++count)
		{
			lettercount=0;
			temp = *count;
			for (z=0; z<temp.length(); ++z)	
			{
				if (g[0]==temp[z])
					lettercount++;
			}
			if (lettercount<lettermin)
			{
				lettermin =lettercount;
			}
		}
		
		for (count=wordlist.begin(); count!=wordlist.end(); ++count)
		{
			lettercount=0;
			temp = *count;
			//cout<<temp<<"\n";
			for (z=0; z<temp.length(); ++z)	
			{
				if (g[0]==temp[z])
					lettercount++;
			}
			if (lettercount>lettermin)
			{
				wordlist.erase(count);
				--count;
				randomword();
			}

		}
	}
	return 1;
}
/*
 *Once a word is shown on the game board
 *I remove all the words from the dictionary
 *that don't have the same position as the one shown
 *on the game board
 */
void removepos(int pos)
{
	string temp;
	for (count=wordlist.begin(); count!=wordlist.end(); ++count)
	{
		temp = *count;
		if (temp[pos] != theword[pos])
		{
			wordlist.erase(count);
			--count;
		}
		
	}
}
bool checkalpha(string let, char alpha[])
{
	int i;
	if (let.length()!=1)
		return false;
	for (i=0;i<26;++i)
	{
		if ( alpha[i] == let[0] )
		{
			alpha[i]=' ';
			return true;
		}
	}
	return false;
}

int main(int argc, char* argv[])
{
	bool wingame;
	int x;
	int guesses;
	string guess;
	
	while (playagain.compare("quit")!=0)
	{

	char alpha [26] ={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
					'p','q','r','s','t','u','v','x','w','y','z'};
	make(argv[1]);	
	cout << "Welcome to Hangman!\n";
	//***Making the Array with _ _ _ _ etc
	string playfield [randlength];
	for (x=0; x<randlength; ++x)
		playfield[x]="_";

	guesses=10;
	
	while (guesses>0)
	{
		cout << "Please enter your guess: ";
		cin >> guess;
		
		while ( checkalpha(guess,alpha)==false )
		{
			cout << "Enter a correct guess: ";
			cin >> guess;
		}
		
		int guessremover = remove(guess);
		if (guessremover==0)
		{
			cout<<"Sorry there are no "<<guess<<"'s.\n\n";
			--guesses;
		}
		used.push_back(guess);
				
		int space;
		bool lettertrue = false;
		cerr << "Wordlist size: "<<wordlist.size()<<endl;
		for (space=0; space<randlength; ++space)
		{
			if ( theword[space]==guess[0] )
			{
				playfield[space] = guess[0];
				lettertrue = true;
				removepos(space);
			}
			cout<< playfield[space];
			cout<<" ";
	
		}
		cout << "\n";
		if (lettertrue == false && guessremover!=0)
		{
			cout<<"Sorry there are no "<<guess<<"'s.\n\n";
			--guesses;
		}
		wingame=true;
		for (x=0; x<randlength; ++x)
		{
			string temp = playfield[x];
			if ( theword[x] != temp[0] )
				wingame=false;
		}
		if (wingame)
		{
			while (used.size()>0)
				used.pop_front();
			while (wordlist.size()>0)
				wordlist.pop_front();
			cout<<"You Win!\n";
			cout<<"Play Again? (type quit to quit or any letter to continue) ";
			cin >> playagain;
			cout<<"\n\n************************\n";
			break;
		}

		cout << "You have "<<guesses <<" guesses remaining\n";
		cout<<"Used Letters: ";
		for (count=used.begin();count!=used.end();++count)
		{
			cout<<*count<<" ";
		}
		cout<<"\n";
		//cout << "\nWord list size "<<wordlist.size();
	}
	if (!wingame)
	{
		while (used.size()>0)
			used.pop_front();
		while (wordlist.size()>0)
			wordlist.pop_front();
		cout<<"The word was "<<theword<<".\n";
		cout<<"Play Again? (type quit to quit or any letter to continue) ";
		cin >> playagain;
		cout<<"\n\n************************\n";
	}

	}//end play again while
}































