#include <vector>
#include <httplib.h>
#include <string>
#include <server.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "visen_tests.h"
#include "ImageSearch.h"
#include "bisen_tests.h"
#include "untrusted_util.h"

using namespace httplib;
using namespace std;
using std::vector;
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

string imagePath = "../images/temporary.png";
secure_connection *conn;
SseClient client;
feature_extractor desc(1, 500);

int main(void)
{
    untrusted_util::init_secure_connection(&conn, "localhost", 7910);
    Server svr;
    bisen_setup(conn, &client);
    string trainType = "client_kmeans";
    visen_setup(conn, desc.get_desc_len(), 5000, trainType.c_str());
    svr.Post("/api/update", [](const Request &req, Response &res)
             {
                 uint8_t output = 0;
                 std::cout << req.body << endl;

                 string fileType = req.get_param_value("file_type");
                 string name = req.get_param_value("name");
                 save_file(req.body, fileType, name);
                 string fileName = "../update/" + name + "." + fileType;
                 vector<string> queryVec(1);
                 queryVec[0] = fileName;
                 if(fileType == "png" || fileType == "jpeg" || fileType == "jpg"){
                 visen_add_files(conn, desc, queryVec);
                 search_flickr(conn, desc, queryVec, 1, &output);
                 }
                 else if(fileType == "txt"){
                 bisen_update(conn,&client, "wiki", 1, queryVec);
                 }  

             });

    svr.Post("/api/search", [](const Request &req, Response &res)
             {
                 uint8_t output = 0;
                 string emptyBod = "none";
                 string query = "";
                 if (req.has_param("query"))
                     query = req.get_param_value("query");

                 if (req.body != emptyBod)
                 {
                     char *cstr = new char[req.body.length() + 1];
                     strcpy(cstr, req.body.c_str());
                     vector<string> splitedString = splitString(cstr, ',');
                     delete[] cstr;
                     base64_decode_image(splitedString.at(1));
                     imageSearchADP(imagePath, conn, &output);
                     string string_name = to_string(output);
                     cout << string_name << endl;
                     string Response = "Message Processed on LocalServer | Query Received: " + string_name;
                     res.set_content(Response, "text/plain");

                 }
                 if (!query.empty())
                 {
                     printf("ENTROU");
                     vector<string> queryVec(1);
                     queryVec[0] = query;
                     printf("%s\n", queryVec[0].c_str());
                     textSearch(queryVec, conn, &output);
                     string Response = "Message Processed on LocalServer | Query Received: " + query;
                     res.set_content(Response, "text/plain");
                 }
             });

    auto ret = svr.set_mount_point("/", "../www");
    if (!ret)
    {
        std::cout << "Error" << std::endl;
    }
    svr.listen("10.101.87.247", 8080);
}

void save_file(const string &input, const string &type, const string &name)
{
    ofstream outfile;
    string fileName = "../update/" + name + "." + type;
    outfile.open(fileName.c_str(), ofstream::binary);

    outfile.write(input.c_str(), input.size());
    outfile.close();
    cout << "file saved" << endl;
}

string textSearch(vector<std::string> query, secure_connection *conn, uint8_t *output)
{
    string response = "Tratar de resposta";
    bisen_search(conn, &client, query, output);
    printf("%d\n", *output);
    return response;
}

string imageSearchADP(std::string imagePath, secure_connection *conn, uint8_t *output)
{
    vector<string> files_search(1);
    files_search[0] = imagePath;
    search_flickr(conn, desc, files_search, 1, output);
    string response = "Tratar de resposta";
    return response;
}

vector<std::string> splitString(const char *str, char c)
{
    vector<std::string> result;

    do
    {
        const char *begin = str;

        while (*str != c && *str)
            str++;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}

bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

void base64_decode_image(const string &input)
{
    auto myid = this_thread::get_id();
    stringstream ss;
    ss << myid;
    string idStr = ss.str();
    ofstream outfile;
    
    outfile.open("../images/temporary.png", ofstream::binary);

    string temp = base64_decode(input);

    outfile.write(temp.c_str(), temp.size());
    outfile.close();
    cout << "file saved" << endl;
}
