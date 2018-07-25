/**
 *  @file pssmathparser_global.h
 *  @brief Global headers for the PssMathParser
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

#ifndef PSSMATHPARSER_GLOBAL_H
#define PSSMATHPARSER_GLOBAL_H

#define DEBUG_MESSAGES_PSSMATHPARSER 0 /**< Print debug messages */
#define DEBUG_MESSAGES_PSSMATHPARSER_FUNCPOINTERS 0 /**< Print debug messages */

// Versioning
#ifndef VERSION
    #define VERSION 0.0.0 /**< Version number as major.minor.micro */
#endif

// These are defined in math.h
#define _USE_MATH_DEFINES

// These is the dll spec for WIN and LINUX
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define PSSMATHPARSER_EXPORT_PUBLIC __attribute__ ((dllexport))
    #else // Note: actually gcc seems to also supports this syntax.
      #define PSSMATHPARSER_EXPORT_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define PSSMATHPARSER_EXPORT_PUBLIC __attribute__ ((dllimport))
    #else // Note: actually gcc seems to also supports this syntax.
      #define PSSMATHPARSER_EXPORT_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define PSSMATHPARSER_EXPORT_LOCAL
#else
  #if __GNUC__ >= 4
    #define PSSMATHPARSER_EXPORT_PUBLIC __attribute__ ((visibility ("default")))
    #define PSSMATHPARSER_EXPORT_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define PSSMATHPARSER_EXPORT_PUBLIC
    #define PSSMATHPARSER_EXPORT_LOCAL
  #endif
#endif

#endif // PSSMATHPARSER_GLOBAL_H
