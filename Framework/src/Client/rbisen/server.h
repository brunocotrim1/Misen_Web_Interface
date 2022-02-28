#include <vector>
#include <string>
using namespace std;
bool is_base64(unsigned char c);
vector<string> splitString(const char *str, char c);
string base64_decode(string const &encoded_string);
void base64_decode_image(const string &input);