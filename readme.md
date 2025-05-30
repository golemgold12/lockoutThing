
NOTE: This is meant to be scalable! You can write your own servers that send back response codes that line up.
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
Here's a example...
const std::string json_data = (a list of names, with the first two being a admin's username and password);
curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5023/LogProcess"); Run this via a c++ program! We are not curl scripting here.
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());


//g++ crummytest.cpp -o crummytest -I./vcpkg/installed/x64-mingw-static/include -L./vcpkg/installed/x64-mingw-static/lib -lcurl -lws2_32 -lbcrypt -lz -lcrypt32 -lws2_32 -lwinmm -luser32 -lgdi32 -ladvapi32
Here's all the junk i had to do to compile a test program. You probably don't need to do this depending on what you use. Python is recommended for testing


That should be read by the server program, which will parse each string into a nice little package.

SERVER SIDE:
Compile the program with this command:
cmake --build build
This will create a folder called build, which will contain the program called lockupManager.exe. I'd recommend deleting the build folder for clean builds.

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



