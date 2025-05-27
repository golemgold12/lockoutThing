#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <crow.h>
#include <string.h>
//  COMPILE VIA: g++ main.cpp -IC:\curl\include -LC:\curl\lib -lcurl -o SecurityCheck.exe
size_t writeCallback(void *contents, size_t size, size_t newMember, std::string *out)
{
    size_t totalSize = size * newMember;
    out->append((char *)contents, totalSize);
    std::cout << "Callback written to us.";
    return totalSize;
}

void ReadAndTakeAction(const std::vector<std::string> &usernames)
{

    // note that the first name will be the password for the admin
    curl_global_init(CURL_GLOBAL_ALL); // initialize!
    std::ostringstream newString;

    newString << "[";
    for (size_t i = 0; i < usernames.size(); i++)
    {

        newString << "\"" << usernames[i] << "\"";
        int tester = usernames.size() - 1;
        if (i != tester)
        {

            newString << ", ";
        }
    }
    newString << "]";
    std::string jsonArg = newString.str();
    std::cout << jsonArg;
    CURL *curl = curl_easy_init();

    if (curl && newString)
    {
        CURLcode res;
        struct curl_slist *headers = NULL;

        curl_easy_setopt(curl, CURLOPT_URL, "http://classwork.engr.oregonstate.edu:5012/api/lockup"); // No idea atm for security

        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonArg.c_str());
        // now we do response

        std::string responseString;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << " \n";
        else
        {
            if (responseString != "")
            {
                std::cout << "Successfully wrote message to the server";
             
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return;
}

int programRun()
{
 

    crow::SimpleApp crowSimple;

    CROW_ROUTE(crowSimple,"/LogProcess").methods(crow::HTTPMethod::Post)([](const crow::request & crowreq){

        crow::json::rvalue requestBody = crow::json::load(crowreq.body);
        if(requestBody){
            std::cout << "We've recieved some form of message ";
          
            std::vector<std::string> ourusernames;
            for(const auto&  val : requestBody){
                ourusernames.push_back(val.s());

            }
            //std::vector<std::string> ourUsers = {"superAdmin1999", "testAdmin30", "Nolan80", "MackTruck12"}; // test

            //ReadAndTakeAction(ourUsers);
            ReadAndTakeAction(ourusernames);

            return crow::response(200, "\nFound and processes usernames also WHY are we using crow");
        } else{

            return crow::response(400, "\nBad Request!");
        }

    });
    crowSimple.port(5023).multithreaded().run();

    /*
    Someting here

    */
    std::cout << "\nEnded program.";
    return 0;
}
int main()
{

    std::cout << "\n\n Starting Program \n\n";
    int testShot = programRun();
    if (testShot)
    {

        return 0;
    }
    else
    {
        return 1;
    }
}