#define TESTFILE "../test/input1.txt"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "pssmathparser.h"

using namespace std;
using namespace PssMathParser;

int main()
{
    cout << "######################################" << endl;
    cout << "############### TEST 1 ###############" << endl;
    cout << "######################################" << endl;
    cout << endl;
    bool output = false, testFailed = false;

    // Check if file exists
    ifstream infile(TESTFILE);
    if (infile.is_open() == false) {
        testFailed = true;
        cout << "Error: Could not open input file '" << TESTFILE << "'" << endl;
        cout << endl;
        return 1;
    }

    // Run tests
    string line, outputLine;
    MathParser *mp = MathParser::makeMathParser();
    uint16_t counter = 1;
    while(getline(infile, line)) {
        if(line[0] != '#') {
            if(output == false) {
                // Input
                cout << counter << ".test line: '" << line << "'" << endl;
                mp->setExpression(line);
                mp->expressionToReversePolish();
                outputLine = mp->reversePolish();
                cout << "  - expression: '" << mp->expression().data()
                     << "'" << endl;
                cout << "  - reversePolish: '" << outputLine.data()
                     << "'" << endl;
//                cout << "  - math after expansion = '"
//                     << mp->mathToStringFull().data() << "'" << endl;
                output = true;
            }
            else {
                // Output
                cout << counter << ".expected output: '" << line
                     << "'" << endl;
                if (outputLine == line) {
                    cout << counter << ".TEST PASSED" << endl;
                }
                else {
                    cout << counter << ".TEST FAILED" << endl;
                    testFailed = true;
                    break;
                }
                cout << endl;
                counter++;
                output = false;
                mp->clear();
            }
        }
    }

    // Test summary
    cout << endl;
    cout << "######################################" << endl;
    if (testFailed) {
        cout << "#### TEST SUMMARY: TESTING FAILED ####" << endl;
    }
    else {
        cout << "### TEST SUMMARY: ALL TESTS PASSED ###" << endl;
    }
    cout << "######################################" << endl << endl;

    // Clear used data
    infile.close();
    delete mp;
    return 0;
}
