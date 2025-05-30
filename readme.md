
NOTE: This is meant to be scalable! You can write your own servers that send back response codes that line up. I tried my best to make em as general as possible.
NOTE 2: Server response codes and codes sent to client are sometimes different!
Installation:
First, install vcpkg
git clone https://github.com/microsoft/vcpkg.git
Go to the vcpkg folder via cd.
.\bootstrap-vcpkg.bat 
./vcpkg install curl:x64-mingw-static
./vcpkg install crow:x64-mingw-static
Type these commnads in to install crow and curl.

When you are ready to start the server microservice, just run the lockupManager.exe program. You may need to confirm permissions as it's gonna be using localhost.

CLIENT SIDE(Calling the program):
Send a array of strings via JSON to the server. The first string is the passkey to access the microservice, the secound is the admin username for whichever server the microservice is connected to, and the third is the passkey for that admin user.
If you have the correct passkey, the microservice will handle calling the server. We are returned results based on HTTP codes.
SERVER SIDE:
Compile the program with this command:
cmake --build build
This will create a folder called build, which will contain the program called lockupManager.exe. I'd recommend deleting the build folder for clean builds.\
Run the exe file to start up the server. Hit CTRL-C to stop it.

WHAT TO RETURN?
This program will be listening for the following codes:
200 - success
400 - Account not found
401 - No admin credentials
403 - Bad Password
423 - Account is locked
Anything else will tell the user to see the readme!
Questions?
Email me at freundl@oregonstate.edu.
![Sequence diagram](https://github.com/user-attachments/assets/9d40a7ee-f3e3-4ba4-b134-e90f3b74d3c4)



