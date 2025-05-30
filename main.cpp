#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <crow.h>
#include <string.h>

/* 
Structure requests to this program like this: 
["username", "password", "Username of Acconut", "Username of account"...]
Structure your server responses like this:
200 - success
400 - Account not found
401 - No admin credentials
403 - Bad Password
423 - Account is locked
*/
size_t writeCallback(void *contents, size_t size, size_t newMember, std::string *out)
{
    size_t totalSize = size * newMember;
    out->append((char *)contents, totalSize);
    std::cout << "Callback written to us.";
    return totalSize;
}

int ReadAndTakeAction(const std::vector<std::string> &usernames, const std::string passCheck)
{    

    if(usernames[0] != passCheck){

        return 2;
    }
    // note that the first name will be the password for the admin
    curl_global_init(CURL_GLOBAL_ALL); // initialize!
    std::ostringstream newString;

    newString << "[";
    for (size_t i = 1; i < usernames.size(); i++)
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
    //std::cout << jsonArg;
    CURL *curl = curl_easy_init();

    if (curl && newString)
    {
        CURLcode res;
        struct curl_slist *headers = NULL;

        /*
        You don't have to just use my site. This can work anywhere as long as you follow the error codes.
        */
        curl_easy_setopt(curl, CURLOPT_URL, "http://classwork.engr.oregonstate.edu:5012/api/lockup");  
        
        
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonArg.c_str());
       

        std::string responseString;
        // now we do response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            std::cerr << "Curl failed to connect to server with error: " << curl_easy_strerror(res) << " \n";
            return 0;
        }
          
        else
        {
            long HTTPCodeResponse = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &HTTPCodeResponse);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            return HTTPCodeResponse;


        }

      
    }
    return 0;

}

int programRun(std::string passCheck)
{

    crow::SimpleApp crowSimple;

    CROW_ROUTE(crowSimple,"/LogProcess").methods(crow::HTTPMethod::Post)([passCheck](const crow::request & crowreq){

        crow::json::rvalue requestBody = crow::json::load(crowreq.body);

        if(!requestBody){
            crow::json::wvalue badJSON;
            badJSON["status"] = "error";
            badJSON["message"] = "Your request had data that isn't JSON. Please format it accordingly.";
            return crow::response(400, badJSON);
        }
        else{
            
            std::vector<std::string> ourusernames;

            for(const auto&  val : requestBody){

            
           
                ourusernames.push_back(val.s());
                

            }
          

            try{
                int loginAttempt = ReadAndTakeAction(ourusernames,passCheck);
                crow::json::wvalue acceptedJSON;

                switch(loginAttempt){
                    case 0:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "Failure to connect.";
                        return crow::response(200, acceptedJSON);

                    break;
                    case 2:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "User passkey for this progam wasn't correct";
                        return crow::response(401, acceptedJSON);

                    break;
                    case 200:
                        acceptedJSON["status"] = "success";
                        acceptedJSON["message"] = "Usernames processed successfully.";
                        return crow::response(200, acceptedJSON);

                    break;
                    case 400:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "User doesen't exist";
                        return crow::response(400, acceptedJSON);

                    break;

                    case 401:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "User isn't a admin"; 
                        return crow::response(401, acceptedJSON);

                    break;
                    case 403:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "Acceptable admin user, but bad password."; 
                        return crow::response(401, acceptedJSON);

                    break;

                    case 423:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "User account is locked"; 
                        return crow::response(423, acceptedJSON);

                    break;

                    default:
                        acceptedJSON["status"] = "error";
                        acceptedJSON["message"] = "No criteria met, refer to code to structure your errors";
                        return crow::response(500, acceptedJSON);
                    break;
                }
            } catch(const std::exception& ex){

                crow::json::wvalue badJSON;
                badJSON["status"] = "error";
                badJSON["message"] = "Rare Error- something is wrong with the main code";
                 return crow::response(500, badJSON);
            }
            
        } 

    });
    crowSimple.port(5023).multithreaded().run();

  
    std::cout << "\nCtrl-C pressed, ending program.";
    return 0;
}
int main()
{
    std::string superSecret = "Puppies";
    std::cout << "\n\n ----Starting Program---- \n\n";
    int testShot = programRun(superSecret);
}