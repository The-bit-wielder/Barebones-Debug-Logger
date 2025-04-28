#ifndef BITS_DEBUG_HPP // Header guard to prevent multiple inclusions of the header file
#define BITS_DEBUG_HPP

#if _DEBUG    // Check for Debug mode
#define DEBUG // Define Debug enable;
#endif

// Include standard libraries
#include <iostream>
#include <string>
#include <vector>

namespace BDL // Namespace for the library
{

#ifdef DEBUG

std::string debugString;        // String for storing Debug errors
std::string loopBuffer = "";    // String for storing loop errors
bool debugErrorLoop = false;    // Flag for error thrown in while loops
bool debugErrorThrown = false;  // Flag for error thrown in while loops
bool debugErrorDisplay = false; // Flag for error displayed

void appendToGlobalString(std::string errorMessage) // Debug buffer function for storing Debug errors (don't use it)
{
    debugString += errorMessage + "\n"; // Append the string to the Debug string and add a new line
}
#endif // debug

// Global functions
void linearDebugMessage(std::string errorMessage, bool isFatal) // Debug for linear instructions
{
#ifdef DEBUG
    debugErrorLoop = true; // Set the error loop flag to true
    if (isFatal == true)   // Check if error its fatal
    {
        appendToGlobalString("[Fatal]" + errorMessage); // Call the Debug buffer function
        std::cerr << "Caught fatal error. List of all caught errors:\n" << debugString; // Print the fatal error message
        std::exit(1); // Exit the program with return value 1
    }
    else
    {
        appendToGlobalString("[Warn]" + errorMessage); // Call the Debug buffer function
    }
    if (loopBuffer != "")
    {
        appendToGlobalString("[Loop]" + loopBuffer); // Call the Debug buffer function
        loopBuffer = "";                             // Clear the loop buffer
    }
#endif // debug
}
void loopDebugMessage(std::string errorMessage, bool isFatal) // Debug for loop instructions
{
#ifdef DEBUG
    if (isFatal)                                   // Check if error its fatal
        linearDebugMessage(errorMessage, isFatal); // Call the linear Debug message function to terminate the program
    if (loopBuffer.find(errorMessage) != std::string::npos) // Check if the error message is already in the buffer
    {
        // I don't know what to put here yet
    }
    else
    {
        loopBuffer += "[Loop]" + errorMessage + "\n"; // Append the error message to the buffer
    }
#endif // debug
}
void debugDisplayOutput()
{
#ifdef DEBUG
    if (loopBuffer != "") // Check if the loop buffer is not empty
        appendToGlobalString(loopBuffer); // Call the Debug buffer function
    if (debugErrorDisplay == true) // Check if end of Debug was called
    {
        std::cerr << "List of all new errors since last display call:\n" << debugString; // Print the end error message
        debugString = "";                                                                // Clear the Debug string
    }
    else
    {
        debugErrorDisplay = true; // Set the Debug display flag to true
        std::cerr << "Debug display called. List of all caught errors:\n"
                  << debugString; // Print the Debug errors message
        debugString = "";         // Clear the Debug string
    }

#endif // debug
}
} // namespace bits

#endif // BITS_DEBUG_HPP
