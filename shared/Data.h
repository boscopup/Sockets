#include <string>
#include <sstream>

using namespace std;

class Data {
private:
    string serverName;
    int health;

public:
    Data(string name);
    void setHealth(int h);
    int getHealth();
    string getServerName();
    string serialize();
    void deserialize(string bufferString);
};

Data::Data(string name) {
    serverName = name;
    health = 100;
}

void Data::setHealth(int h) {
    health = h;
}

int Data::getHealth() {
    return health;
}

string Data::getServerName() {
    return serverName;
}

string Data::serialize()
{
    stringstream s;
    s << serverName << ":" << health;
    return s.str();
}

void Data::deserialize(string bufferString)
{
    int index = bufferString.find(':');
    serverName = bufferString.substr(0, index);
    health = stoi(bufferString.substr(index+1));
}