/**
 *  @file pssmathparser.h
 *  @brief Headers for the PssMathParser
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

#ifndef PSSMATHPARSER_H
#define PSSMATHPARSER_H

#include "pssmathparser_global.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @brief Library for parsing math expression strings
 * @details This library basicaly does what every other parser does: turn
 * infix notation to Reverse Polish and than calculate the result. The
 * additional feature is that this library creates a structure (map) of the
 * Reverse Polish so that it can be later used as a function. The call to the
 * function is performed by setting the variables to a prior constructed math
 * expression object. The goal is to have a fast calculation as close as
 * possible to the speed of a hardcoded function.
 */
namespace PssMathParser {

using namespace std;

/**
 * @brief Enum defines all the types of functions for the operator
 */
enum class EntityType {
    None,
    Operator,
    OperatorInDoubleOutDouble,
    OperatorInIntOutInt,
    OperatorInDoubleDoubleOutDouble,
    OperatorInDoubleIntOutDouble,
    Argument,
    ArgumentConstant,
    ArgumentUserConstant,
    ArgumentVariable,
    ArgumentGenerated,
    ArgumentGeneratedFromOneArg,
    ArgumentGeneratedFromTwoArg
};

/**
 * @brief Enum defines precedence of operators
 */
enum class OperatorPrecedence {
    Function = 9,
    Multiplication = 6,
    Addition = 3
};

/**
 * @brief Base class for math expression entities
 * @details This class can be Operator, Argument or Generator. Array of these
 * entities represents the reverse polish notation of the mathemathical
 * expression and the succesive steps in producing the final result.
 */
class Entity {

public:
    Entity(const string &a_name,
           const EntityType a_type = EntityType::None);
    ~Entity();

    const Entity *getEntity() const;
    EntityType entityType() const;    

protected:
    string  m_name; /**< Name of func or arg */
    EntityType m_type; /**< Type of Entity, from enum class */
};

/**
 * @brief Derived from Entity, used for storing pointers to math functions
 * @details This class has pointer to functions that are standard math.h
 * functions. These are called from the MathExpression during parsing. Several
 * types are posible of these class.
 * @note Example:
 * > EntityType::OperatorInIntOutInt is a function of the type
 * > **int function(int a_value)**
 */
class Operator : public Entity {

public:
    Operator(const string &a_name,
             EntityType a_type = EntityType::Operator,
             OperatorPrecedence m_precedence = OperatorPrecedence::Function,
             double (*a_ddOperator)(const double) = nullptr,
             double (*a_dddOperator)(const double, const double) = nullptr,
             double (*a_ddiOperator)(const double, const int) = nullptr);

    double dddOperator(double a_arg1, double a_arg2) const;
    double ddOperator(double a_arg1) const;
    uint16_t precedence() const;
private:
    OperatorPrecedence m_precedence;  /**< Higher number operates first */
    double (*m_ddOperator)(const double); /**< Pointer to func */
    double (*m_dddOperator)(const double, const double); /**< Pointer to func */
    double (*m_ddiOperator)(const double, const int); /**< Pointer to func */
};

/**
 * @brief Derived from Entity, used for all that is not Operator.
 * @details This class can represent variables, constants or plain numbers
 * (here refered to as **userConstants**).
 */
class Argument : public Entity {
public:
    Argument(const string &a_name,
             EntityType a_type = EntityType::Argument,
             int a_ivalue = 0,
             double a_dvalue = 0);

    void setDoubleValue(const double a_dvalue);
    double getDoubleValue() const;
    void setIntValue(const int a_ivalue);
    int getIntValue();
private:
    double m_dvalue; /**< Double value of the argument */
    int m_ivalue; /**< Integer value of the argument */
};

/**
 * @brief Derived from Entity, used for intermediate steps of calcualtion
 * @details This class is connected to argument, its objects are not defined by
 * the user but generated as intermediate steps (arguments) that are used for
 * generating the value of other arguments. This class has pointers to the
 * arguments and the operator wich is used for generating its value. The
 * Generator class doesn't store a value because it supose to produce the value
 * upon call. The connection between the Argument and the Generator that
 * generates it is through its names.
 */
class Generator : public Entity {
public:
    Generator(const string &a_name,
              EntityType a_entityType,
              const Operator *a_op = nullptr,
              const Argument *a_arg1 = nullptr,
              const Argument *a_arg2 = nullptr,
               Argument *a_myArg = nullptr);

    double generateDoubleValue() const;
    const string &getName() const;

private:
    const Operator *m_op; /**< Points to the operator of the genrator */
    const Argument *m_arg1; /**< Points to the first argument */
    const Argument *m_arg2; /**< Points to the second argument */
    Argument *m_myArg; /**< Points to the generated argument */
};

/**
 * @brief The export point of this library.
 * @details Class is used to be implemented by other class internal to the
 * mathparser lib. Only this class has the export point to be used in other
 * programs.
 */
class PSSMATHPARSER_EXPORT_PUBLIC MathParser
{
public:

