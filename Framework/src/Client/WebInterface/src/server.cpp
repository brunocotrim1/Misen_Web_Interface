#include <vector>
#include <httplib.h>
#include <string>
#include <server.h>
#include <iostream>
#include <fstream>

#include "bisen_tests.h"


using namespace httplib;
using namespace std;
using std::vector;
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

int main(void)
{

    Server svr;
    svr.Post("/api/search", [](const Request &req, Response &res)
             {
                secure_connection *conn;
                untrusted_util::init_secure_connection(&conn, "101.101.2.5", 305);//valores de exemplo
                 

                //Tratar de fazer a conexão apenas quando os casos são verificados o useja existir query ou imagem ou ambos 
                //Senao mandar bad request 
                //

                 string emptyBod = "none";
                 if (req.body != emptyBod)
                 {

                     char *cstr = new char[req.body.length() + 1];
                     strcpy(cstr, req.body.c_str());
                     vector<string> splitedString = splitString(cstr, ',');
                     delete[] cstr;
                     base64_decode_image(splitedString.at(1));
                    //  if (remove("../images/temporary.png") != 0)//remover o ficheiro no final da execução do código 
                    //      perror("File deletion failed");
                 }if (req.has_param("query"))
                 {
                     auto query = req.get_param_value("query");
                     auto image_file = req.get_file_value("image_file");

                     string Response = "Message Processed on LocalServer | Query Received: " + query;
                     res.set_content(Response, "text/plain");
                 }
                 else
                     res.status = 404;

                untrusted_util::close_secure_connection(conn);
             });

    auto ret = svr.set_mount_point("/", "../www");
    if (!ret)
    {
        std::cout << "Error" << std::endl;
    }
    svr.listen("localhost", 8080);
}

string textSearch(vector<std::string> query, secure_connection *conn)
{
    SseClient client;
    string response;
    bisen_setup(conn, &client);
    bisen_search(conn, &client, query);//Fazer MEMCOPY para um parametro novo que é a response

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
    ofstream outfile;
    outfile.open("../images/temporary.png", ofstream::binary);

    string temp = base64_decode(input);

    outfile.write(temp.c_str(), temp.size());
    outfile.close();
    cout << "file saved" << endl;
}