#ifndef BITS_DEBUG_HPP // Header guard to prevent multiple inclusions of the header file
#define BITS_DEBUG_HPP
#define BDL // Define the BDL macro to indicate that the library is being used

// Include standard libraries
#include <iostream>
#include <string>
#include <fstream>

namespace BDL // Namespace for the library
{

    std::string debugString;            // String for storing Debug errors
    std::string loopBuffer = "";        // String for storing loop errors
    std::string logName;                // String for storing the log file name
    bool debugErrorDisplay = false;     // Flag for error displayed
    bool autoOutputDebug = true;        // Flag for auto display
    bool fileOutputDebug = false;       // Flag for file output
    bool enableConsoleOutput = true;    // Flag for console output
    bool enableFileOutput = false;      // Flag for file output
    u_int16_t debugErrorCount = 0;      // Counter for Debug errors
    u_int16_t debugErrorCountMax = 255; // Set the default max number of debug errors to store

    void appendToGlobalString(std::string errorMessage) // Debug buffer function for storing Debug errors (don't use it)
    {
        debugErrorCount++;                                           // Increment the Debug error counter
        debugString += errorMessage + "\n";                          // Append the string to the Debug string and add a new line
        if (debugErrorCount > debugErrorCountMax && autoOutputDebug) // Check if the Debug error counter exceeds the error count limit
        {
            outputDebug();       // Call the Debug output function to display the errors
            debugErrorCount = 0; // Reset the Debug error counter
        }
    }
    void setParams(bool consoleOut, bool fileOut, std::string logFileName, bool autoOutput, u_int16_t autoOutputInterval) // Function to set parameters for Debug output
    {
        // Set the parameters for Debug output
        if (consoleOut == true) // Check if console output is enabled
        {
            linearDebugMessage("Console output enabled.\n", false); // Print message to console
        }
        if (fileOut == true) // Check if file output is enabled
        {
            logName = logFileName;       // Set the log file name
            std::ofstream file(logName); // Open file for writing
        }
        if (autoOutput) // Check if auto output is enabled
        {
            linearDebugMessage("Auto output enabled. Interval: " + std::to_string(autoOutputInterval) + "\n", false); // Print message to console
            autoOutputDebug = true;                                                                                   // Set the auto display flag to true
        }
    }
    void linearDebugMessage(std::string errorMessage, bool isFatal) // Debug for linear instructions
    {
        if (isFatal == true) // Check if error its fatal
        {
            appendToGlobalString("[Fatal]:" + errorMessage); // Call the Debug buffer function
            outputDebug(); // Call the Debug output function to display the errors
        } 
        else
        {
            appendToGlobalString("[Error]:" + errorMessage); // Call the Debug buffer function
        }
    }
    void loopDebugMessage(std::string errorMessage, bool isFatal) // Debug for loop instructions
    {
        if (isFatal)                                            // Check if error its fatal
            linearDebugMessage("[Loop]:"+ errorMessage, isFatal);          // Call the linear Debug message function to terminate the program
        if (loopBuffer.find(errorMessage) == std::string::npos) // Check if the error message is already in the buffer
        {
            linearDebugMessage("[Loop]:" + errorMessage, false); // Call the linear Debug message function to store the error message
            loopBuffer += errorMessage + "\n";                  // Append the error message to the loop buffer
        }
    }
    void infoDebugMessage(std::string errorMessage) // Debug for info instructions
    {
        appendToGlobalString("[Info]:" + errorMessage); // Call the Debug buffer function
    }
    void warningDebugMessage(std::string errorMessage) // Debug for warning instructions
    {
        appendToGlobalString("[Warn]:" + errorMessage); // Call the Debug buffer function
    }
    void paramOutput() // Function to output the parameters for Debug output
    {
        std::cerr << "Console output: " << (enableConsoleOutput ? "enabled" : "disabled") << "\n"
                  << "File output: " << (enableFileOutput ? "enabled" : "disabled") << "\n"
                  << "Log file name: " << logName << "\n"
                  << "Auto output: " << (autoOutputDebug ? "enabled" : "disabled") << "\n"; // Print the parameters to console
    }
    void outputDebug() // Function to output the Debug errors
    {
        if (enableConsoleOutput) // Check if console output is enabled
        {
            debugConsoleOutput(false); // Call the Debug display function
        }
        if (enableFileOutput) // Check if file output is enabled
        {
            debugFileOutput(false); // Call the Debug file output function
        }
        debugString = "";    // Clear the Debug string
        debugErrorCount = 0; // Reset the Debug error counter
    }
    void debugConsoleOutput(bool eraseString)
    {
        if (loopBuffer != "")                 // Check if the loop buffer is not empty
            appendToGlobalString(loopBuffer); // Call the Debug buffer function
        if (debugErrorDisplay == true)        // Check if end of Debug was called
        {
            std::cerr << "List of all new errors since last display call:\n"
                      << debugString; // Print the end error message
        }
        else
        {
            debugErrorDisplay = true; // Set the Debug display flag to true
            std::cerr << "Debug display called. List of all caught errors:\n"
                      << debugString; // Print the Debug errors message
        }
        if (eraseString) // Check if the string should be erased
        {
            debugString = ""; // Clear the Debug string
        }
    }
    void debugFileOutput(bool eraseString)
    {
        if (!fileOutputDebug)
        {
            std::ofstream file(logName); // Open file for writing
            if (!file.is_open())
            {
                std::cerr << "Error opening file for writing: " << logName << "\n"; // Print error message if file cannot be opened
                return;                                                             // Exit the function if file cannot be opened
            }
            file << "List of all caught errors:\n"
                 << debugString; // Print the Debug errors message to the file
            file.close();        // Close the file
        }
        else
        {
            std::ofstream file(logName, std::ios::app); // Open file for appending
            file << "List of all new caught errors since last call:\n"
                 << debugString; // Print the Debug errors message to the file
        }
        if (eraseString) // Check if the string should be erased
        {
            debugString = ""; // Clear the Debug string
        }
    }

    // Function to clear the string manually, only use it in special cases as debug display already does that
    void clearDebug()
    {
        debugString = "";
        warningDebugMessage("Debug cleared manually. Keep in mind that output function already does that.");
    }
} // namespace BDL

#endif // BITS_DEBUG_HPP
