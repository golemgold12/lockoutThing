#include <iostream>
#include <curl/curl.h>
#include <string>

// g++ crummytest.cpp -o crummytest -I./vcpkg/installed/x64-mingw-static/include -L./vcpkg/installed/x64-mingw-static/lib -lcurl -lws2_32 -lbcrypt -lz -lcrypt32 -lws2_32 -lwinmm -luser32 -lgdi32 -ladvapi32

void startItUp()
{
    std::cout << "Starting test program.";
    std::cout << "\nSee canvas for help with 1st user and pass.";
}
void epicFail()
{
    std::cout << "We got no response- ensure the server is set up....";
}

size_t writeCallback(void *contents, size_t size, size_t newMember, std::string *out)
{
    size_t totalSize = size * newMember;
    out->append((char *)contents, totalSize);
    std::cout << "Callback written to us.";
    return totalSize;
}

int main()
{

    startItUp();
    CURL *curl;
    CURLcode res;
    bool itWorked = false;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl)
    {


        const std::string json_data = R"(["Puppies","superAdmin199", "testAdmin30!", "Nolan80", "MackTruck12"])"; // your first and secound string should be a admin username and password

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5023/LogProcess");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // For response
        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *contents, size_t size, size_t nmemb, std::string *out)
                         {
            size_t totalSize = size * nmemb;
            out->append((char *)contents, totalSize);
            return totalSize; });
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "\nOur curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }
        else
        {
            std::cout << "\n We got: " << response_string << std::endl;
            itWorked = true;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    if (itWorked == false)
    {

        epicFail();
    }
    curl_global_cleanup();
    return 0;
}