    virtual ~MathParser() = 0;

    static MathParser *makeMathParser();

    virtual const string expression() const = 0;
    virtual void setExpression(const string &a_expression) = 0;
    virtual void setMath(const string &a_expression) = 0;
    virtual const string reversePolish() const = 0;
    virtual bool setArgumentMap(const string &a_reversePolish) = 0;
    virtual bool setArgumentMap() = 0;
    virtual bool expressionToReversePolish() = 0;
    virtual const string mathToString() const = 0;
    virtual const string mathToStringFull() const = 0;
    virtual void setVariableDouble(const string &a_name,
                                   const double a_value) = 0;
    virtual bool expandMathExpression() = 0;
    virtual double calculateExpression() = 0;
    virtual void setMathPrintPrecision(const uint16_t &mathPrintPrecision) = 0;
    virtual uint16_t getMathPrintPrecision() const = 0;
    virtual uint16_t getVariableSize() const = 0;
    virtual void clear() = 0;
};


/**
 * @brief Main functionality of this library.
 * @details Everything from parsing expressions, converting to Reverse Polish
 * notation and contruction of maps of generator for fast calculation is done
 * here.
 */
class MathExpression : public MathParser
{
public:

    MathExpression();
    ~MathExpression();

    static vector<char> specialChars; /**< Special characters */
    static unordered_map<string, Operator> operatorMap; /**< Defined operators */
    static unordered_map<string, Argument> constantMap; /**< Defined constants */

    static double add(const double a_arg1, const double a_arg2);
    static double subtract(const double a_arg1, const double a_arg2);
    static double multiply(const double a_arg1, const double a_arg2);
    static double divide(const double a_arg1, const double a_arg2);
    static bool isNumber(const string &a_str);
    static bool isSpecialCharacter(const char &a_char);
    static bool isSpecialNoParenthesis(const char &a_char);
    static double convertStringToDouble(const string &a_str);
    static double getResultdddOperator(
            double a_arg1,
            double a_arg2,
            const string &a_operatorKey);

    const string expression() const;
    void setExpression(const string &a_expression);

    void setMath(const string &a_expression);

    const string reversePolish() const;
    bool setArgumentMap(const string &a_reversePolish);
    bool setArgumentMap();

    bool expressionToReversePolish();

    double calculateExpression();

    uint32_t reversePolishErrorNum();
    const string reversePolishErrorString();

    uint32_t expressionErrorNum();
    const string expressionErrorString();

    const string mathToString() const;
    const string mathToStringFull() const;

    bool expandMathExpression();

    void clear();

    void setMathPrintPrecision(const uint16_t &mathPrintPrecision);
    uint16_t getMathPrintPrecision() const;

    void setVariableDouble(const string &a_name, const double a_value);

    uint16_t getVariableSize() const;

private:
    size_t operatorMapSize();
    bool hasOperatorMap(const string &a_operatorName) const;
    bool hasConstantMap(const string &a_constantName) const;
    bool hasArgumentMap(const string &a_argumentName) const;
    bool hasGeneratorMap(const string &a_generatorName) const;
    bool isOperator(const string a_entityName) const;
    bool isArgument(const string a_entityName) const;
    bool isGenerator(const string a_entityName) const;
    bool isOperatorOneArg(const string a_entityName) const;
    bool isOperatorTwoArg(const string a_entityName) const;
    bool generateArgArgOp(const string &a_key1,
                                const string &a_key2,
                                const string &a_key3);
    bool generateArgOp(const string &a_key1,
                             const string &a_key2);
    double getArgumentDoubleValue(const string &a_key) const;
    double getConstantDoubleValue(const string &a_key) const;
    const Argument *getConstant(const string &a_key) const;
    const Operator *getOperator(const string &a_key) const;
    Generator *getGenerator(const string &a_key);
    const Argument *getArgument(const string &a_key) const;
    uint16_t getEntitySize(EntityType a_entityType) const;
    EntityType entityType(const string &a_key) const;
    void setDoubleValueToArgument(const string &a_key, const double a_value);
    const string createNewUserConstantName() const;
    bool pushToReversePolish(const string &a_str, const char &a_char);
    void appendToReversePolishString(const string &a_str);

    uint32_t m_reversePolishError; /**< Error num in the RP notation */
    string m_reversePolishErrorString; /**< Error string in the RP notation */
    uint32_t m_expressionError; /**< Error num in the infix notation */
    string m_expressionErrorString; /**< Error string in the infix notation */
    string m_expression; /**< The infix notation */
    string m_reversePolish; /**< The RP notation */
    uint16_t m_mathPrintPrecision; /**< Precision when printing numbers */
    unordered_map<string, Argument> m_argumentMap; /**< Map of Arguments */
    vector<Generator> m_generatorVec;/**< Vector of generators */
    vector<string> m_math; /**< The expression in entities */
    vector<string> m_RPstack; /**< Stack for the Shunting-yard algorithm */
};

}

#endif // PSSMATHPARSER_H
