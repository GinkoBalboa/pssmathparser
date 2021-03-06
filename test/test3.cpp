#define TESTFILE "../test/input3.txt"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include "pssmathparser.h"

using namespace std;
using namespace PssMathParser;

int main()
{
    cout << "######################################" << endl;
    cout << "############### TEST 3 ###############" << endl;
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
    string line, outputLine, variableName, variableValue;
    double dout, dvalue;
    bool readingValue=false;
    MathParser *mp = MathParser::makeMathParser();
    uint16_t counter = 1, numVariables=0;
    while(getline(infile, line)) {
        if(line[0] != '#') {
            if(output == false) {
                // Input
                cout << counter << ".test line: '" << line << "'" << endl;
                mp->setMath(line);

                // Get number of variables
                numVariables = mp->getVariableSize();

                // Set variables
                for(uint16_t i=0; i<numVariables; i++) {
                    readingValue = false;
                    getline(infile, line);
                    variableName.clear();
                    variableValue.clear();
                    for(uint16_t j=0; j<line.size(); j++) {
                        if(readingValue == true) {
                            variableValue += line[j];
                        }
                        else if (line[j] != '=') {
                            variableName += line[j];
                        }
                        else if (line[j] == '='){
                            readingValue = true;
                        }
                    }
                    dvalue = atof(variableValue.data());
                    mp->setVariableDouble(variableName, dvalue);
                }

                // Calculate the expression
                dout = mp->calculateExpression();

                ostringstream s;
                s.setf(ios::scientific);
                s << setprecision(7) << dout;

                // Here we add another digit in the exponent if it has only two
                string stmp = s.str();
                if(stmp.size() - stmp.find('e') == 4)
                    stmp.insert(stmp.find('e') + 2, "0");
                outputLine = stmp;

                cout << "  - expression: '" << mp->expression().data()
                     << "'" << endl;
                cout << "  - reversePolish: '" << mp->reversePolish().data()
                     << "'" << endl;
//                cout << "  - math after expansion = '"
//                     << mp->mathToStringFull().data() << "'" << endl;
                cout << "  - result = " << outputLine.data() << endl;
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
