/**
 *  @file pssmathparser.cpp
 *  @brief Source code for the PssMathParser
 *  @date  Nov 15 2017
 *  @author Ginko Balboa
 *  @copyright
 *  This file is part of PssMathParser.
 *
 *  PssMathParser is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PssMathParser is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PssMathParser.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "pssmathparser.h"

using namespace PssMathParser;

/**
* @brief Special characters found in the infix notation
* @details The vector holds the special characters in the infix notation.
* Operators and parenthesis.
*/
vector<char> MathExpression::specialChars =
{
    '+',
    '-',
    '*',
    '/',
    '^',
    '(',
    ')'
};

/**
* @brief Available operators in a map
* @details The map hold the operator name as a key. The map values are set to
* point to a function. These keys are also reserved words for the writing
* expressions.
*/
unordered_map<string, Operator>
MathExpression::operatorMap(
{
            pair<string , Operator> ("+",
            {"+", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Addition,
             nullptr, &MathExpression::add}),

            pair<string , Operator> ("-",
            {"-", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Addition,
             nullptr, &MathExpression::subtract}),

            pair<string , Operator> ("*",
            {"*", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Multiplication,
             nullptr, &MathExpression::multiply}),

            pair<string , Operator> ("/",
            {"/", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Multiplication,
             nullptr, &MathExpression::divide}),

            pair<string , Operator> ("^",
            {"^", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Function,
             nullptr, &pow}),

            pair<string , Operator> ("pow",
            {"pow", EntityType::OperatorInDoubleDoubleOutDouble,
             OperatorPrecedence::Function,
             nullptr, &pow}),

            pair<string , Operator> ("sin",
            {"sin", EntityType::OperatorInDoubleOutDouble,
             OperatorPrecedence::Function,
             &sin}),

            pair<string , Operator> ("cos",
            {"cos", EntityType::OperatorInDoubleOutDouble,
             OperatorPrecedence::Function,
             &cos}),

            pair<string , Operator> ("tan",
            {"tan", EntityType::OperatorInDoubleOutDouble,
             OperatorPrecedence::Function,
             &tan}),

            pair<string , Operator> ("sqrt",
            {"sqrt", EntityType::OperatorInDoubleOutDouble,
             OperatorPrecedence::Function,
             &sqrt}),

            pair<string , Operator> ("exp",
            {"exp", EntityType::OperatorInDoubleOutDouble,
             OperatorPrecedence::Function,
             &exp})
        }
        );

/**
* @brief Available constants in a map
* @details The map hold the constant name as a key. The map values are preset
* here. These keys are also reserved words for the writing expressions.
*/
unordered_map<string, Argument>
MathExpression::constantMap(
{
            pair<string , Argument> ("pi",
            {"pi", EntityType::ArgumentConstant,
             0, M_PI}),

            // 1/pi
            pair<string , Argument> ("invPi",
            {"invPi", EntityType::ArgumentConstant,
             0, M_1_PI}),

            // Electron charge in C
            pair<string , Argument> ("qe",
            {"qe", EntityType::ArgumentConstant,
             0, 1.6021766208e-19}),

            // Boltzmann constant in J/K
            pair<string , Argument> ("kBJ",
            {"kBJ", EntityType::ArgumentConstant,
             0, 1.38064852e-23}),

            // Boltzmann constant in eV/K
            pair<string , Argument> ("kBeV",
            {"kBJ", EntityType::ArgumentConstant,
             0, 8.6173303e-5}),

            // Absolute zero in K
            pair<string , Argument> ("ToK",
            {"ToK", EntityType::ArgumentConstant,
             0, 273.15})

        });

/**
* @brief Factory constructor
* @details We use the factory method to construct an object inside this library.
* The exit point is the MathParser class. All functions that are meant to be
* used are given in MathParser and later overloaded by this library classes.
* @return *MathParser This is a pointer to the constructed MathParser through
* its child MathExpression.
*/
MathParser *MathParser::makeMathParser() {
    return new MathExpression;
}

/**
 * @brief Virtual destructor because this class is pure virtual
 */
MathParser::~MathParser()
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "MathParser destructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Overloaded addition of two doubles
 * @param a_arg1
 * @param a_arg2
 * @return a_arg1+a_arg2
 */
double MathExpression::add(const double a_arg1, const double a_arg2)
{
    return a_arg1+a_arg2;
}

/**
 * @brief Overloaded subtraction of two doubles
 * @param a_arg1
 * @param a_arg2
 * @return a_arg1-a_arg2
 */
double MathExpression::subtract(const double a_arg1, const double a_arg2)
{
    return a_arg1-a_arg2;
}

/**
 * @brief Overloaded multiplication of two doubles
 * @param a_arg1
 * @param a_arg2
 * @return a_arg1*a_arg2
 */
double MathExpression::multiply(const double a_arg1, const double a_arg2)
{
    return a_arg1*a_arg2;
}

/**
 * @brief Overloaded division of two doubles
 * @param a_arg1
 * @param a_arg2
 * @return a_arg1/a_arg2
 */
double MathExpression::divide(const double a_arg1, const double a_arg2)
{
    return a_arg1/a_arg2;
}

/**
 * @brief Check if string is number
 * @param a_str String to test if it is number
 * @return **true** Is number
 * @return **false** Is not number
 */
bool MathExpression::isNumber(const string &a_str)
{
    // TODO: Check for numbers in exponential notation
    if (a_str.length()==0)
        return false;
    if((isdigit(a_str[0]))
            &&(a_str.length()==1))
        return true;
    if(((isdigit(a_str[0]))
        ||(a_str[0] == '+')
        ||(a_str[0] == '-')
        ||(a_str[0] == '.'))
            &&(a_str.length()>1))
        return true;
    return false;
}

/**
 * @brief Checks if the character exists in the vector of special chars
 * @param a_char
 * @return **true** Character is special
 * @return **false** Character doesn't exist in the vector of special chars
 */
bool MathExpression::isSpecialCharacter(const char &a_char)
{
    return find(specialChars.begin(), specialChars.end(), a_char)
            != specialChars.end();
}

/**
 * @brief isSpecialCharacter but exclude parenthesis
 * @param a_char
 * @return **true** Character is special
 * @return **false** Character doesn't exist in the vector of special chars
 */
bool MathExpression::isSpecialNoParenthesis(const char &a_char)
{
    if ((a_char == '(') || (a_char == ')'))
        return false;
    return find(specialChars.begin(), specialChars.end(), a_char)
            != specialChars.end();
}

/**
 * @brief Convert string to double using atof()
 * @param a_str
 * @return **double** Value of the conversion
 */
