#include <vector>
#include <string>
#include "untrusted_util.h"
using namespace std;
bool is_base64(unsigned char c);
vector<string> splitString(const char *str, char c);
string base64_decode(string const &encoded_string);
void base64_decode_image(const string &input);

string textSearch(vector<std::string> query, secure_connection *conn,uint8_t* output);
string imageSearchADP(std::string imagePath, secure_connection *conn, uint8_t *output);
void save_file(const string &input,const string &type,const string &name);