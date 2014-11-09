/* HIGH LEVEL DESCRIPTION OF PROGRAM

	This program will compare the output of commands, given
	in an INPUT file, of bash and rshell, and display which commands
	failed and passed

	1. Create directory for the user/bash logs. Will be used more extensively later
	2. Loop through each line of the INPUT file
		3. Create an rshell instance, and using script -c along with
			redirection, input the command. Record output to log
		3b. Sleep for .25 seconds, or else the the while loop will
			iterate too quickly and the program wont work
		4. Same as 3, but for the bash instance
		5. call/store compareOutput() to see if the two log files are the same
	6. Cleanup directories/open files 
	7. Print the results

	** TODO
	1. Testing. Needs more testing for edge cases, etc
	2. (basic) cheating detection -- for echo statements, etc
	3. Check to see if the environment (ls -lra, pwd) is the same
		after each command. This will help test cd, mkdir, etc
	4. Better formatting, more specific feedback
	5. ??? 

	- Jamal Moon
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// The path to your rshell program. This is important!
const string PATH_TO_RSHELL = "/bin/rshell"; 


// Two directories will be made. They will each be the testing environments
//		for the user/bash shell versions. The purpose of this is to test
//		that cd, pwd, ls, etc all work properly
//	They will also contain the log files.
//	The directories may be automatically deleted afterwards, if AUTO_DELETE=true
const string USER_FOLDER	= "user_folder_tmp";
const string BASH_FOLDER	= "bash_folder_tmp";
const bool AUTO_DELETE	= false; // Delete the files after you're done?

// The name of the log files
const string USER_OUT_FILE = "user_out.log";
const string BASH_OUT_FILE = "bash_out.log";

const string bashpath = (BASH_FOLDER + "/" + BASH_OUT_FILE);
const string userpath = (USER_FOLDER + "/" + USER_OUT_FILE);

// Input file
// Has list of commands-to-be-tested delimited by a line
const string INPUT_FILE = "input";


// Just like system(xxx.c_str()), but faster to type. 
int cmd(string cmdstr) {
	return system(cmdstr.c_str());
}


// A simple struct that will be used in a vector to record
//		the results of the tests
struct test_case {
	string command;
	bool pass;
};


// fixString
// Param: string (one line of text in user/bash.log) by address
// Purpose: Removes random characters (^M at end of lines)
//		and deletes the "Script started on...Script done on...*date*" 
//		lines because they are unnecessary for comparisons
void fixString(string &line)
{
	if(line.find("Script started on ") != string::npos || line.find("Script done on ") != string::npos)
	{
		line = "";
	}
	line.erase(std::remove(line.begin(), line.end(), '\x0D'), line.end());

	//line = "\"" + line + "\"";
}


// compareOutput
// PARAM: none
// Purpose: compares the user.log and bash.log files
// Returns true if they are the same, false otherwise

bool compareOutput() {
	// Create a file stream for the user.log and bash.log
	//		so we cane read and compare
	ifstream user(userpath.c_str());
	ifstream bash(bashpath.c_str());

	// Turns false when the input is not the same
	// or when a file has reached the end but the other hasn't
	// I made this variable instead of just returning in the loop
	//		so that I can close the files before returning
	bool retValue = true;

	string userline; // a single line from the user log
	string bashline; // a single line from the bash log

	// Loop thru userlog.
	// Bash log could be larger than userlog, so check afterwards
	while(getline(user, userline)) {
		getline(bash, bashline);
		
	//	cout << "Userline: " << userline << endl;
	//	cout << "Bashline: " << bashline << endl;

		// Fix the strings to remove the pesky ^M character + other things
		fixString(bashline);
		fixString(userline);
		
		cout << "bl: " << bashline << endl;
		cout << "ul: " << userline << endl;

		// Compare
		if(userline != bashline)
			retValue = false;	
	}

	// Check if the bash log file has more space in the file. If so, 
	//	the user and bash files are not equivalent.
	if(retValue)
	{
		//	cout << "Testing if " << BASH_LOG_FILE << " is also complete" << endl;
		retValue = !getline(bash, bashline);  
	}

	// Close files and return
	user.close();
	bash.close(); 
	return retValue;
}


//	clearOutputFiles
//	Param: none
//	Purpose: clears the bash/user log files from any text
//	Returns nothing
//	CURRENTLY NOT USED
void clearOutputFiles()
{
	ofstream user(userpath.c_str());
	ofstream bash(bashpath.c_str());
	user.close();
	bash.close();
}


/* createLogs
	Param: none
	Purpose: to create the log files with proper parameters
	Return: nothing
	CURRENTLY NOT BEING USED 
*/

void createLogs()
{
	ofstream user (userpath.c_str());
	ofstream bash (userpath.c_str());

	string x = "chmod a+x " + userpath;
	string y = "chmod a+x " + bashpath;
	cmd(x);
	cmd(y);
}

int main()
{
	// Make these directories for separate testing environments
	cmd("mkdir " + USER_FOLDER);
	cmd("mkdir " + BASH_FOLDER);

	// Test list
	// Each command from the input file will be added to this vector
	//		and it will provide information about whether it passed/failed
	vector<test_case> tests;

	// ifstream of the INPUT_FILE.. this contains the list of commands to test
	ifstream infile(INPUT_FILE.c_str());

	// Go through each command in the input file
	string line; // contains the command that is currently being tested
	while (getline(infile, line)) {
		// Make sure they are clean before doing anything.	
		//clearOutputFiles();
		//createLogs();

		// Make a file for redirection
		ofstream ofile ("commandtext.txt");
		ofile << line << endl;
		ofile << "exit" << endl;
		ofile.close();

		// test the user's shell
		string theCommand = "script -c " + PATH_TO_RSHELL + " < commandtext.txt "
			+ userpath;
		cmd(theCommand);

		// Add a delay. Change it for slower machines, although this value
		//	should be sufficient.
		// If this while loop runs too quickly, the program won't run properly
		cmd("sleep .25");

		// Test the actual bash
		cout << "\nTesting bash shell" << endl;
		string cmd2 = "script -c \"" + line + "\" " + bashpath;
		// Run the command and exit
		cmd(cmd2);
		cmd("exit");

		// Create an instance of a test case
		// Add it to the tests vector
		test_case tc;
		tc.pass = compareOutput();
		tc.command = line; 
		tests.push_back(tc);

		cmd("rm commandtext.txt");
	}
		

	// Remove the directories
	// DO NOT screw this up or else everything might just get deleted...
	if(AUTO_DELETE) {
		cmd("rm -r " +  USER_FOLDER);
		cmd("rm -r " + BASH_FOLDER);
	}

	// close the inputfile
	infile.close();

	// Output test cases
	cout << "Printing test results." << endl;
	int total=0, passed=0;
	for(total = 0; total < tests.size(); total++)
	{
		cout << "Test case " << total << ":" << endl;
		cout << "Command: " << tests.at(total).command << endl;
		cout << "Pass: " << tests.at(total).pass << endl;
		cout << endl;

		if(tests.at(total).pass)
			passed++;
	}
	cout << "Final Results: " << passed << "/" << total << " passed. Gj" << endl;
	return 0;
}