double MathExpression::convertStringToDouble(const string &a_str)
{
    return atof(a_str.data());
}

/**
 * @brief Returns the value of the function that operates on two args
 * @param a_arg1 First argument
 * @param a_arg2 Secont Argument
 * @param a_operatorKey Name of the operator or function
 * @return **double** Result of the function(a_arg1, a_arg2)
 */
double MathExpression::getResultdddOperator(
        double a_arg1,
        double a_arg2,
        const string &a_operatorKey)
{
    return operatorMap.find(a_operatorKey)->second.dddOperator(a_arg1, a_arg2);
}

/**
 * @brief Constructor, just a constructor list
 * @param a_name Name of the Entity
 * @param a_type EntityType of the Entity
 */
Entity::Entity(const string &a_name, const EntityType a_type):
    m_name(a_name),
    m_type(a_type)
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "Entity constructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Destructor
 */
Entity::~Entity()
{
    m_name.clear();
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "Entity destructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Returns the pointer to the Entity object
 * @details Used in child classes to access its parent Entity members.
 * @return *Entity Returns **this**
 */
const Entity *Entity::getEntity() const
{
    return this;
}

/**
 * @brief Get the EntityType of the object
 * @return EntityType
 */
EntityType Entity::entityType() const
{
    return m_type;
}

/**
 * @brief Constructor, with constructor list only
 * @param a_name Name key of the Operator, part of the Entity parent class
 * @param a_type Type of the Operator, part of the Entity parent class
 * @param a_precedence The OperatorPrecedence
 */
Operator::Operator(const string &a_name,
                   EntityType a_type,
                   OperatorPrecedence a_precedence,
                   double (*a_ddOperator)(const double),
                   double (*a_dddOperator)(const double, const double),
                   double (*a_ddiOperator)(const double, const int)):
    Entity(a_name, a_type),
    m_precedence(a_precedence),
    m_ddOperator(a_ddOperator),
    m_dddOperator(a_dddOperator),
    m_ddiOperator(a_ddiOperator)
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "Operator constructor called" << endl;
    cout.flush();
#endif
    // This prints some tests for the operators
#if DEBUG_MESSAGES_PSSMATHPARSER_FUNCPOINTERS == 1
    if(entityType() == EntityType::OperatorInDoubleDoubleOutDouble) {
        cout << "MathParser acquired function: " << m_name
             << " of the type: double func(double, double)" << endl;
        cout << "    test1: func(2,2) = " << m_dddOperator(2,2)
             << endl;
        cout << "    test1: func(0.5,2) = "
             << m_dddOperator(0.5,2) << endl;
        cout << "    test1: func(1.5,2.3) = "
             << m_dddOperator(1.5,2.3) << endl;
        cout << "    test1: func(3,1.5) = "
             << m_dddOperator(3,1.5) << endl;
    }
    if(entityType() == EntityType::OperatorInDoubleOutDouble) {
        cout << "MathParser acquired function: " << m_name
             << " of the type: double func(double)" << endl;
        cout << "    test1: func(2) = " << m_ddOperator(2) << endl;
        cout << "    test1: func(0.5) = " << m_ddOperator(0.5)
             << endl;
        cout << "    test1: func(1.5) = " << m_ddOperator(1.5)
             << endl;
        cout << "    test1: func(3.14125) = "
             << m_ddOperator(3.1415926535) << endl;
    }
    cout.flush();
#endif
}

/**
 * @brief Overload of the function call
 * @param a_arg1 First argument
 * @param a_arg2 Second argument
 * @return **double** Return value from function(a_arg1, a_arg2)
 */
double Operator::dddOperator(double a_arg1, double a_arg2) const
{
    return m_dddOperator(a_arg1, a_arg2);
}

/**
 * @brief Overload of the function call
 * @param a_arg1 First argument
 * @return **double** Return value from function(a_arg1)
 */
double Operator::ddOperator(double a_arg1) const
{
    return m_ddOperator(a_arg1);
}

/**
 * @brief Return the operator precedence as unsigned int
 *
 * @details We return the unsigned int in order to be able to compare two
 *          precedences.
 * @return **uint16_t** The cast of the OperatorPrecedence enum
 */
uint16_t Operator::precedence() const
{
    return static_cast<uint16_t>(m_precedence);
}

/**
 * @brief Constructor, only constructor list
 * @param a_name Name key of the Operator, part of the Entity parent class
 * @param a_type Type of the Operator, part of the Entity parent class
 * @param a_ivalue Integer value of the Argument
 * @param a_dvalue Double value of the Argument
 */
Argument::Argument(const string &a_name,
                   EntityType a_type,
                   int a_ivalue,
                   double a_dvalue):
    Entity(a_name, a_type),
    m_dvalue(a_dvalue),
    m_ivalue(a_ivalue)
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "Argument constructor called" << endl;
    cout.flush();
#endif
#if DEBUG_MESSAGES_PSSMATHPARSER_FUNCPOINTERS == 1
    if(entityType() == EntityType::ArgumentConstant) {
        cout << "MathParser has defined constant: " << m_name
             << " of value: " << m_dvalue << endl;
    }
    if(entityType() == EntityType::ArgumentUserConstant) {
        cout << "MathParser acquired user constant: " << m_name
             << " of value: " << m_dvalue << endl;
    }
    if(entityType() == EntityType::ArgumentVariable) {
        cout << "MathParser acquired variable: " << m_name << endl;
    }
    if(entityType() == EntityType::ArgumentGenerated) {
        cout << "MathParser generated value: " << m_name << endl;
    }
#endif
}

/**
 * @brief Setter of the double value
 * @param a_dvalue Value to be setted the m_dvalue to
 */
void Argument::setDoubleValue(const double a_dvalue)
{
    m_dvalue = a_dvalue;
}

/**
 * @brief Getter of the double value
 * @return **double** Return m_dvalue
 */
double Argument::getDoubleValue() const
{
    return m_dvalue;
}

/**
 * @brief Setter of the int value
 * @param a_ivalue Value to be setted the m_ivalue to
 */
void Argument::setIntValue(const int a_ivalue)
{
    m_ivalue = a_ivalue;
}

/**
 * @brief Getter of the int value
 * @return **int** Return m_ivalue;
 */
int Argument::getIntValue()
{
    return m_ivalue;
}

/**
 * @brief Constructor, constructor list only
 * @details Set the errors to zero
 */
MathExpression::MathExpression():
    m_reversePolishError(0),
    m_expressionError(0),
    m_mathPrintPrecision(7)
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "MathExpression constructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Destructor
 */
