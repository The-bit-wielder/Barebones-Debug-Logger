# Bits-Debugging-Library
My own debugging library for handling errors.
Its not particularly impressive but its easy to integrate and use.
The functions are the folowing:
```CPP
linearDebugMessage(Message,isFatal);
loopDebugMessage(Message,isFatal);
debugDisplayOutput();
```
The functions loopDebugMessage and linearDebugMessage accepts 2 parameters in the folowing format :
```CPP
linearDebugMessage(Message,isFatal);
loopDebugMessage(Message,isFatal);
```
errorMessage is a string containing the error you want to send to the console.
isFatal is a boolean which declares if the error in question is fatal and the program should end with exit code 1.
```CPP
linearDebugMessage(Message,isFatal);
```
This function is used in normal code where you just want a error to be added to the buffer for displaying at a later date,with the option to terminate the program if the error is fatal.
```CPP
loopDebugMessage(Message,isFatal);
```
This function is designed for loops to only store a error once no matter how many times the error is called by the loop, and also has the option to terminate the program.
```CPP
debugDisplayOutput();
```
This function will output all errors in the buffer and clean it for the next call.
