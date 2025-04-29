#ifndef BITS_DEBUG_HPP // Header guard to prevent multiple inclusions of the header file
#define BITS_DEBUG_HPP
#define BDL            // Define the BDL macro to indicate that the library is being used

// Include standard libraries
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

namespace BDL // Namespace for the library
{


std::string debugString;        // String for storing Debug errors
std::string loopBuffer = "";    // String for storing loop errors
std::string logName = "latest.log";       // String for storing the log file name
bool debugErrorDisplay = false; // Flag for error displayed
bool autoOutputDebug = false;        // Flag for auto display
bool fileOutputDebug = false;        // Flag for file output
u_int16_t debugErrorCount = 0;    // Counter for Debug errors
u_int16_t debugErrorCountMax = (1 << (sizeof(debugErrorCount) * 8)) - 1; // Set the default max number of debug errors to store

void setParams(bool consoleOut , bool fileOut,std::string logFileName, bool autoOutput,u_int16_t autoOutputInterval) // Function to set parameters for Debug output
{
    // Set the parameters for Debug output
    if (consoleOut == true) // Check if console output is enabled
    {
        linearDebugMessage << "Console output enabled.\n"; // Print message to console
    }
    if (fileOut == true) // Check if file output is enabled
    {
        logName=logFileName; // Set the log file name
        std::ofstream file(logName); // Open file for writing
    }
    if (autoOutput) // Check if auto output is enabled
    {
        linearDebugMessage << "Auto output enabled. Interval: " << autoOutputInterval << "\n"; // Print message to console
        autoOutputDebug = true; // Set the auto display flag to true
    }
}

void appendToGlobalString(std::string errorMessage) // Debug buffer function for storing Debug errors (don't use it)
{
    debugErrorCount++; // Increment the Debug error counter
    debugString += errorMessage + "\n"; // Append the string to the Debug string and add a new line
    if (debugErrorCount > debugErrorCountMax && autoOutputDebug) // Check if the Debug error counter exceeds the error count limit
    {

        debugErrorCount = 0; // Reset the Debug error counter
    }
    

}


// Global functions
void linearDebugMessage(std::string errorMessage, bool isFatal) // Debug for linear instructions
{
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
}
void loopDebugMessage(std::string errorMessage, bool isFatal) // Debug for loop instructions
{
    if (isFatal)                                   // Check if error its fatal
        linearDebugMessage(errorMessage, isFatal); // Call the linear Debug message function to terminate the program
    if (loopBuffer.find(errorMessage) != std::string::npos) // Check if the error message is already in the buffer
    {
        // I don't know what to put here yet :D
    }
    else
    {
        loopBuffer += "[Loop]" + errorMessage + "\n"; // Append the error message to the buffer
    }
}
void debugDisplayOutput()
{
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
    //Function to clear the string manualy , only use it in special cases as debug display already doese that
    void clearDebug(){
        debugString=""; 
        linearDebugMessage("Debug cleared manualy.Keep in mind that output functions already do that.",false)
    }

    void debugFileOutput(){
        if(!fileOutputDebug){
        std::ofstream file("debug_output.txt"); // Open file for writing
        file << "List of all caught errors:\n" << debugString; // Print the Debug errors message to the file
        file.close(); // Close the file
    }
    else
    {
        //Add code here
    }
    }
}
} // namespace BDL

#endif // BITS_DEBUG_HPP
