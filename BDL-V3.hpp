#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <fstream>
#include <cstdlib>
#include <unordered_set>


//Macros for configuration flags
#define BDL_C_FLAG_AUTO_OUTPUT 0x01
#define BDL_C_FLAG_FILE_OUTPUT 0x02
#define BDL_C_FLAG_CONSOLE_OUTPUT 0x04
#define BDL_C_FLAG_LOOP_CHECK 0x10

class BDL {
private:
    std::mutex mtx;
    std::string fName;
    std::string debugLevel;
    std::stringstream mainBuffer;
    std::unordered_set<std::string> loopCheckBuffer;
    std::atomic<short> configFlags = 27;
    short looplimit = 1024;
    std::atomic<short> autoOutputInterval = 1024;
    std::atomic<short> autoOutputCounter = 0;
    bool wasInitialized = false;

    void debugOutputInternal() {
        if ((configFlags.load(std::memory_order_relaxed) & BDL_C_FLAG_CONSOLE_OUTPUT)) {
            std::cerr << mainBuffer.str();
        }
        if ((configFlags.load(std::memory_order_relaxed) & BDL_C_FLAG_FILE_OUTPUT)) {
            static std::ofstream outFile(fName, std::ios::app);
            if (outFile) {
                outFile << mainBuffer.str();
            }
            else {
                std::cerr << "Error: File write failed. Disabling file output.\n";
                configFlags.fetch_and(~BDL_C_FLAG_FILE_OUTPUT, std::memory_order_relaxed);
            }
        }
        mainBuffer.str("");
        autoOutputCounter = 0;
    }
public:
    void setFilePath(const std::string& fileName) {
        fName = fileName;
    }
    void setDebugLevel(const std::string& level) {
        debugLevel = '[' + level + ']';
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
        }
        else {
            configFlags &= ~BDL_C_FLAG_AUTO_OUTPUT;
        }
    }
    void setFileOutput(bool enable) {
        if (enable) {
            configFlags |= BDL_C_FLAG_FILE_OUTPUT;
        }
        else {
            configFlags &= ~BDL_C_FLAG_FILE_OUTPUT;
        }
    }
    void setConsoleOutput(bool enable) {
        if (enable) {
            configFlags |= BDL_C_FLAG_CONSOLE_OUTPUT;
        }
        else {
            configFlags &= ~BDL_C_FLAG_CONSOLE_OUTPUT;
        }
    }
    void setLoopCheck(bool enable) {
        if (enable) {
            configFlags |= BDL_C_FLAG_LOOP_CHECK;
        }
        else {
            configFlags &= ~BDL_C_FLAG_LOOP_CHECK;
        }
    }
    void initialize() {
        wasInitialized = true;
        if (debugLevel.empty()) {
            std::cerr << "Error: Debug level not set." << std::endl;
            exit(EXIT_FAILURE);
        }
        mainBuffer.str(""); // Clear the main buffer
        loopCheckBuffer.clear(); // Clear the loop check buffer
        autoOutputCounter = 0; // Reset auto output counter
        if ((configFlags & BDL_C_FLAG_FILE_OUTPUT) == 1) {
            if (fName.empty()) {
                mainBuffer << "Error: File name not set for file output but file output enabled. Defaulting to console output.\n";
                configFlags &= ~BDL_C_FLAG_FILE_OUTPUT; // Disable file output
                configFlags |= BDL_C_FLAG_CONSOLE_OUTPUT; // Enable console output
            }
        }
    }
    void linearDebugMessage(std::string message) {
        std::lock_guard<std::mutex> lock(mtx); // Lock held for entire function
        if (!wasInitialized) {
            initialize();
            linearDebugMessage("BDL not initialized. Initializing now with default configuration.");
        }
        if ((configFlags & BDL_C_FLAG_LOOP_CHECK) && !loopCheckBuffer.count(message)) {
            loopCheckBuffer.insert(message);
            mainBuffer << debugLevel << message << std::endl;
        }
        else if (!(configFlags & BDL_C_FLAG_LOOP_CHECK)) {
            mainBuffer << debugLevel << message << std::endl;
        }
        if ((configFlags & BDL_C_FLAG_AUTO_OUTPUT) && ++autoOutputCounter >= autoOutputInterval) {
            debugOutputInternal(); // Private helper (assumes lock is held)
        }
    }
    void debugOutput() {
        std::lock_guard<std::mutex> lock(mtx);
        debugOutputInternal();
    }
}; // Ensure the class definition ends properly