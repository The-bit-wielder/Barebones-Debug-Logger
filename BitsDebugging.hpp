#ifndef BITS_DEBUG_HPP // Header guard to prevent multiple inclusions of the header file
#define BITS_DEBUG_HPP
#define BDL // Define the BDL macro to indicate that the library is being used

// Include standard libraries
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

namespace BDL
{

    std::string debugString;                 
    std::string loopBuffer = "";             
    std::string logName;                     
    bool debugErrorDisplay = false;          
    bool autoOutputDebug = true;             
    bool fileOutputDebug = false;            
    bool enableConsoleOutput = true;         
    bool enableFileOutput = false;           
    unsigned short debugErrorCount = 0;      
    unsigned short debugErrorCountMax = 255; 

    void appendToGlobalString(std::string& errorMessage) 
    {
        debugErrorCount++;                                           
        debugString += errorMessage + "\n";                          
        if (debugErrorCount > debugErrorCountMax && autoOutputDebug) 
        {
            outputDebug();       
            debugErrorCount = 0; 
        }
    }
	/// This function will set the parameters for the debugging system.
	/// @param name="consoleOut" This will enable or disable console output.
	/// @param name="fileOut" This will enable or disable file output.
	/// @param name="logFileName" This is the name of the log file that will be created.
	/// @param name="autoOutput" This will enable or disable auto output.
	/// @param name="autoOutputInterval" This is the interval for auto output in seconds.
    void setParams(bool consoleOut, bool fileOut, std::string logFileName, bool autoOutput, u_int16_t autoOutputInterval) 
    {
        
        if (consoleOut == true) 
        {
            linearDebugMessage("Console output enabled.\n", false); 
        }
        if (fileOut == true) 
        {
            if (logFileName == "")
            {
                logName = "debug.log"; 
            }
            else
            {
                logName = logFileName; 
            }
            std::ofstream file(logName); 
        }
        if (autoOutput) 
        {
            linearDebugMessage("Auto output enabled. Interval: " + std::to_string(autoOutputInterval) + "\n", false); 
            autoOutputDebug = true;                                                                                   
        }
    }
	/// This function will output a message to the console and/or file.
	/// @param name="errorMessage" This is the message that will be outputted.
	/// @param name="isFatal" This will determine if the message is fatal or not.
    void linearDebugMessage(std::string errorMessage, bool isFatal) 
    {
        if (isFatal == true) 
        {
            appendToGlobalString("[Fatal]:" + errorMessage); 
            outputDebug();                                   
            std::exit(EXIT_FAILURE);                         
        }
        else
        {
            appendToGlobalString("[Error]:" + errorMessage); 
        }
    }
	/// This function will output a message to the console and/or file.
	/// This function will also check if the message is already in the buffer.
	/// @param name="errorMessage" This is the message that will be outputted.
	/// @param name="isFatal" This will determine if the message is fatal or not.
    void loopDebugMessage(std::string errorMessage, bool isFatal) 
    {
        if (isFatal)                                               
            linearDebugMessage("[Loop]:" + errorMessage, isFatal); 
        if (loopBuffer.find(errorMessage) == std::string::npos)    
        {
            linearDebugMessage("[Loop]:" + errorMessage, false); 
            loopBuffer += errorMessage + "\n";                   
        }
    }
	/// This function will output an info message to the console and/or file.
	/// @param name="errorMessage" This is the message that will be outputted.
    void infoDebugMessage(std::string errorMessage) 
    {
        appendToGlobalString("[Info]:" + errorMessage); 
    }
	/// This function will output a warning message to the console and/or file.
	/// @param name="errorMessage" This is the message that will be outputted.
    void warningDebugMessage(std::string errorMessage) 
    {
        appendToGlobalString("[Warn]:" + errorMessage); 
    }
	///This function will output the current parameters of the debugging system to the console.
    void paramOutput() 
    {
        std::cerr << "Console output: " << (enableConsoleOutput ? "enabled" : "disabled") << "\n"
                  << "File output: " << (enableFileOutput ? "enabled" : "disabled") << "\n"
                  << "Log file name: " << logName << "\n"
                  << "Auto output: " << (autoOutputDebug ? "enabled" : "disabled") << "\n"; 
    }
	/// This function will output the debug string to the console and/or file.
	/// It will also clear the debug string after outputting to console and/or file.
	/// To configure the output method, use setParams() function.
	/// By default it will output to the console.
    void outputDebug() 
    {
        if (enableConsoleOutput) 
        {
            debugConsoleOutput(false); 
        }
        if (enableFileOutput) 
        {
            debugFileOutput(false); 
        }
        debugString = "";    
        debugErrorCount = 0; 
    }
	/// This function will output the debug string to the console.
	/// This is the default output method used by the debugging system.
	/// Its also considered a fallback method if the file output fails.
	/// @param name="eraseString" This will erase the debug string after outputting to console.
    void debugConsoleOutput(bool eraseString)
    {
        if (loopBuffer != "")                 
            appendToGlobalString(loopBuffer); 
        if (debugErrorDisplay == true)       
        {
            std::cerr << "List of all new errors since last display call:\n"
                      << debugString; 
        }
        else
        {
            debugErrorDisplay = true; 
            std::cerr << "Debug display called. List of all caught errors:\n"
                      << debugString; 
        }
        if (eraseString)
        {
            debugString = ""; 
        }
    }
	/// This function will output the debug string to a file.
	/// To use this function though outputDebug() must first set fileOutputDebug to true.
	/// Can also be called manualy to output the debug string to a file.
	/// @param name="eraseString" If true the debug string will be erased after outputting to file.
    void debugFileOutput(bool eraseString)
    {
        if (!fileOutputDebug)
        {
            std::ofstream file(logName); 
            if (!file.is_open())
            {
                std::cerr << "Error opening file for writing: " << logName << "\n"; 
                debugConsoleOutput(true);                                           
            }
            else
            {
                file << "List of all caught errors:\n"
                     << debugString; 
                file.close();        
                fileOutputDebug = true; 
            }
        }
        else
        {
            std::ofstream file(logName, std::ios::app);
            file << "List of all new caught errors since last call:\n"
                 << debugString; 
        }
        if (eraseString) 
        {
            debugString = ""; 
        }
    }
	/// Function to clear debug string and will also add a warning that it was cleared manually.
    void clearDebug()
    {
        debugString = "";
        warningDebugMessage("Debug cleared manually. Keep in mind that output function already does that.");
    }
	/// Test function to demonstrate the usage of the debugging functions
	/// and to to provide a centralized system for looking if the debugging system is working. 
    /// This function doesnt take params.
    void testDebug()
    {
        linearDebugMessage("Test debug message", false); 
        loopDebugMessage("Test loop debug message", false); 
        infoDebugMessage("Test info debug message"); 
        warningDebugMessage("Test warning debug message"); 
        paramOutput(); 
    }
} // namespace BDL

#endif // BITS_DEBUG_HPP
