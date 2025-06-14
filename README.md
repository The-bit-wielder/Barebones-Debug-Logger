# BDL - Barebones Debug Logger (Ex Bits Debugging Library)
BDL (Barebones Debug Logger) is a lightweight and thread-safe C++ logging library designed for simplicity and efficiency. It's ideal for scenarios where std::cout isn't sufficient due to thread-safety concerns, and comprehensive logging frameworks like spdlog are overkill. BDL aims to provide core logging functionalities without unnecessary complexity, allowing you to focus on debugging rather than configuration.

## Features
🚀 Thread-safe: All logging operations are protected by a mutex, ensuring safe concurrent access from multiple threads.</br>
📝 Console and File Logging: Output logs directly to the console (std::cerr) and/or to a specified file.</br>
⚡ Efficient: Designed for minimal overhead, making it suitable for non-performance-critical debugging paths.</br>
🧩 Zero Dependencies: Relies solely on the C++20 standard library.</br>
🔧 Hackable: Contained within a single header file, making it easy to understand, modify, and integrate into your projects.</br>
🔁 Loop Check: An optional feature to prevent logging the same message repeatedly, useful for avoiding log spam in loops.</br>
⏰ Auto Output: Automatically flushes logs after a configurable number of messages. </br>
## Installation
BDL is a header-only library. Simply include the BDL.hpp file in your project.
```CPP

#include "BDL.hpp"

int main() {
    BDL logger;

    // Initialize the logger. This is crucial before logging messages.
    // If not called explicitly, it will be called on the first log message
    // but it's good practice to initialize upfront.
    logger.setDebugLevel("INFO"); // Set a debug level string, e.g., "[INFO]"
    logger.initialize();

    // Enable console output (it's enabled by default, but shown for clarity)
    logger.setConsoleOutput(true);

    // Log a simple message
    logger.linearDebugMessage("Application started successfully.");

    // You can change debug levels on the fly
    logger.setDebugLevel("WARN");
    logger.linearDebugMessage("Potential issue detected.");

    // Manually flush the buffer to output
    logger.debugOutput();

    // Disable auto output and loop check for specific scenarios
    logger.setAutoOutput(false);
    logger.setLoopCheck(false);
    logger.linearDebugMessage("Auto output and loop check disabled temporarily.");
    logger.debugOutput(); // Manual flush needed now

    return 0;
}
```

## Configuration Options
BDL provides several methods to configure its behavior:

```CPP
#include "BDL.hpp"

int main() {
    BDL logger;
    logger.setDebugLevel("DEBUG");
    logger.initialize(); // Always initialize after setting initial configurations

    // Enable file logging and set the file path
    logger.setFileOutput(true);
    logger.setFilePath("application.log");
    logger.linearDebugMessage("Logs are now being written to application.log");

    // Disable console output
    logger.setConsoleOutput(false);
    logger.linearDebugMessage("Console output is now disabled."); // This message will only go to the file

    // Disable duplicate message filtering (loop check)
    logger.setLoopCheck(false);
    logger.linearDebugMessage("This message will appear multiple times if logged consecutively.");
    logger.linearDebugMessage("This message will appear multiple times if logged consecutively.");
    logger.debugOutput();

    // Configure auto output interval
    logger.setAutoOutput(true); // Ensure auto output is enabled
    logger.setAutoOutputInterval(5); // Flush logs after every 5 messages
    for (int i = 0; i < 7; ++i) {
        logger.linearDebugMessage("Message " + std::to_string(i));
    }
    // The first 5 messages will be flushed, then the next 2 will remain in buffer until another 3 messages or manual flush.

    // Manually flush pending logs
    logger.debugOutput();

    // Enable exit cleanup (note: atexit registration needs to be handled carefully with classes)
    // The current implementation of atexit(cleanup) won't work directly with member functions.
    // Consider a singleton pattern for BDL if you want automatic cleanup on exit.
    // logger.setEnableCleanup(true); // This flag can be set, but atexit needs manual setup for object instances.

    return 0;
}
```
## Why BDL?
I created BDL out of a need for a straightforward, thread-safe logging solution without the steep learning curve or excessive dependencies often found in larger libraries.

std::cout isn't inherently thread-safe, leading to interleaved output in concurrent applications.
Other libraries can be overly complex, requiring extensive configuration just to get basic logging operational.
It was a fun project to build a minimal, yet functional, logging utility.
