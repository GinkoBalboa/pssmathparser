# PssMathParser

[![GitHub license](https://img.shields.io/github/license/GinkoBalboa/pssmathparser.svg)](https://github.com/GinkoBalboa/pssmathparser/blob/master/LICENSE)
[![Github size](https://img.shields.io/github/repo-size/GinkoBalboa/pssmathparser.svg)](https://github.com/GinkoBalboa/pssmathparser)
[![GitHub issues](https://img.shields.io/github/issues/GinkoBalboa/pssmathparser.svg)](https://github.com/GinkoBalboa/pssmathparser/issues)
[![GitHub forks](https://img.shields.io/github/forks/GinkoBalboa/pssmathparser.svg)](https://github.com/GinkoBalboa/pssmathparser/network)
[![GitHub stars](https://img.shields.io/github/stars/GinkoBalboa/pssmathparser.svg)](https://github.com/GinkoBalboa/pssmathparser/stargazers)

Library written in C++ for parsing mathematical expressions and creating
expression maps that can be used as an arbitrary functions in runtime. The
syntax of the expression is in C and the parsing is done by transforming the
given infix expression to Reverse Polish (RP) notation. The RP is then further
parsed to yield a map of arguments and operators that can be reused. The RP map
can be used as an arbitrary function by setting the user defined variables in
the expression and calling for the calculation of the result.

## Contributing

One of the purposes of this repository is to continue to develop this library.
Removing bugs, making it faster and easier to use. Development happens in
the open on GitHub, and I'm grateful to the community for contributing
bug-fixes and improvements.

## Class architecture and working

The architecture of the class is build around the single export class
MathParser. This class is virtual class that returns a pointer to the created
object inside the library space. This way we keep all the alocated memory
inside the library heap and the functionality is provided by a reference to
the created object.

### Objective

The main objective of this library is to have the following functionality:

1. Load an expression as a string:
```c++
MathParser *mp = MathParser::makeMathParser();
mp->setMath("2 * sin( 2*pi*f*t + phi )");
```

2. Use the generated object as a function with arguments in runtime:
```c++
mp->setVariableDouble("phi", 0.785398);
double result = mp->calculateExpression();
```

### Parsing

We parse the expression and resolve the elements to different types of
entities. Then we combine the entities in a RP notation. We run through the RP
notation and generate intermediary steps of the calculation as separate
arguments (generated variables).

For example, if we parse the infix expression :

```c++
a*pow(5*b, pi/(2 + 1)^4)
```

The following RP notation is generated:
```c++
a 5 b * pi 2 1 + 4 ^ / pow *
```
Which in the code is written as:
```c++
#AA #AB #AC * pi #AD #AE + #AF ^ / pow *
```

After expanding the RP expression to intermediate steps we get the following
expression:
```c++
#AA #AB #AC * pi #AD #AE + #AF ^ / pow * #AA #AG pi #AH #AF ^ / pow * #AA #AG pi #AI / pow * #AA #AG #AJ pow * #AA #AK * #AL
```

Arguments:

|user variable | user constant | generated variable            |
|:-------------|:--------------|:------------------------------|
|#AA = a       |               |                               |
|              |#AB = 5        |                               |
|#AC = b       |               |                               |
|              |#AD = 2        |                               |
|              |#AE = 1        |                               |
|              |#AF = 4        |                               |
|              |               | #AG = 5*b                     |
|              |               | #AH = 2+1                     |
|              |               | #AI = (2+1)^4                 |
|              |               | #AJ = pi/(2+1)^4              |
|              |               | #AK = pow(5*b, pi/(2+1)^4)    |
|              |               | #AL = a\*pow(5\*b, pi/(2+1)^4)|

The expanded expression is not meant to be calculated, instead calculations are
performed only on the generated variables. In order to get fast calling we
store the generated variables in separate map. Upon request for the result we
calculate generated variables successively from #AG to #AL. In the end #AL is
the result of our calculation. By setting the user variables prior to calling
the result this train of calculations gives the result of the function
operating on those user variables.

### Entities

We recognize three types of entities:
* **Argument** - anything that can have a value. This can be a predefined
constant in the system, a number from the expression (user constant), a variable
or a generated variable (a temporal variable generated as a stack for
calculation steps).
* **Operator** - functions defined in the system at compile time. Names of
operators are reserved words. These are functions defined for example in the
math.h library like **sin**, **pow** or static functions that are defined in
the MathParser.
* **Generator** - combination of operators and arguments used in mid-steps of
calculating the result. Names of generator coincide with some names of arguments
from the argument map, this connects the argument and generator. Arguments
connected to generators are generated variables.

## Examples of use

First you must include the header in the .c file where you want to use the
MathParser.

You can create parsed expression by directly writing the expression as a string.

```c++
#include "pssmathparser.h"
// Some code
using namespace PssMathParser;
// Some code
int main()
{
  MathParser *mp = MathParser::makeMathParser();
  mp->setMath("2 * sin( 2*pi*f*t + phi )");
  mp->setVariableDouble("phi", 0.785398);
  double result = mp->calculateExpression();
}
```

Or if you have some code that parses user input (or files) you can load it in
a variable and use the result.

```c++
string exStr;
string arg1Str;
double arg1Val;
// Load the expression (function) in the variable (exStr)
// Load the function arguments (arg1Str)
// Get the argument values (arg1Val)
// Calculate the function in runtime
mp->setMath(line);
mp->setVariableDouble(arg1Str, arg1Val);
double result = mp->calculateExpression();
```

You can get the number of variables (arguments):

```c++
uint16_t numVariables = mp->getVariableSize();
```

## Folder structure

```
  |
  +--- bin
  +--- docs
  +--- lib
  |    +--- so
  |    |    +--- valgrind
  |    |    +--- release
  |    |    +--- debug
  |    +--- dll
  |         +--- release
  |         +--- debug
  +--- src
  +--- test
       +--- Makefile
       +--- test1.pro
       +--- test2.pro
       +--- ...
  +--- pssmathparser.pro
  +--- Makefile
```

* In the root folder are the Qt project file `pssmathparser.pro` and the
  Linux `Makefile`.
* `bin` The folder for binaries, this is where the compiled tests are build.
* `docs` The documentation files generated with Doxygen and the Doxygen config
  itself.
* `lib` Library files (`lib\so` for Linux and `lib\dll` for Windows). Further
  down in this folders under `release` and `debug` are shared libs compiled from
  the Qt. The `valgrind` folder is the .so file compiled with `-O0` flag in
  order to have the valgrind work corectly.
* `src` Folder for the `.cpp` and `.h` files for the math parser. The include
  header is located here.
* `test` Folder with `testx.cpp` and coresponding `inputx.txt` files for
  testing. Here is also the `Makefile` for the tests and the Qt project files
  for the tests.

## Building from sources

### Qt or Makefile

You can use the .pro file and open and build the application under Qt. This
only uses the QtCreator, it doesn't use any of the Qt libraries. Or you can
use supplied Makefile and run it directly from Linux console.

### Windows

You can run from your QtCreator as described.

### Linux

In Linux you can buld everything directly from your console. No third party
software is needed.

#### Building the libraries

Go to the pssmathparser folder and run:

```bash
user@pc:path/to/pssmathparser$ make
```

This command will run the Makefile in the same directory. Makefile cleans the
lib folder and then builds the **libpssmathparser.so.x.x.x** in the lib folder.
Then it creates the symbolic links from **libpssmathparser.so** to the library
file.

If you wish to run test for valgrind than you need to compile the lib for a
valgrind (without the `-O3` optimization). This is done by running:

```bash
user@pc:path/to/pssmathparser$ make valgrind
```

#### Building the tests

The library is now ready to use.

You can make all the tests by running:

```bash
user@pc:path/to/pssmathparser$ make test
```

To run tests, cd into test dir. To run test1:

```bash
user@pc:path/to/pssmathparser/test$ ./test1
```

## Using compiled libraries

If you only want to use the library without modifying it, download the binaries
(.dll for Windows or .so for Linux) and the header files. Include the header in
your project and point the linker where the library is. After this you can use
it as described.

To see how you can link to the pssmathparser library check the tests. There is
a Makefile that includes the library:

```bash
LDFLAGS       = -Wl,-R $(PWD)/../lib/ -L$(PWD)/../lib/
LIBS          = -lpssmathparser
INCLUDES      = -I$(PWD)/../src/
# Some code ...
all:
$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)
```

Here we pass the Library Flags (LDFLAGS) to point where the library is located.
In our case the library binaries are under folder above in the lib folder.
We pass the name of the library to the linker (LIBS). We also must tell the
compiler where the header "pssmathparser.h" file is located (INCLUDES).

Then we include the header in the .c file where we use the MathParser.

```c++
#include "pssmathparser.h"
// Some code
using namespace PssMathParser;
// Some code
int main()
{
  MathParser *mp = MathParser::makeMathParser();
  mp->setMath("2 * sin( 2*pi*f*t + phi )");
  mp->setVariableDouble("phi", 0.785398);
  double result = mp->calculateExpression();
}
```

## Testing and metrics

Code used for testing is located under the folder `test`.

### Compiling and running tests

Enter the test folder and type

```bash
user@pc:path/to/pssmathparser/test$ make
```

to compile all the tests or just

```bash
user@pc:path/to/pssmathparser/test$ make test1
```

to compile `test1`.
The program gets compiled and the binary is build in the bin folder as `test1`.
Run the test from your bash:

```bash
user@pc:path/to/pssmathparser/bin$ ./test1
```

The test uses the file `test/test1.txt` to run test inputs. And prints on the
end if all tests passed or not.

### Types of tests

There are couple of tests testing each functionality. The test No. 4 is for
testing the speed of the calculating the parsed functions vs calling the
hardcoded function.

I've measured good speed (between 1 to 3 times slower than the hardcoded
function). This is from library compiled with `-03` optimization.

```c++
  func1 = Io*(exp(qe*V/(kBJ*(ToK+TC)))-1);
  func2 = Io*(exp(qe*V/(kBJ*(ToK+TC)))-1)+Io*(exp(qe*V^(2.5)/(kBJ*(ToK+TC)))-2);
  func3 = cos(2*pi*3*t)*exp(-pi*t^(2));
```

| Function  | System (compiler)           | Parsed execution time [s] | Hardcoded execution time [s] | Parsed/Hardcoded execution time |
| --------- | --------------------------- | ------------------------- | ---------------------------- | ------------------------------- |
| func1     | Win10 x64 (MSVC2013)        | 4.8e-08                   | 1.6e-08                      | 3                               |
| func1     | Ubuntu16.04 x64 (GCC 5.4.0) | 1.704e-07                 | 9.566e-08                    | 1.781                           |
| func2     | Win10 x64 (MSVC2013)        | 1.24e-7                   | 9.3e-8                       | 1.333                           |
| func2     | Ubuntu16.04 x64 (GCC 5.4.0) | 3.442e-07                 | 2.238e-07                    | 1.538                           |
| func3     | Win10 x64 (MSVC2013)        | 1.24e-7                   | 4.6e-8                       | 2.696                           |
| func3     | Ubuntu16.04 x64 (GCC 5.4.0) | 1.759e-07                 | 1.639e-07                    | 1.073                           |

## License

The PssMathParser library is released under the GNU General Public License


## Version info

*Note*:

Version number should be set at the following places:
  1. `Makefile` from the root folder.
  2. `pssmathparser.pro` from the root folder.
  3. In `Doxyfile` from the `docs` folder, set the variable `PROJECT_NUMBER`

* **1.0.0** : Minimum set of functions and constants.
