<h1>Rshell Unit Tester
<h4>By Jamal M. Moon</h4>

<h2>Table of Contents</h2>
<ol>
    <li><a href="#introTag">Introduction</a></li>
    <li><a href="#featuresTag">Features</a></li>
    <li><a href="#howToGetTag">How To Install</a></li>
    <li><a href="#howToUseTag">How To Use</a></li>
    <li><a href="#bugsTag">Bugs/Limitations</a></li>
    <li><a href="#toDoTag">To-Do List</a></li>
    <li><a href="#howItWorksTag">How It Works</a></li>
</ol>

<h1><a name="introTag">Introduction</a></h1>

This project will allow one to quickly test the functionality of his or her rshell program and receive basic feedback (eg: which commands failed/passed). This program reads an input file (in the folder: <i>tests/input</i>), executes the bash shell and the student's shell, stores and compares the output. If the output differs, then it is recorded as a fail and printed out when the entire program finishes

<h1><a name="featuresTag">Features</a></h1>

<ul>
    <li>Provides an orderly, structured method to create and test your cases</li>
    <li>Test cases and commands are stored in a file and can be quickly accessed or changed for new/different tests</li>
    <li>Quickly determines whether or not your rshell output matches the bash output</li>
        <ul><li>Prints all test cases at the end of the program, with the command name and if it passed or failed</li></ul>
</ul>

<h1><a name="howToGetTag">How To Install</a></h1>

<p>
1) git clone http://github.com/jmoon018/rshell_grader.git
<br>
2) cd rshell_grader
<br>
3) make
<br>
*4) add 'shelltester' to PATH
</p>

<h5>*For help: <a href="http://www.linfo.org/path_env_var.html">Go here</a> </h5>

<h1><a name="howToUseTag">How To Use</a></h1>

<p>If you added 'shelltester' to your PATH, you can navigate to any directory and run the unit tester -- there is no need to install the unit tester again. When you are in the directory containing the rshell files, you can run 'shelltester' there or in the /bin folder and the results should be the same. However, note that you must have the <i>input</i> file in the <i>tests</i> folder, which must be one directory above the <i>bin</i> folder. Here is an example layout:
<br>
<br>
<b>Main Homework Folder</b>
<br>
<ul>
    <li>-tests [folder]</li>
    <li>-bin [folder]</li>
    <li>-src [folder]</li>
    <li>-LICENSE</li>
    <li>-README.md</li>
    <li>-Makefile</li>
</ul>

<b>bin folder:</b>
<ul>
    <li>rshell</li>
    <li>ls</li>
</ul>

<b>tests folder:</b>
<ul>
    <li>input </li>
</ul>

The necessary components are: tests/input (which stores the list of commands) and bin/rshell (the program that is being unit tested).


<p>Navigate to your <i>bin</i> folder and then type ./autograder when you are ready to run the program.<p>
<p>To change the test cases, navigate to the <i>tests</i> folder, open the <i>input</i> file and add a command you want to test. Make sure that each command has its own separate line. Don't leave an extra, empty line or else the program
may give you a "fail" for that 'test.'</p>

<h1><a name="bugsTag">Known Bugs/Limitations</a></h1>
<ol>
    <li>More testing required</li>
    <li>Does not check environment</li>
    <li>The formatting and feedback can use some improvement</li>
</ol>

<h1><a name="toDoTag">To Do List</a></h1>
<ul>
    <li>Automate testing for all students---the program should download all the student's files and test them</li>
    <li>Detect cheating</li>
    <li>Detect minor differences in output (different but still valid output)</li>
    <li>Formatting and feedback can be nicer</li>
    <li>Add a file of the commands that failed</li>
    <li>Ensure the environment is correct after each command</li>
</ul>

<h1><a name="howItWorksTag">How It Works</a></h1>

<ul>
<li>1. Create directory for the user/bash logs. Will be used more extensively later (to test environment with ls/pwd)</li>
<li>  2. Loop through each line of the INPUT file</li>
<li>  3. Create an rshell instance, and using "script -c along" with redirection, input the command. Record output to log</li>
    <li> 3b. Sleep for .2 seconds, or else the the while loop will iterate too quickly and the program wont work</li>
<li> 4. Same as 3, but for the bash instance</li>
<li> 5. call/store compareOutput() to see if the two log files are the same</li>
    <li> 5b. Before doing so, the log files are quickly fixed; any random characters, and the timestamp, are removed.</li>
<li> 6. Cleanup directories/open files </li>
<li> 7. Print the results</li>
</ul>

<p>
  The source file has many comments to help everyone follow along. If there are any questions, let me know.
</p>

If this program is not working, or if you have suggestions to fix it, please email me at jmoon018@ucr.edu or make an issue. Thanks
