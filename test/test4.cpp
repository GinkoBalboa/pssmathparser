#define TESTFILE "../test/input4.txt"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include "pssmathparser.h"
#include <time.h>

using namespace std;
using namespace PssMathParser;

double function1(double var1, double var2, double var3) {
    // Io*(exp(qe*V/(kBJ*(ToK+TC)))-1)
    return var1*(exp(1.6021766208e-19*var2/(1.38064852e-23*(273.15+var3)))-1);
}

double function2(double var1, double var2, double var3) {
    // Io*(exp(qe*V/(kBJ*(ToK+TC)))-1)+Io*(exp(qe*V^2.5/(kBJ*(ToK+TC)))-2)
    return var1*(exp(1.6021766208e-19*var2/(1.38064852e-23*(273.15+var3)))-1)
            + var1*(exp(1.6021766208e-19*pow(var2,2.5)
                        /(1.38064852e-23*(273.15+var3)))-2);
}
double function3(double var1) {
    // cos(2*pi*3*t)*exp(-pi*t^2)
    return cos(2*3.14159265358979323846*3*var1)
            *exp(-3.14159265358979323846*pow(var1,2));
}

int main()
{
    cout << "######################################" << endl;
    cout << "############### TEST 4 ###############" << endl;
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
    double dout, factor, offset, parseTime, funcTime, funcCleanTime;
    double tmp;
    bool readingValue=false;
    MathParser *mp = MathParser::makeMathParser();
    uint32_t counter = 1, numVariables=0;
    uint32_t iterations;
    clock_t start, end;
    vector<vector<double>> variable;
    vector<string> varName;
    while(getline(infile, line)) {
        if(line[0] != '#') {
            if(output == false) {
                // Input
                cout << counter << ".test line: '" << line << "'" << endl;
                mp->setMath(line);

                // Get number of variables
                numVariables = mp->getVariableSize();

                // Set variable vectors
                variable.clear();
                varName.clear();
                for(uint32_t i=0; i<numVariables; i++) {
                    // Initialize
                    if(i==0) {
                        readingValue = false;
                        getline(infile, line);
                        variableName.clear();
                        variableValue.clear();
                        for(uint32_t j=0; j<line.size(); j++) {
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
                        iterations = atoi(variableValue.data());
                    }
                    for(uint32_t k=0; k<3; k++) {
                        readingValue = false;
                        getline(infile, line);
                        variableName.clear();
                        variableValue.clear();
                        for(uint32_t j=0; j<line.size(); j++) {
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
                        if(variableName=="factor") {
                            factor = atof(variableValue.data());
                        }
                        else if(variableName=="offset") {
                            offset = atof(variableValue.data());
                        }
                        else {
                            varName.push_back(variableName);
                            variable.push_back(vector<double>());
                            for (uint32_t ii=0; ii<iterations; ii++) {
                                variable[i].push_back(offset + (double)ii*factor);
                            }
                        }
                    }
                }

                start = clock();
                // Calculate expression from the parser
                for (uint32_t i=0; i<iterations; i++) {
                    for(uint32_t j=0; j<numVariables; j++) {
                        mp->setVariableDouble(varName[j], variable[j][i]);
                    }
                    // Calculate the expression
                    dout = mp->calculateExpression();
                    tmp += dout;
                }
                end = clock();
                parseTime = (double)(end-start)/iterations/CLOCKS_PER_SEC;

                start = clock();
                // Calculate expression from the function
                for (uint32_t i=0; i<iterations; i++) {
                    // Calculate the expression
                    if(counter == 1)
                        dout = function1(variable[0][i], variable[1][i], variable[2][i]);
                    else if(counter == 2)
                        dout = function2(variable[0][i], variable[1][i], variable[2][i]);
                    else if(counter == 3)
                        dout = function3(variable[0][i]);
                    tmp += dout;
                }
                end = clock();
                funcCleanTime = (double)(end-start)/iterations/CLOCKS_PER_SEC;

                start = clock();
                // Calculate expression from the function with delay same as
                // the parser (for setting variables)
                for (uint32_t i=0; i<iterations; i++) {
                    for(uint32_t j=0; j<numVariables; j++) {
                        mp->setVariableDouble(varName[j], variable[j][i]);
                    }
                    // Calculate the expression
                    if(counter == 1)
                        dout = function1(variable[0][i], variable[1][i], variable[2][i]);
                    else if(counter == 2)
                        dout = function2(variable[0][i], variable[1][i], variable[2][i]);
                    else if(counter == 3)
                        dout = function3(variable[0][i]);
                    tmp += dout;
                }
                end = clock();
                funcTime = (double)(end-start)/iterations/CLOCKS_PER_SEC;


                cout << "  - expression: '" << mp->expression().data()
                     << "'" << endl;
                cout << "  - reversePolish: '" << mp->reversePolish().data()
                     << "'" << endl;
                //                cout << "  - math after expansion = '"
                //                     << mp->mathToStringFull().data() << "'" << endl;
                //                cout << "  - result = " << outputLine.data() << endl;
                cout << scientific;
                cout << "  - parse time var loading   = "
                     << setprecision(7) << parseTime << " s" << endl;
                cout << "  - func time var loading    = "
                     << setprecision(7) << funcTime << " s" << endl;
                cout << "  - parse time no var loading= "
                     << setprecision(7)
                     << parseTime - (funcTime -  funcCleanTime)
                     << " s" << endl;
                cout << "  - func time no var loading = "
                     << setprecision(7) << funcCleanTime << " s" << endl;
                cout << "  - speed ratio parser/func         = "
                     << fixed << setprecision(3)
                     << (parseTime - (funcTime -  funcCleanTime))/funcCleanTime
                     << endl;
                cout << "  - time spent on loading variables = "
                     << fixed << setprecision(3)
                     << 100*(funcTime -  funcCleanTime)/parseTime << " %"
                     << endl;

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
