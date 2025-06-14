#ifndef BDL
#define BDL

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unordered_set>


class BDL {
	private:
		std::string fName;
		std::string debugLevel;
		std::stringstream mainBuffer;
		std::unordered_set<std::string> loopCheckBuffer;
		short configFlags = 27;
			// Default configuration flags: auto output, file output, console output, enable exit cleanup
			// 0b00011101 = 27 in decimal
			// 1st bit: auto output enabled
			// 2nd bit: file output enabled
			// 3rd bit: console output enabled
			// 4th bit: enable exit cleanup
			// 5th bit: loop check enabled 
			);
		short looplimit = 1024;
		short autoOutputInterval = 1024;
		short autoOutputCounter = 0;
		bool wasInitialized = false;
		//Macros for configuration flags
#define BDL_C_FLAG_AUTO_OUTPUT 0x01
#define BDL_C_FLAG_FILE_OUTPUT 0x02
#define BDL_C_FLAG_CONSOLE_OUTPUT 0x04
#define BDL_C_FLAG_ENABLE_EXIT_CLEANUP 0x08
#define BDL_C_FLAG_LOOP_CHECK 0x10
		//Macros for runtime flags
	public:
		void setFilePath(const std::string& fileName) {
			fName = fileName;
		}
		void setDebugLevel(const std::string& level) {
			debugLevel = '[' +level + ']';
		}
		void setLoopLimit(short limit) { 
			looplimit = limit;
		}
		void setAutoOutputInterval(short interval) {
			autoOutputInterval = interval;
		}
		void setAutoOutput(bool enable) {
			if (enable) {
				configFlags |= BDL_C_FLAG_AUTO_OUTPUT;
			} else {
				configFlags &= ~BDL_C_FLAG_AUTO_OUTPUT;
			}
		}
		void setFileOutput(bool enable) {
			if (enable) {
				configFlags |= BDL_C_FLAG_FILE_OUTPUT;
			} else {
				configFlags &= ~BDL_C_FLAG_FILE_OUTPUT;
			}
		}
		void setConsoleOutput(bool enable) {
			if (enable) {
				configFlags |= BDL_C_FLAG_CONSOLE_OUTPUT;
			} else {
				configFlags &= ~BDL_C_FLAG_CONSOLE_OUTPUT;
			}
		}
		void setEnableCleanup(bool enable) {
			if (enable) {
				configFlags |= BDL_C_FLAG_ENABLE_EXIT_CLEANUP;
			} else {
				configFlags &= ~BDL_C_FLAG_ENABLE_EXIT_CLEANUP;
			}
		}
		void setLoopCheck(bool enable) {
			if (enable) {
				configFlags |= BDL_C_FLAG_LOOP_CHECK;
			} else {
				configFlags &= ~BDL_C_FLAG_LOOP_CHECK;
			}
		}
		void initialize() {
			wasInitialized = true;
			if (debugLevel.empty()) {
				cerr << "Error: Debug level not set." << std::endl;
				exit(EXIT_FAILURE);
			}
			mainBuffer.str(""); // Clear the main buffer
			loopCheckBuffer.clear(); // Clear the loop check buffer
			autoOutputCounter = 0; // Reset auto output counter
			if ((configFlags & BDL_C_FLAG_FILE_OUTPUT) == 1) {
				if (fName.empty()) {
					mainBuffer << "Error: File name not set for file output but file output enabled.Defaulting to console output.\n";
					configFlags &= ~BDL_C_FLAG_FILE_OUTPUT; // Disable file output
					configFlags |= BDL_C_FLAG_CONSOLE_OUTPUT; // Enable console output
				}
			}
			if ((configFlags & BDL_C_FLAG_ENABLE_EXIT_CLEANUP) == 1) {
				atexit(cleanup); // Register cleanup function to be called on exit
			}
		}
		void linearDebugMessage(std::string message) {
			if (!wasInitialized) {
				initialize(); // Ensure initialization if not done yet
				linearDebugMessage("BDL not initialized. Initializing now with default configuration.");
			}
			if ((configFlags & BDL_C_FLAG_LOOP_CHECK) == 1) {
				if (loopCheckBuffer.find(message) == loopCheckBuffer.end()) {
					loopCheckBuffer.insert(message);
					mainBuffer << debugLevel << message << std::endl;
				}
			}
			else
			{
				mainBuffer << debugLevel << message << std::endl;
			}
			if ((configFlags & BDL_C_FLAG_AUTO_OUTPUT) == 1) {
				autoOutputCounter++;
				if (autoOutputCounter >= autoOutputInterval) {}
				debugOutput;
			}
			}
		void debugOutput() {
			if ((configFlags & BDL_C_FLAG_CONSOLE_OUTPUT) == 1) {
				std::cerr << mainBuffer.str(); // Output to console
			}
			if ((configFlags & BDL_C_FLAG_FILE_OUTPUT) == 1) {
				std::ofstream outFile(fName);
				if (outFile.is_open()) {
					outFile << mainBuffer.str(); // Output to file
					outFile.close();
				} else {
					std::cerr << "Error: Unable to open file " << fName << " for writing." << std::endl;
					configFlags &= ~BDL_C_FLAG_FILE_OUTPUT; // Disable file output if file cannot be opened
				}
			}
			mainBuffer.str(""); // Clear the main buffer after output
			if ((configFlags & BDL_C_FLAG_AUTO_OUTPUT) == 1) {
				autoOutputCounter = 0; // Reset auto output counter
            if ((configFlags & BDL_C_FLAG_AUTO_OUTPUT) == 1) {
			}
		}
		void cleanup() {
				mainBuffer.str(""); // Clear the main buffer
				loopCheckBuffer.clear(); // Clear the loop check buffer
				autoOutputCounter = 0; // Reset auto output counter
			}
		}
};
#endif // !BDL
