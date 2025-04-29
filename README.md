# Bits-Debugging-Library
My own debugging library for handling errors.
Its not particularly impressive but its easy to integrate and use.
### The namespace for this library is BDL
A list of functions currently implemented are:
```CPP
setParams(EnableConsoleOutput,EnableFileOutput,SetFileName,EnableAutoOutput,CountTillAutoOutput);
//Set parameters for the debugging system
linearDebugMessage(Message,IsFatal); 
//Function for linear code
loopDebugMessage(Message,IsFatal); 
//Function for looping code with extra handling for repeated error messages
warningDebugMessage(Message);
//Function for warning outputs
infoDebugMessage(Message);
//Function for info output
outputDebug();
//Outputs based on the parameters set (By default its console output with 255 auto outputting and erases the buffer)
debugConsoleOutput(EraseString);
//Output all error messages in console since last output call 
debugFileOutput(EraseString);
//Output all error messages in a file since last output call
paramOutput();
//Outputs all set parameters in the console
clearDebug();
//Used for manualy cleaning the buffer without outputting anything
appendToGlobalString(Message)
//This function its for internal use please avoid using it:
```
## Details each parameter you can pass to the functions:
1. EnableConsoleOutput , Boolean. Will enable or disable the console output system, by default its true.
2. EnableFileOutput , Boolean. Will enable or disable the file output system , by default its false.
3. SetFileName , String. Sets the filename for the debugging log , by default its set to latest.log.
4. EnableAutoOutput , Boolean. Will enable or disable the automatic displaying system after a declared amount of error messages have been collected default true.
5. CountTillAutoOutput , Unsigned integer 16 bits long (65535) , sets the max amount of errors to collect before auto outputing , default 255.
6. Message , String , Message to be handled by the debugging system.
7. IsFatal , Boolean , If the debugging system should terminate the program if the error is encountered and output all stored messages soo far.
8. EraseString , Boolean , will erase the string when the output functions are called.
## Functions in detail
The functions for adding debug messages are with each output example:
```CPP
Message="This is a example message";
IsFatal=False;
linearDebugMessage(Message,IsFatal);// [Error]:This is a example message
loopDebugMessage(Message,IsFatal);//   [Error]:[Loop]:This is a example message
warningDebugMessage(Message);//        [Warn]:This is a example message
infoDebugMessage(Message);//           [Info]:This is a example message 
```
The functions for outputing and configuring:
```CPP
setParams(); //Based on the parameters you plug in will globaly set the output debug settings for every call
outputDebug();//Will output based on the parameters set by setParams and clear the buffer
debugConsoleOutput();//For manualy outputting to console when needed
debugFileOutput();//For manualy outputting to file when needed
```
P.S Im still learning soo if you have any suggestions please let me know