MathExpression::~MathExpression()
{
    clear();
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "MathExpression destructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Getter of the loaded expression
 * @return **string** The loaded infix m_expression of the object
 */
const string MathExpression::expression() const
{
    return m_expression;
}

/**
 * @brief Load the infix expression to the object
 * @details Clear the m_expression. Set m_expression and remove spaces and tabs.
 * @param a_expression String with the infix expression
 */
void MathExpression::setExpression(const string &a_expression)
{
    m_expression.clear();
    m_expression = a_expression;
    // Remove spaces and tabs from expression
    m_expression.erase(
                remove(m_expression.begin(), m_expression.end(), ' '),
                m_expression.end());
    m_expression.erase(
                remove(m_expression.begin(), m_expression.end(), '\t'),
                m_expression.end());
}

/**
 * @brief Does all the steps needed to produce a result
 * @details These are the steps that produce full expression that can give
 * result:
 *
 * ```c++
 * setExpression(a_expression);
 * expressionToReversePolish();
 * setArgumentMap();
 * expandMathExpression();
 * ```
 *
 * @param a_expression
 */
void MathExpression::setMath(const string &a_expression)
{
    setExpression(a_expression);
    expressionToReversePolish();
    setArgumentMap();
    expandMathExpression();
}

/**
 * @brief Getter of the expression in reverse Polish notation
 * @return m_reversePolish The string that is the reverse polish of the
 * m_expression
 */
const string MathExpression::reversePolish() const
{
    return m_reversePolish;
}

/**
 * @brief Setter of the Argument Map from user input of reverse polish
 * @details This function exist if you want to load the m_argumentMap from
 * polish notation from the user string input and not to be calculated from
 * the infix. This also sets the m_reversePolish to the given input.
 * @param a_reversePolish The reverse polish with space as a separator
 * @return **true** Everything was ok
 * @return **false** Some error
 */
bool MathExpression::setArgumentMap(const string &a_reversePolish)
{
    m_reversePolish.clear();
    m_reversePolish = a_reversePolish;
    return setArgumentMap();
}

/**
 * @brief Creates the Reverse polish notation of the expression
 * @details Start parsing the infix notation. On every full argument or
 * operator (entity) that is found, a call to the pushToReversePolish function
 * is performed to reorder the entities in a reverse polish notation. This
 * function can change the setted expression. One change is adding 0 in front
 * of signed variables thus making a sign into an operator. Other change is
 * adding parenthesis arround the ^ operator because it is right to left
 * operator (meaning 2^3^4 = 2^(3^4)). Main functionality of this algorithm
 * is to recognize separate entities from the infix notation. Then it sends
 * this data to the pushToReversePolish which only rearanges the entries by
 * preceedence and parenthesis.
 * @return **true** Successful creation of the reverse polish
 * @return **false** Some error occured
 */
bool MathExpression::expressionToReversePolish()
{
    m_RPstack.clear();
    string tmp;
    string alpha;
    string number;
    string special;
    bool readingAlpha = false;
    bool readingNumber = false;
    bool readingSpecial = false;
    bool addParenthesis = false;
    // Insert parenthesis in front of successive operators
    for (size_t i = 0; i < m_expression.length(); i++) {
        if(i>0) {
            // If -- or ++ convert to +
            if ((m_expression[i-1] == '-' && m_expression[i] == '-')
                    ||(m_expression[i-1] == '+' && m_expression[i] == '+'))  {
                m_expression[i] = '+';
                m_expression.erase(i-1,1);
                i--;
                continue;
            }
            // If -+ or +- convert to -
            if ((m_expression[i-1] == '-' && m_expression[i] == '+')
                    ||(m_expression[i-1] == '+' && m_expression[i] == '-'))  {\
                m_expression[i] = '-';
                m_expression.erase(i-1,1);
                i--;
                continue;
            }
            // Two operators without parenthesis -> add parenthesis
            if ((isSpecialNoParenthesis(m_expression[i-1])
                 && ( (m_expression[i] == '+' || m_expression[i] == '-')
                      && m_expression[i-1] != 'e') )
                 || (m_expression[i-1] == '^' && m_expression[i] != '(')
                 )
                addParenthesis = true;
        }

        // Surround with parenthesis
        if (addParenthesis == true) {
            m_expression.insert(m_expression.begin() + i, '(');
            size_t j;
            for (j=i+2; j < m_expression.length(); j++) {
                if (isSpecialCharacter(m_expression[j])) {
                    // Don' count for succesive ^ because is right to left op.
                    if(m_expression[i-1] == '^' && m_expression[j]=='^')
                        continue;
                    // Don't count for scientific notation
                    if(m_expression[j-1] == 'e'
                            && (m_expression[j]=='-' || m_expression[j]=='+'))
                        continue;
                    m_expression.insert(m_expression.begin() + j, ')');
                    addParenthesis = false;
                    break;
                }
            }
            // If it went to the end
            if (j==m_expression.length()) {
                m_expression.insert(m_expression.begin() + j, ')');
                addParenthesis = false;
                break;
            }
        }
    }
    for (size_t i = 0; i < m_expression.length(); i++) {
        tmp = m_expression[i];
        // If it is a letter
        if(isalpha(m_expression[i])
                && !(readingNumber && m_expression[i] == 'e')) {
            // Detect begining of an alpha
            if( !readingAlpha ) {
                // If it was reading a number
                if (readingNumber) {
                    pushToReversePolish(number, 'n');
                }
                // If it was reading a special
                else if (readingSpecial) {
                    pushToReversePolish(special, 's');
                }
                alpha.clear();
                readingAlpha = true;
                readingNumber = false;
                readingSpecial = false;
                alpha.push_back(m_expression[i]);
                // If it has some sign in front of it
                if (i>1) {
                    if ((m_expression[i-2] == '(' && m_expression[i-1] == '+')
                            ||(m_expression[i-2] == '('
                               && m_expression[i-1] == '-')) {
                        // Push zero to reverse polish
                        pushToReversePolish("0", 'n');
                    }
                }
            }
            // Already reading alpha just append
            else {
                alpha.push_back(m_expression[i]);
            }
        }
        // If it is a number or special char
        else {
            if (i>0) {
                // Detect begining of a number
                if ( !readingNumber
                     && ((m_expression[i-1] == '(' && m_expression[i] == '+')
                         || (m_expression[i-1] == '(' && m_expression[i] == '-')
                         || isdigit(m_expression[i])
                         || (!isdigit(m_expression[i-1])
                             && m_expression[i] == '.'))  ) {
                    // If it was reading a letter
                    if (readingAlpha) {
                        pushToReversePolish(alpha, 'a');
                    }
                    // If it was reading a special
                    else if (readingSpecial) {
                        pushToReversePolish(special, 's');
                    }
                    number.clear();
                    readingAlpha = false;
                    readingNumber = true;
                    readingSpecial = false;
                    number.push_back(m_expression[i]);
                }
                // Detect begining of a special (only one character)
                else if ( isSpecialCharacter(m_expression[i])
                          && !(readingNumber
                               && m_expression[i-1] == 'e'
                               && (m_expression[i] == '+'
                                   || m_expression[i] == '-'))) {
                    // If it was reading a letter
                    if (readingAlpha) {
                        pushToReversePolish(alpha, 'a');
                    }
                    // If it was reading a number
                    else if (readingNumber) {
                        pushToReversePolish(number, 'n');
                    }
                    // If it was reading a special
                    else if (readingSpecial) {
                        pushToReversePolish(special, 's');
                    }
                    special.clear();
                    readingAlpha = false;
                    readingNumber = false;
                    readingSpecial = true;
                    // If it has some sign in front of (
                    if (i>1) {
                        if ((m_expression[i-2] == '('
                             && m_expression[i-1] == '+'
                             && m_expression[i] == '(')
                                ||(m_expression[i-2] == '('
                                   && m_expression[i-1] == '-'
                                   && m_expression[i] == '(')) {
                            // Push zero to reverse polish
                            pushToReversePolish("0", 'n');
                        }
                    }
                    special.push_back(m_expression[i]);
                }
                // Already reading number just append
                else if ( readingNumber
                          && (isdigit(m_expression[i])
                              || (m_expression[i] == '.')
                              || (m_expression[i] == 'e')
                              || (m_expression[i] == '+')
                              || (m_expression[i] == '-')) ) {
                    number.push_back(m_expression[i]);
                }
            }
            // Here is for the first character i=0
            else {
                // If it starts with a sign, add zero on begining
                if (m_expression[i] == '+' || m_expression[i] == '-') {
                    number.clear();
                    special.clear();
                    readingAlpha = false;
                    readingNumber = false;
                    readingSpecial = true;
                    number.push_back('0');
                    special.push_back(m_expression[i]);
                    pushToReversePolish(number, 'n');
                }
                // Detect begining of a number
                else if ( (m_expression[i] == '.'
                           || isdigit(m_expression[i])) ) {
                    number.clear();
                    readingAlpha = false;
                    readingNumber = true;
                    readingSpecial = false;
                    number.push_back(m_expression[i]);
                }
                // Detect begining of a special
                else if ( m_expression[i] == '(' ) {
                    special.clear();
                    readingAlpha = false;
                    readingNumber = false;
                    readingSpecial = true;
                    special.push_back(m_expression[i]);
                }
                // Some error
                else {
                    m_expressionError = 5;
                    m_expressionErrorString = string(
                                "The expression has incorect begining");
                    return false;
                }
            }
        }
    }
    // Add the last entity
    if (readingAlpha) { // If it was reading a letter
        pushToReversePolish(alpha, 'A');
    }
    else if (readingNumber) { // If it was reading a number
        pushToReversePolish(number, 'N');
    }
    else if (readingSpecial) { // If it was reading a special
        pushToReversePolish(special, 'S');
    }
    return true;
}

/**
 * @brief Calculates the expression
 * @details This function calculates all the generators from the generator map
 * going successively which yields in the end the result of the calculation of
 * the whole expression.
 * @return **double** The result of the calculation, i.e. the value of the last
 * argument in the map
 */
double MathExpression::calculateExpression()
{
    double dvalue=0.0;
    if (m_generatorVec.size() == 0) {
        if (m_math.size() > 0)
            dvalue = m_argumentMap.at(m_math[m_math.size()-1]).getDoubleValue();
        else
            dvalue = 0;
    }
    else {
        for (vector <Generator>::iterator igen = m_generatorVec.begin();
             igen != m_generatorVec.end() - 1; ++igen) {
            igen->generateDoubleValue();
        }       
        dvalue = m_generatorVec.back().generateDoubleValue();
    }
    return dvalue;
}

/**
 * @brief Getter of m_reversePolishErrorNum
 * @return **uint32_t** The m_reversePolishError
 */
uint32_t MathExpression::reversePolishErrorNum()
{
    return m_reversePolishError;
}

/**
 * @brief Getter of m_reversePolishErrorString
 * @return **string** The m_reversePolishErrorString
 */
const string MathExpression::reversePolishErrorString()
{
    return m_reversePolishErrorString;
}

/**
 * @brief Getter of m_expressionErrorNum
 * @return **uint32_t** The m_expressionError
 */
uint32_t MathExpression::expressionErrorNum()
{
    return m_expressionError;
}

/**
 * @brief Getter of m_expressionErrorString
 * @return **string** The m_expressionErrorString
 */
const string MathExpression::expressionErrorString()
{
    return m_expressionErrorString;
}

/**
 * @brief Gets the vector m_math into string
 * @return **string** String of elements of m_math separated by coma
 */
const string MathExpression::mathToString() const
{
    string str;
    for (auto iter = m_math.begin(); iter!=m_math.end(); iter++) {
        str += (*iter);
        if(next(iter) != m_math.end()) {
            str += ", ";
        }
    }
    return str;
}

/**
 * @brief Gets the vector m_math into string, with explanation of elements
 * @return **string** String of elements of m_math separated by coma with
 * explanation
 */
const string MathExpression::mathToStringFull() const
{
    string str;
    ostringstream s;
    s.setf(ios::scientific);
    for (auto iter = m_math.begin(); iter!=m_math.end(); iter++) {
        str += (*iter);
        str += "(";
        if(entityType(*iter) == EntityType::ArgumentConstant) {
            str += "constant, ";
            s.str("");
            s << setprecision(m_mathPrintPrecision)
              << getConstantDoubleValue(*iter);
            str += s.str();
        }
        else if(entityType(*iter) == EntityType::ArgumentUserConstant) {
            str += "userConstant, ";
            s.str("");
            s << setprecision(m_mathPrintPrecision)
              << getArgumentDoubleValue(*iter);
            str += s.str();
        }
        else if(entityType(*iter) == EntityType::ArgumentGenerated
                || entityType(*iter) == EntityType::ArgumentGeneratedFromOneArg
                || entityType(*iter)
                == EntityType::ArgumentGeneratedFromTwoArg) {
            str += "generated, ";
            s.str("");
            s << setprecision(m_mathPrintPrecision)
              << getArgumentDoubleValue(*iter);
            str += s.str();
        }
        else if(entityType(*iter) == EntityType::Operator
                || entityType(*iter)
                == EntityType::OperatorInDoubleDoubleOutDouble
                || entityType(*iter)
                == EntityType::OperatorInDoubleIntOutDouble
                || entityType(*iter) == EntityType::OperatorInDoubleOutDouble
                || entityType(*iter) == EntityType::OperatorInIntOutInt) {
            str += "function";
        }
        else if(entityType(*iter) == EntityType::ArgumentVariable) {
            str += "variable, ";
            s.str("");
            s << setprecision(m_mathPrintPrecision)
              << getArgumentDoubleValue(*iter);
            str += s.str();
        }
        if(next(iter) == m_math.end()) {
            str += ") ";
        }
        else {
            str += "), ";
        }
    }
    return str;
}

/**
 * @brief Generates an Argument that is connected to a generator
 * @details Creates new argument and ads it to the m_argumentMap. Ads the
 * generator to he m_generattorMap. Ads entity to the m_math.
 * @param a_key1 Argument 1 name
 * @param a_key2 Argument 2 name
 * @param a_key3 Operator name
 * @return **true** All went ok
 * @return **false** Some error happened
 */
bool MathExpression::generateArgArgOp(const string &a_key1,
                                      const string &a_key2,
                                      const string &a_key3)
{    

    const Argument *arg1;
    if(entityType(a_key1) == EntityType::ArgumentConstant)
        arg1 = getConstant(a_key1);
    else
        arg1 = getArgument(a_key1);

    const Argument *arg2;
    if(entityType(a_key2) == EntityType::ArgumentConstant)
        arg2 = getConstant(a_key2);
    else
        arg2 = getArgument(a_key2);

    const Operator *op;
    op = getOperator(a_key3);

    // Construct new constants and push to maps
    string cstName = createNewUserConstantName();
    m_argumentMap.insert(
                pair<const string, Argument>(
                    cstName, { cstName,
                               EntityType::ArgumentGeneratedFromTwoArg,
                               0,
                               0.0}));

    m_generatorVec.push_back({ cstName,
                               EntityType::ArgumentGeneratedFromTwoArg,
                               op,
                               arg1,
                               arg2,
                               &(m_argumentMap.find(cstName)->second)});

    m_math.push_back(cstName);
    return true;
}

/**
 * @brief Generates an Argument that is connected to a generator
 * @details Creates new argument and ads it to the m_argumentMap. Ads the
 * generator to the m_generattorMap. Ads entity to the m_math.
 * @param a_key1 Argument name
 * @param a_key2 Operator name
 * @return **true** All went ok
 * @return **false** Some error happened
 */
bool MathExpression::generateArgOp(const string &a_key1,
                                   const string &a_key2)
{
    const Argument *arg1;
    if(entityType(a_key1) == EntityType::ArgumentConstant)
        arg1 = getConstant(a_key1);
    else
        arg1 = getArgument(a_key1);

    const Operator *op;
    op = getOperator(a_key2);

    // Construct new constants and push to maps
    string cstName = createNewUserConstantName();
    m_argumentMap.insert(
                pair<const string, Argument>(
                    cstName, { cstName,
                               EntityType::ArgumentGeneratedFromOneArg,
                               0,
                               0.0 }));

    m_generatorVec.push_back({ cstName,
                               EntityType::ArgumentGeneratedFromOneArg,
                               op,
                               arg1,
                               nullptr,
                               &(m_argumentMap.find(cstName)->second)});

    m_math.push_back(cstName);

    return true;
}

/**
 * @brief Expands the math expression with generated arguments
 * @details This function extracts the expression to calculation steps. It
 * expands the m_math expression to a one-step calculations arguments
 * (generated arguments) and unused operators. These arguments don't exist in
 * the original expression but represent in-between steps of calculation
 * (temporary varables). The extended expression ends with an argument which is
 * the result of the calculation. This way we form a structure that can later be
 * used as an arbitrary function for any user input variables.
 *
 * Example 1:
 * > infix expression: (10 + 2) - 3 + 5\n
 * > reverse polish: 10 2 + 3 - 5 +\n
 * > expanded math: 10 2 + 3 - 5 + a 3 - 5 + c 5 + d (where: a=10+2;
 * > c=a-3; d=c+5;)\n
 *
 * Example 2:
 * > infix expression: 162 / (2 + 1 ) ^4 \n
 * > reverse polish: 162 2 1 + 4 ^ / \n
 * > expanded math: 162 2 1 + 4 ^ / 162 a 4 ^ / 162 b / c (where: a=2+1; b=a^4;
 * > c=162/b;) \n
 *
 * @return **true** If successful expression extraction
 * @return **false** If some error happened. This sets the members
 * m_expressionError and m_expressionErrorString
 */

bool MathExpression::expandMathExpression()
{
    for (size_t i=0; i<m_math.size(); i++) {
        if (isOperator(m_math.at(i))) { // is (i) operator
            if (i>0) { // is i>0
                if (isArgument(m_math.at(i-1))  // is(i-1, i) of type (arg op)
                        && isOperator(m_math.at(i)) // and operates on one arg
                        && isOperatorOneArg(m_math.at(i))) {
                    // Do a backward loop from j=i-2 to a previous operator
                    // and push all args to m_math in natural order
                    if (i>1) {
                        int j=(int)i-2;
                        for ( ; j>=0; j--) {
                            if (isOperator(m_math.at(j))) {
                                break;
                            }
                        }
                        for (j++; j<=(int)i-2; j++) {
                            m_math.push_back(m_math.at(j));
                        }
                    }
                    // Generate new arg of type (arg op) and push new arg to
                    // m_math
                    generateArgOp(m_math.at(i-1), m_math.at(i));
                    continue;
                }
                else if (isOperator(m_math.at(i-1)) // is(i-1, i) of
                         && isOperator(m_math.at(i))) { // type (op op)
                    // Push operator to map
                    m_math.push_back(m_math.at(i));
                }
                else if (i>1) { // is i>1
                    if (isArgument(m_math.at(i-2))  // is(i-2, i-1, i) of type
                            && isArgument(m_math.at(i-1)) //  (arg arg op) and
                            && isOperator(m_math.at(i)) // operates on two args
                            && isOperatorTwoArg(m_math.at(i))) {
                        // Do a backward loop from j=i-3 to a previous operator
                        // and push all args to m_math in natural order
                        if (i>2) {
                            int j=(int)i-3;
                            for ( ; j>=0; j--) {
                                if (isOperator(m_math.at(j))) {
                                    break;
                                }
                            }
                            for (j++; j<=(int)i-3; j++) {
                                m_math.push_back(m_math.at(j));
                            }
                        }
                        // Generate new arg of type (arg arg op) and push new
                        // arg to m_math
                        generateArgArgOp(m_math.at(i-2),
                                         m_math.at(i-1),
                                         m_math.at(i));
                        continue;
                    }
                    else if (isOperator(m_math.at(i-2))  // is(i-2, i-1, i) of
                             && isArgument(m_math.at(i-1)) // type (op arg op)
                             && isOperator(m_math.at(i)) // and operates on two
                             && isOperatorTwoArg(m_math.at(i))) {
                        // Push (arg op) to math
                        m_math.push_back(m_math.at(i-1));
                        m_math.push_back(m_math.at(i));
                    }                   
                }
                else { // is i>1 :false
                    m_expressionError = 2;
                    m_expressionErrorString = string(
                                "The RP expression has operator without args");
                    return false;
                }
            }
            else { // is i>0 :false
                m_expressionError = 1;
                m_expressionErrorString = string(
                            "The RP expression starts with an operator");
                return false;
            }
        }
        else {
            continue;
        }
    }
    return true;
}

/**
 * @brief Clears all containters from data
 */
void MathExpression::clear()
{
    m_reversePolishErrorString.clear();
    m_expressionErrorString.clear();
    m_expression.clear();
    m_reversePolish.clear();
    m_argumentMap.clear();
    m_generatorVec.clear();
    m_math.clear();
    m_RPstack.clear();
}

/**
 * @brief Getter of the operatorMap number of elements (size)
 * @return **size_t** Unsigned int, the operatorMap size
 */
size_t MathExpression::operatorMapSize()
{
    return operatorMap.size();
}

/**
 * @brief Tests if the operatorMap has an operator with the given name
 * @param a_operatorName Name of the tested operator
 * @return **true** Exists operator with the given name
 * @return **false** Doesn't exist operator with the given name
 */
bool MathExpression::hasOperatorMap(const string &a_operatorName) const
{
    return (operatorMap.find(a_operatorName) != operatorMap.end());
}

/**
 * @brief Tests if the constantMap has a constant with the given name
 * @param a_constantName Name of the tested constant
 * @return **true** Exists constant with the given name
 * @return **false** Doesn't exist constant with the given name
 */
bool MathExpression::hasConstantMap(const string &a_constantName) const
{
    return (constantMap.find(a_constantName) != constantMap.end());
}

/**
 * @brief Tests if the m_argumentMap has an argument with the given name
 * @param a_argumentName Name of the tested argument
 * @return **true** Exists argument with the given name
 * @return **false** Doesn't exist argument with the given name
 */
bool MathExpression::hasArgumentMap(const string &a_argumentName) const
{
    return (m_argumentMap.find(a_argumentName) != m_argumentMap.end());
}

/**
 * @brief Tests if the m_generatorVec has a generator with the given name
 * @param a_generatorName Name of the tested generator
 * @return **true** Exists generator with the given name
 * @return **false** Doesn't exist generator with the given name
 */
bool MathExpression::hasGeneratorMap(const string &a_generatorName) const
{
    for(const Generator &igen : m_generatorVec) {
        if (igen.getName() == a_generatorName)
            return true;
    }
    return false;
}

/**
 * @brief Tests if the given name is an Operator
 * @details This is same as hasOperatorMap function, but is here for
 * completness with other functions.
 * @param a_entityName The name of the entity to be checked
 * @return **true** It is an operator
 * @return **false** It is not an operator
 */
bool MathExpression::isOperator(const string a_entityName) const
{
    return hasOperatorMap(a_entityName);
}

/**
 * @brief Tests if the given name is an Argument
 * @details Arguments are either in the m_argumentMap or in the constantMap.
 * We check if in one of these maps.
 * @param a_entityName The name of the entity to be checked
 * @return **true** It is an argument
 * @return **false** It is not an argument
 */
bool MathExpression::isArgument(const string a_entityName) const
{
    return (hasArgumentMap(a_entityName) || hasConstantMap(a_entityName));
}

/**
 * @brief Tests if the given name is an Generator
 * @details This is same as hasGeneratorMap function, but is here for
 * completness with other functions.
 * @param a_entityName The name of the entity to be checked
 * @return **true** It is a generator
 * @return **false** It is not a generator
 */
bool MathExpression::isGenerator(const string a_entityName) const
{
    return hasGeneratorMap(a_entityName);
}

/**
 * @brief Checks if the operator operates on one arg.
 * @param a_entityName The operator name
 * @return **true** Operates on one arg
 * @return **false** Doesn't operate on one arg
 */
bool MathExpression::isOperatorOneArg(const string a_entityName) const
{
    if(hasOperatorMap(a_entityName)) {
        if ( (getOperator(a_entityName)->entityType()
              == EntityType::OperatorInDoubleOutDouble)
             || (getOperator(a_entityName)->entityType()
                 == EntityType::OperatorInIntOutInt) )
            return true;
    }
    return false;
}

/**
 * @brief Checks if the operator operates on two args.
 * @param a_entityName The operator name
 * @return **true** Operates on two args
 * @return **false** Doesn't operate on two args
 */
bool MathExpression::isOperatorTwoArg(const string a_entityName) const
{
    if(hasOperatorMap(a_entityName)) {
        if ( (getOperator(a_entityName)->entityType()
              == EntityType::OperatorInDoubleDoubleOutDouble)
             || (getOperator(a_entityName)->entityType()
                 == EntityType::OperatorInDoubleIntOutDouble) )
            return true;
    }
    return false;
}

/**
 * @brief Gets the number of specific entities
 * @details This is used to count the Arguments for creating a new argument.
 * @param a_entityType This is one of the entities
 * @return **uint16_t** Number of entities
 */
uint16_t MathExpression::getEntitySize(EntityType a_entityType) const
{
    uint16_t count=0;
    // Count all arguments
    if (a_entityType == EntityType::Argument) {
        for (auto const &argmap : m_argumentMap) {
            if( argmap.second.entityType()
                    == EntityType::Argument
                    || argmap.second.entityType()
                    == EntityType::ArgumentGenerated
                    || argmap.second.entityType()
                    == EntityType::ArgumentGeneratedFromOneArg
                    || argmap.second.entityType()
                    == EntityType::ArgumentGeneratedFromTwoArg
                    || argmap.second.entityType()
                    == EntityType::ArgumentUserConstant )
                count++;
        }
    }    
    // Count only speciffic arguments
    else {
        for (auto const &argmap : m_argumentMap) {
            if(argmap.second.entityType() == a_entityType)
                count++;
        }
    }
    return count;
}

/**
 * @brief Getter of the EntityType
 * @param a_key Name of the entity
 * @return EntityType Return of the entityType()
 */
EntityType MathExpression::entityType(const string &a_key) const
{
    if(hasConstantMap(a_key))
        return getConstant(a_key)->entityType();
    if(hasOperatorMap(a_key))
        return getOperator(a_key)->entityType();
    if(hasArgumentMap(a_key)) {
        return getArgument(a_key)->entityType();
    }
    return EntityType::None;
}

/**
 * @brief Setter of the m_dvalue of Argument
 * @param a_key Name of the argument
 * @param a_value Value to set
 */
void MathExpression::setDoubleValueToArgument(const string &a_key,
                                              const double a_value)
{
    // Here we must be certain that this argument exists in the map
    Argument *arg = &(m_argumentMap.find(a_key)->second);
    arg->setDoubleValue(a_value);
}

/**
 * @brief Populates the m_argumentMap
 * @details Traverses the m_reversePolish notation and creates internally
 * defined variables, constants and user defined constants. This new names are
 * written in the map m_math.
 * @return **true** Everything was ok
 * @return **false** Some error
 */
bool MathExpression::setArgumentMap()
{
    // First clear containers
    m_math.clear();
    m_argumentMap.clear();
    m_generatorVec.clear();
    string entity;
    for(size_t i=0; i<m_reversePolish.size(); i++) {
        // If it is end of entity - start parsing
        if( (m_reversePolish[i]==' ')
                || (i==m_reversePolish.size()-1) ) {
            // If it is the end of line than there can be no space
            if ( (i==m_reversePolish.size()-1) &&  (m_reversePolish[i]!=' ') )
                entity.push_back(m_reversePolish[i]);
            // If first char is alpha, than it is a variable or operator
            if(isalpha(entity[0])) {
                // If it is not an operator or a defined constant -> than
                // it is a variable and must be added to the argument map
                if (hasOperatorMap(entity)) {
                    m_math.push_back(entity);
                }
                else if (hasConstantMap(entity)) {
                    if(hasArgumentMap(entity) == false) {
                        m_argumentMap.insert(
                            pair<const string, Argument>(
                            entity, { entity,
                            EntityType::ArgumentConstant,
                            0,
                            getConstantDoubleValue(entity)}));
                    }
                    m_math.push_back(entity);
                }
                else {
                    if(hasArgumentMap(entity) == false) {
                        m_argumentMap.insert(
                            pair<const string, Argument>(
                            entity, { entity,
                            EntityType::ArgumentVariable,
                            0,
                            0.0}));
                    }
                    m_math.push_back(entity);
                }
            }
            // If it is number than it is constant
            else if(isNumber(entity)) {
                // Construct constant name of the form #AA, #AB...
                string cstName = createNewUserConstantName();
                double cstDoubleValue = convertStringToDouble(entity);
                m_argumentMap.insert(
                            pair<const string, Argument>(
                                cstName, { cstName,
                                           EntityType::ArgumentUserConstant,
                                           0,
                                           cstDoubleValue}));
                entity = cstName;
                m_math.push_back(entity);
            }
            // If is some special character than it is operator
            else {
                if(hasOperatorMap(entity)) {
                    m_math.push_back(entity);
                }
                // Error: unknown operator
                else {
                    m_reversePolishError = uint32_t(i);
                    m_reversePolishErrorString =
                            string("The operator \'") + entity +
                            string("\' doesn't exist in the operator map");
                    return false;
                }
            }
            entity.clear();
        }
        else {
            entity.push_back(m_reversePolish[i]);
        }
    }
    return true;
}

/**
 * @brief Creates new names for internaly defined arguments
 * @details Counts how many arguments are there and creates one from the new
 * available name. Available names are of the form #AA, #AB, #AC, ..., #BA,
 * #BB, ...
 * @return **string** The new name
 */
const string MathExpression::createNewUserConstantName() const
{
    string cstName;
    cstName.push_back('#');
    cstName.push_back(char(int('A')
                           + getEntitySize(
                               EntityType::Argument
                               )/26));
    cstName.push_back(char(int('A')
                           + getEntitySize(
                               EntityType::Argument
                               )%26));
    return cstName;
}

/**
 * @brief Converts infix notation to Reverse Polish
 * @details  Uses the Shunting-yard algorithm to reorder infix notation coming
 * successively as it is written. The algorithm needs first to recognize the
 * type of the argument (variable, constant, operator...). Then it parses the
 * operator precednce and pushes to stack. The **m_RPstack** acts as containter
 * while the infix is being read. The RP notation is build in the
 * **m_reversePolish** as a string.
 *
 * The algorithm shorlty goes as follows:
 *
 * ```
 * if a_str is a number or variable then push it to the front of the output;
 * else if a_str is an operator, then:
 *     find the first '(' and make this position iend or if there is no '('
 *     make end of the stack position the iend;
 *     loop from i=0 to iend:
 *         if the i operator from the stack has greater than or equal
 *         precedence than a_str:
 *             pop from front of the operators stack onto the output queue;
 *             reset i and iend because of poping the operator stack;
 *     push to front the operator a_str;
 * else if a_str is '(' push to front of stack;
 * else if a_str is ')':
 *     pop from front of stack to output until the first '(';
 *     if there is no '(' then raise error;
 *
 * if it is the last entry from the infix push all stack entries to the output;
 * ```
 *
 * @param a_str Variable, constant, number or operator as a string
 * @param a_char Explains the a_str ('n'=number, 's'=special, 'a'=alpha,
 * 'N'=last number, 'S'=last special, 'A'=last alpha)
 * @return **true** Construction of RP is ok
 * @return **false** Construction of the RP went wrong
 *
 */
bool MathExpression::pushToReversePolish(
        const string &a_str,
        const char &a_char)
{
    // If it is a number, constant or variable push it to output
    if ((a_char == 'n' || a_char == 'N' || a_char == 'a' || a_char == 'A')
            && hasOperatorMap(a_str) == false ) {
        appendToReversePolishString(a_str);
    }
    else {
        // If it is an operator
        if(hasOperatorMap(a_str)) {
            int i;
            size_t iend;
            // Go to frst parenthesis occurence
            for(iend=0; iend<m_RPstack.size(); iend++) {
                if(m_RPstack[iend] == "(") break;
            }
            for(i=0; static_cast<size_t>(i)<iend; i++) {
                if(getOperator(m_RPstack[i])->precedence() >=
                        getOperator(a_str)->precedence()) {
                    appendToReversePolishString(m_RPstack[i]);
                    m_RPstack.erase(m_RPstack.begin() + i);
                    i--;
                    iend--;
                }
                else break;
            }
            // Push operators by precedence
            m_RPstack.insert(m_RPstack.begin()+i, a_str);
        }
        // If it is a ( add to begin of stack
        else if (a_str == "(") {
            m_RPstack.insert(m_RPstack.begin(), a_str);
        }
        // If it is a ) push stack to output until (
        else if (a_str == ")") {
            for(; m_RPstack[0] != "(";) {
                appendToReversePolishString(m_RPstack[0]);
                if (m_RPstack.size()>0) {
                    m_RPstack.erase(m_RPstack.begin());
                }
                else {
                    m_expressionError = 4;
                    m_expressionErrorString = string(
                                "The expression doesn't "
                                "have matching parenthesis");
                    return false;
                }
            }
            m_RPstack.erase(m_RPstack.begin());
        }
    }
    // If it is the last entry from the infix
    if (a_char == 'N' || a_char == 'A' || a_char == 'S') {
        for(size_t i=0; i<m_RPstack.size(); i++) {
            appendToReversePolishString(m_RPstack[i]);
        }
        m_RPstack.clear();
    }

    return true;
}

/**
 * @brief Appends elements to the Reverse Polish separated by space
 * @param a_str Entity to be apended
 */
void MathExpression::appendToReversePolishString(const string &a_str)
{
    if(m_reversePolish.size()>0)
        m_reversePolish += ' ';
    m_reversePolish += a_str;
}

/**
 * @brief Get the math precision for printing
 * @return **uint16_t** The precision when printing
 */
uint16_t MathExpression::getMathPrintPrecision() const
{
    return m_mathPrintPrecision;
}

/**
 * @brief Sets the value of the variable from the argument map
 * @param a_name Name of the variable
 * @param a_value Value of the variable (double)
 */
void MathExpression::setVariableDouble(const string &a_name,
                                       const double a_value)
{
    if (hasArgumentMap(a_name)) {
        m_argumentMap.find(a_name)->second.setDoubleValue(a_value);
    }
}

/**
 * @brief Return the number of variables in the argumentMap
 */
uint16_t MathExpression::getVariableSize() const
{
    return getEntitySize(EntityType::ArgumentVariable);
}

/**
 * @brief Sets the precission (digits after comma) when printing numbers
 * @param mathPrintPrecision Integer of precission default is 7
 */
void MathExpression::setMathPrintPrecision(const uint16_t &mathPrintPrecision)
{
    m_mathPrintPrecision = mathPrintPrecision;
}

/**
 * @brief Getter of the Argument m_dvalue
 * @param a_key Name of the argument
 * @return **double** Value of m_dvalue
 */
double MathExpression::getArgumentDoubleValue(const string &a_key) const
{
    Argument arg = m_argumentMap.find(a_key)->second;
    return arg.getDoubleValue();
}

/**
 * @brief Getter of the constant double value
 * @param a_key Name of the constant
 * @return **double** Value of the constant
 */
double MathExpression::getConstantDoubleValue(const string &a_key) const
{
    Argument arg = constantMap.find(a_key)->second;
    return arg.getDoubleValue();
}

/**
 * @brief Gets pointer to the operator
 * @param a_key Name of the operator
 * @return *Operator If the operator exists in operatorMap
 * @return **nullptr** If the operator doesn't exist
 */
const Operator *MathExpression::getOperator(const string &a_key) const
{
    if(hasOperatorMap(a_key))
        return &(operatorMap.find(a_key)->second);
    else
        return nullptr;
}

/**
 * @brief Gets pointer to the constant
 * @param a_key Name of the constant
 * @return *Argument If the constant exists in constantMap
 * @return **nullptr** If the constant doesn't exist
 */
const Argument *MathExpression::getConstant(const string &a_key) const
{
    if(hasConstantMap(a_key))
        return &(constantMap.find(a_key)->second);
    else
        return nullptr;
}

/**
 * @brief Gets pointer to the argument
 * @param a_key Name of the argument
 * @return *Argument If the argument exists in m_argumentMap
 * @return **nullptr** If the argument doesn't exist
 */
const Argument *MathExpression::getArgument(const string &a_key) const
{
    if(hasArgumentMap(a_key))
        return &(m_argumentMap.find(a_key)->second);
    else
        return nullptr;
}

/**
 * @brief Gets pointer to the generator
 * @param a_key Name of the generator
 * @return *Generator If the generator exists in m_generatorVec
 * @return **nullptr** If the generator doesn't exist
 */
Generator *MathExpression::getGenerator(const string &a_key)
{
    for(Generator &igen : m_generatorVec) {
        if(igen.getName() == a_key)
            return &igen;
    }
    return nullptr;
}

/**
 * @brief Constructor, only constructor list
 * @param a_name Name of the Generator
 * @param a_entityType EntityType of the Generator
 * @param *a_op Pointer to the Operator used in producing the value
 * @param *a_arg1 Pointer to the first Argument used in producing the value
 * @param *a_arg2 Pointer to the second Argument used in producing the value
 */
Generator::Generator( const string &a_name,
                      EntityType a_entityType,
                      const Operator *a_op,
                      const Argument *a_arg1,
                      const Argument *a_arg2,
                      Argument *a_myArg):
    Entity(a_name, a_entityType),
    m_op(a_op),
    m_arg1(a_arg1),
    m_arg2(a_arg2),
    m_myArg(a_myArg)
{
#if DEBUG_MESSAGES_PSSMATHPARSER == 1
    cout << "Generator constructor called" << endl;
    cout.flush();
#endif
}

/**
 * @brief Returns double value generated with the generator's operator
 * @return **double** Double value of the generator
 */
double Generator::generateDoubleValue() const
{
    double dvalue=0.0;
    if (entityType() == EntityType::ArgumentGeneratedFromOneArg) {
        dvalue = m_op->ddOperator(m_arg1->getDoubleValue());
    }
    else if (entityType() == EntityType::ArgumentGeneratedFromTwoArg) {
        dvalue = m_op->dddOperator(m_arg1->getDoubleValue(),
                                   m_arg2->getDoubleValue());
    }

    m_myArg->setDoubleValue(dvalue);

    return dvalue;
}

/**
 * @brief Getter of the generator name
 * @details This is a name that also exists in the m_argumentMap. This name
 * connects the generator and the argument which value is generated.
 * @return **string** Name of the generator
 */
const string &Generator::getName() const
{
    return m_name;
}
