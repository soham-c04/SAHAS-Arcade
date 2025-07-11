#include <iostream>
#include <vector>
using namespace std;

int connect_to_server(string server_ip, int port_no);
void sendData(vector<int> send_);
pair<vector<int>,vector<string>> receiveData();
void closeConnection();
