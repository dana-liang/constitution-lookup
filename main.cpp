/* Prog4TwoCodes.cpp
 *     Lookup for words/phrases within the U.S. Constitution and Bill of Rights
 *     Pair programmed by:
 *     Tristan Maltizo
 *     Dana Liang
 */

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cctype>     // Allows using the tolower() function
#include <vector>     // Allows using vectors
#include <string>     // Allows using C++ string functions
using namespace std;

// Function declarations

// Opens ConstitutionAndBillOfRights.txt and converts it into a vector, storing each line with a value
void convertVector(vector<string> &vect) {
    // Open a file for reading
    ifstream inStream; // Declare an input stream
    inStream.open("ConstitutionAndBillOfRights.txt"); // In clion you have to put ConstitutionAndBillOfRights.txt
                                                         // in the cmake-build-debug file
    if (!inStream.is_open()) { // If file not found, exit
        cout << "Could not find ConstitutionAndBillOfRights.txt.  Exiting..." << endl;
        exit(-1);
    }

    int size; // will hold size of the current line
    string theLine; // current line

    while (getline(inStream, theLine)) {
        size = theLine.size();
        if (size > 1) {// Add line to the vector if the num of chars in that line > 1 (newline char)
            vect.push_back(theLine);   // Adds line to new index, which also increases the vector size
        }
    }
    // close the file
    inStream.close();
}

// Search the vector containing the lines of the Constitution for a word or phrase, return the index of the line
// where the word or phrase is found. Includes optional parameter for the A function (display all occurrences of
// found word or phrase).
int lineNum(vector<string> vect, string searchKey, int startIndex = 0) {
    int index = 0;
    if (startIndex == 0) { // for displaying the first occurrence of found word or phrase
        for (int i = 0; i < vect.size(); i++) {
                for (int j = 0; j < (vect.at(i)).size(); j++) { // set the string to search to lowercase
                    (vect.at(i)).at(j) = tolower((vect.at(i)).at(j));
                }
                if ((vect.at(i)).find(searchKey) != string::npos) {
                    index = i;
                    break;
                }
                else { // If not found, set index to after the final line
                    index = vect.size();
                }
        }
    }

    else { // for displaying all occurrences of found word or phrase
        // begin search in the line following the line containing the first occurrence
        for (int i = startIndex + 1; i < vect.size(); i++) {
            for (int j = 0; j < (vect.at(i)).size(); j++) { // set the string to search to lowercase
                (vect.at(i)).at(j) = tolower((vect.at(i)).at(j));
            }
            if ((vect.at(i)).find(searchKey) != string::npos) {
                index = i;
                break;
            }
            else {
                index = -1;
            }
        }
    }
    return index;
} // END lineNum

void displayLine(vector<string> vect, int index) {
    cout << "Line " << index << ":" << endl
         << vect.at(index) << endl << endl;
} // END displayLine

void displayMenu(bool isFirst) {
    if (isFirst) { // Display initial menu at program start
        cout << "Select from the following choices:" << endl
             << "   W  Enter word or phrase to find, and display enclosing text" << endl
             << "   X  Exit program" << endl
             << "Your choice: ";
    }

    if (!isFirst) {
        cout << "Select from the following choices:" << endl
             << "   W  Enter word or phrase to find, and display enclosing text" << endl
             << "   P  Display previous text" << endl
             << "   F  Display following text" << endl
             << "   A  Display all occurrences of found word or phrase" << endl
             << "   X  Exit program" << endl
             << "Your choice: ";
    }
} //END displayMenu

//---------------------------------------------------------------------------------------------------
int main() {

    // Declare variables
    char menuOption;
    char tmpChr;
    string wordPhrase;
    vector<string> constitutionVector;  // string type vector containing lines of the constitution
    bool firstTime = true; // Opens features needed only after the first loop

    // Assign the Constitution into a string vector
    convertVector(constitutionVector);

    displayMenu(firstTime); // Display the initial menu and prompt user for input

    cin >> menuOption;
    cin >> tmpChr; // Remove space before keyword
    getline(cin, wordPhrase);
    for (int i = 0; i < wordPhrase.size(); i++) { // set phrase to lowercase
        wordPhrase.at(i) = tolower(wordPhrase.at(i));
    }

    menuOption = tolower(menuOption); // Convert to lowercase

    // OUTER switch cases for INITIAL menu options
    switch ( menuOption) {
        case 'w': {
            firstTime = false;
            while (menuOption == 'w') {
                //current index in the constitution vector, also is the line number in the constitution
                int currIndex = lineNum(constitutionVector, wordPhrase);
                if (currIndex >= 0 && currIndex < constitutionVector.size()) {
                    cout << "Enter word or phrase: " << endl << endl; // Prompt user for input
                    displayLine(constitutionVector, currIndex); // display the line where wordPhrase is found
                }

                displayMenu(firstTime); // display extra options available after first search

                while (menuOption != 'x') {
                    cin >> menuOption; // prompt user for input again for extra options
                    menuOption = tolower(menuOption); //convert menuOption to lowercase
                    cout << endl;

                    switch( menuOption) { // INNER switch cases for ADDITIONAL menu options
                        case 'w': { // Enter word or phrase to find, and display enclosing text
                            getline(cin, wordPhrase);
                            break;
                        } // END case 'w'

                        case 'p': { // Display previous text
                            currIndex--;
                            if (currIndex >= 0) {
                                displayLine(constitutionVector, currIndex);
                                displayMenu(firstTime);
                            }

                            else { // If user tries to go past the first line
                                cout << "There is no previous file contents." << endl << endl;
                                displayMenu(firstTime);
                                currIndex = 0;
                            }
                            continue;
                        }// END case 'p'

                        case 'f': { // Display following text
                            currIndex++;
                            if (currIndex < constitutionVector.size()) {
                                displayLine(constitutionVector, currIndex);
                                displayMenu(firstTime);
                            }

                            else { // If user tries to go past the last line
                                cout << "There is no following file contents." << endl << endl;
                                displayMenu(firstTime);
                                currIndex = constitutionVector.size() - 1;
                            }
                            continue;
                        }// END case 'f'

                        case 'a': { // Display all occurrences of found word or phrase
                            displayLine(constitutionVector, currIndex); // Display first occurrence
                            int nextIndex = currIndex;
                            while (nextIndex != -1) { // Display all following occurrences
                                nextIndex = lineNum(constitutionVector, wordPhrase, nextIndex);
                                // Once end of file is reached with no more occurrences, break the loop
                                if (nextIndex == -1) {
                                    break;
                                }
                                displayLine(constitutionVector, nextIndex);
                            }
                            displayMenu(firstTime);
                            continue;
                        }// END case 'a'

                        case 'x': {
                            cout << "Exiting program." << endl;
                            break;
                        }// END case 'x'

                        default: {
                            cout << "Invalid menu option.  Exiting program." << endl;
                            menuOption = 'x';
                        }
                        break;
                    } // END inner switch case
                    break;
                } // END inner while loop
            } // END outer while loop
            break;
        } // END case 'w'

        case 'x': {
            cout << "Exiting program." << endl;
            break;
        }// END case 'x'

        default: {
            // Sanity check
            cout << "Invalid menu option.  Exiting program." << endl;
            break;
        } // END default

    }// END outer switch case

    return 0;
}// END main()