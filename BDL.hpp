#ifndef BDL

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unordered_set>

namespace BDL
{
	// Variables
	bool initialized = false;
	bool logFileEnabled = false;
	bool consoleOutputEnabled = true;
	bool autoOutputEnabled = false;
	bool firstOutputConsole = true;
	bool firstOutputToFile = true;
	bool outputAtExit = false;
	bool outputAtExitEnabled = false;
	short int autoOutputInterval = 1024;
	short int autoOutputCounter = 0;
	unsigned short int loopStringLimit = 1024;
	std::string logFile;
	std::string globalString;
	std::unordered_set<std::string> loopString;
	std::vector<std::string> debugLevels;
	std::ofstream logFileStream;

	/// @brief Initialize the BDL logging system
	void initialize()
	{
		if (!initialized)
		{
			globalString = "Beginning of log\n";
			debugLevels.resize(256);  // Ensure space for all possible levels
			debugLevels[0] = "[Debug]";
			debugLevels[1] = "[Info]";
			debugLevels[2] = "[Warning]";
			debugLevels[3] = "[Error]";
			debugLevels[4] = "[Fatal]";
			initialized = true;
		}
		if (outputAtExit && !outputAtExitEnabled)
		{
			std::atexit(cleanup);
			outputAtExitEnabled = true;
		}
		if (!logFileEnabled && logFileStream.is_open()){
			logFileStream.close();
		}
	}

	/// @brief Set the parameters for the BDL logging system , also initializes the system if it is not already initialized
	/// @param logFileEnabled Whether to enable logging to a file (default is false)
	/// @param consoleOutputEnabled Whether to enable console output (default is true)
	/// @param autoOutputEnabled Whether to enable automatic output (default is false)
	/// @param autoOutputInterval The interval to output the log file (default is 1024)
	/// @param logFile The path to the log file (default is "log.txt")

	void setParams(const bool logFileEnabled,
				   const bool consoleOutputEnabled,
				   const bool autoOutputEnabled,
				   const short int &autoOutputInterval,
				   const std::string &logFile,
				   const bool outputAtExit)
	{
		BDL::logFileEnabled = logFileEnabled;
		BDL::consoleOutputEnabled = consoleOutputEnabled;
		BDL::autoOutputEnabled = autoOutputEnabled;
		BDL::autoOutputInterval = autoOutputInterval;
		BDL::logFile = (logFile != "") ? logFile : "log.txt";
		BDL::outputAtExit = outputAtExit;
		BDL::initialize();
	}

	/// @brief Output the parameters for the BDL logging system (only for debugging purposes) (only outputs to console)

	void outputParams()
	{
		std::cout << "Log File Enabled: " << logFileEnabled << std::endl;
		std::cout << "Console Output Enabled: " << consoleOutputEnabled << std::endl;
		std::cout << "Auto Output Enabled: " << autoOutputEnabled << std::endl;
		std::cout << "Auto Output Interval: " << autoOutputInterval << std::endl;
		std::cout << "Log File: " << logFile << std::endl;
	}

	/// @brief Internal function to append a message to the global string (avoid using this function directly)
	/// @param message The message to append

	void globalStringFunc(std::string &message)
	{
		if (!initialized) initialize();
		message.append("\n");
		if (autoOutputEnabled)
		{
			autoOutputCounter++;
			if (autoOutputCounter >= autoOutputInterval)
			{
				outputLog();
				autoOutputCounter = 0;
				globalString = "";
			}
		}
		globalString.append(message);
	}

	/// @brief Linear Debug Log Function to handle messages based on the level ID
	/// @param message The message to log
	/// @param levelID The level ID to log the message at (0-4 are defaulted , anything else is a custom level)
	/// @param isFatal Whether the message is a fatal error (default is false)

	void linearDebugLog(const std::string &message, const char levelID, bool isFatal)
	{
		std::string internalMessage;
		internalMessage = (debugLevels[levelID] != "") ? debugLevels[levelID] : "[Unknown]:";
		internalMessage.append(message);
		globalStringFunc(internalMessage);
		if (isFatal)
		{
			outputLog();
			exit(levelID);
		}
	}

	/// @brief loop Debug Log Function to handle messages based on the level ID
	/// @brief This function is used to prevent duplicate messages from being logged
	/// @brief If at any point theres a need to clear the loopString , call BDL::loopString = ""
	/// @param message The message to log
	/// @param levelID The level ID to log the message at (0-4 are defaulted , anything else is a custom level)
	/// @param isFatal Whether the message is a fatal error (default is false)

	void loopDebugLog(const std::string &message, const char levelID, bool isFatal)
	{
		if (loopString.empty() || loopString.find(message) == loopString.end()){
			loopString.insert(message);
			linearDebugLog("[Loop]" + message, levelID, isFatal);
		}
		if (loopString.size() >= loopStringLimit)
		{
			linearDebugLog("Loop string cleared automatically to prevent overflow", 0, false);
			loopString.clear();
		}
	}

	/// @brief Set the debug level for a custom level
	/// @param levelID The level ID to set the debug level for
	/// @param levelName The name of the debug level

	void setDebugLevel(const char levelID, const std::string &levelName)
	{
		if (levelID < 0 || levelID > 255)
		{
			linearDebugLog("Invalid level ID declared , defaulting to 255", 2, false);
			debugLevels[255] = levelName;
		}
		else
			debugLevels[levelID] = levelName;
	}

	/// @brief Output the log to the file and console
	/// @brief This function is automatically called when the program ends if the outputAtExit flag is set to true
	/// @brief This function is also automatically called when the autoOutputEnabled flag is set to true
	/// @brief This function by default outputs to the console

	void outputLog()
	{

		if (logFileStream.is_open() && logFileEnabled)
		{
			logFileStream << globalString;
			globalString = "";
		}
		else
		{
			linearDebugLog("File stream error , defaulting to console", 1, false);
			logFileEnabled = false;
			consoleOutputEnabled = true;
		}
		if (consoleOutputEnabled)
		{
			if (!firstOutputConsole)
				std::cerr << "All new logged messages since last call\n";
			std::cerr << globalString;
			globalString = "";
			firstOutputConsole = false;
		}
	}

	/// @brief Debug System Test Function to test the BDL logging system

	void debugSystemTest(){
		std::string tempLevel = debugLevels[0];
		BDL::setDebugLevel(0, "Test");
		BDL::linearDebugLog("This is a test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::loopDebugLog("This is a loop test", 0, false);
		BDL::outputLog();
		BDL::setDebugLevel(0, tempLevel);
	}
	void cleanup(){
		outputLog();
		if (logFileStream.is_open())
			logFileStream.close();
	}
} // namespace BDL
#endif // BDL