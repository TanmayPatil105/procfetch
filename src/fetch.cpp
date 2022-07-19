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
	fptr.open(path,ios::in);
	string hostname;
	getline(fptr,hostname );
	return hostname;
}

int main()
{
	string user = getuser();
	string hostname = gethostname("/etc/hostname");
	cout<<user<<"@"<<hostname<<endl;
}
