#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

bool printErrors = false;
int totalWordCount = 0;
void prnt(string s)
{
	if(printErrors)
		cout << s << endl;
}

/* 
	vector<string> tokenize(string)
	===================================================================================
	responsible for splitting up the input into words
	creates a vector of strings (size of char** is undetermined until
		parsing is done, so I use vectors because of their dynami size
	
	loops through all the letters of the input, and once a delimiter(space, etc)
		is found, it will merge the letters into a word, add it to the vector,
		and reset the word (word = "")

		
*/

vector<string>  tokenize(string input)
{
	vector<string>  v; // contains all the words delimited by spaces and ;

	int cnt = 0, wordCnt = 0; // cnt=index in input string, wordCnt = current word length
	string word; // add letters from input to this string, then add it to vector when delim. found	

	for(string::iterator it=input.begin(); it != input.end(); it++)// not null
	{
		if(input.at(cnt) == '#')
		{
			if(word != " " && !word.empty()) 
			{
				v.push_back(word);
				
				//cout << "daWord: " << word << endl; 

			}
			v.push_back(";");
			word = "";
			break;
		}
		else if(input.at(cnt) == ' ') // delimiter #1 - space
		{
			//prnt("Found a space");
			
			// check if this space matters at all.. eg: " echo..." should be "echo ..."
			if(!word.empty() && word != "" && word != " ") {
				// add the word we have and reset it
				v.push_back(word);	
			//	cout << "word: " << word << "." << endl;
				word = "";
				totalWordCount++;
				prnt("Delete worked fine");

			}
			else { prnt("hi");  } // to stop the wordCnt++ 
		}
		else if(input.at(cnt) == ';') // delimiter #2 - semicolon
		{
			if(word != "" && word != " ") {
				v.push_back(word); // push back w/e word we have
				totalWordCount++;
			}
			v.push_back(";"); // then add a semicolon [as a new 'word' or element in the vector]
			word = ""; // reset word
		}
		else
		{
			prnt("Found a normal character");
			
		   word += input.at(wordCnt);	
			//cout << "kk: "  << word << endl;
		}
			wordCnt++;
			cnt++;
	}
	
	// add the last word (since it wasnt proccessed by the for loop above)
	if(word != "" && word != " ")
	{
		v.push_back(word);
		if(word != ";")
		{
			v.push_back(";");
		}	
		totalWordCount++;
	}
	totalWordCount++;
	
	if(v.at(v.size()-1) != ";") { v.push_back(";"); }
	totalWordCount = v.size();
	return v;
}

int main(int argc, char** argv)
{
	//cout << "Initializing command prompt.." << endl;
	string input;
	bool cont = true;

	char hostname[128];
	gethostname(hostname, sizeof hostname);
	while(cont) // perpetual until a break (when last cmd is used)
	{
		// get proper prompt display and show
		cout <<  getlogin() << "@" << hostname << "$ ";
		cout.flush();

		// Wait for input
		std::getline (std::cin, input);

		// get string, then convert it to a c* array
		vector<string> words = tokenize(input);
		//cout << "wordsCnt: " << words.size();
		//for(int i =0; i<words.size(); i++) { cout << "w" << i << ": " << words[i] << endl; }		
		const int finalWordCount = totalWordCount+1;
		//char* list[finalWordCount]; 
		char **list = new char*[finalWordCount];
		int count = 0;
		for(int i = 0; i < totalWordCount; i++) 
		{
			//cout << "TWC " << totalWordCount << "..i=" << i << endl; 
			if((words[i] == ";") || words[i] == "||" || (words[i] == "&&"))
			{
				  // execute the command
				  pid_t pid;
				  int status = 0;
				  pid = fork();

				  if(pid <= -1) // something went wrong 
				  {
					  perror("ERROR [FORK]: CHILD FAILED\n");
					  exit(1); // quit the program
				  }
				  else if(pid == 0) // child process
				  {
					  //for(int n = 0; n < 5; n++) { cout << "k: " << list[n] << endl; }
					  //list[0] = "echo"; list[1] = "hi";  list[2] = NULL;
					  list[count] = NULL;
					  int success = execvp(list[0], list);
					  if(success <= -1) // nope, it failed
					  {	
						  perror("ERROR: EXECUTING THE CMD FAILED\n");
						  exit(1); // dip
					  }
					  else { cout  << "It succeeded..." << success << endl; }
					  
				  }
				  else // parent process---wait until the child is done
				  {
					  waitpid(-1, &status, 0);
						//cout << "THE STATUS: " << status << endl;
					  if(words[i] == "&&" && (status > 0))
					  		break;

						if(words[i] == "||" && (status <= 0))
							break;
				  }

					// reset the list
					for(int j = 0; j < count; j++) { 
						list[j] = NULL;
					}	
				   count = 0;
			}	
			else {
				//cout << "Adding word" << endl;
				if(count == 0 && words[i] == "exit")
				{
					exit(1);
				}
				list[count] = new char[words[i].size()+1];
				copy(words[i].begin(), words[i].end(), list[count]);
				list[count][words[i].size()] = '\0';

				//cout << "..." << list[count] << endl;
				count++;
			}
		} 

		//list[totalWordCount] = NULL;
		totalWordCount = 0;			
	}
	return 0;
}
// run rshell within rshell
// run it on local files
// LOOK INTO WAIT 
