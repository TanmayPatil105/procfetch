#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string getuser()
{
	return getenv("USER");
}

string gethostname(string path)
{
	fstream fptr;
	fptr.open(path, ios::in);
	string hostname;
	getline(fptr, hostname);
	return hostname;
}

string get_os_name()
{
	ifstream fin;
	fin.open("/etc/os-release");
	string os;
	getline(fin, os);
	return os.substr(13, os.length() - 14);
}

int main()
{
	string user = getuser();
	string hostname = gethostname("/etc/hostname");
	cout << user << "@" << hostname << endl;
	string os = get_os_name();
	cout << os;
}
