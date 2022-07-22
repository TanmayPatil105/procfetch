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

string getUpTime(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
	string time;

	getline(fptr, time);
    time=time.substr(0, time.find(" "));

    int m = stoi(time)/60;
    int h = m/60;
    int d=h/24;

    string timeS;
    if(h==0)
    {
        timeS = to_string(m%60)+" mins";
    }
    else if(d==0)
    {
        timeS = to_string(h%24) + " hours, " + to_string(m%60)+" mins";
    }
    else
    {
        timeS = to_string(d) + " d, " + to_string(h%24) + " hours, " + to_string(m%60)+" mins";
    }
    
    return timeS;
}

string getCpu(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string cpu,line,sub;

    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,10);
        if(sub=="model name")
        {
            break;
        }
    }
    cpu=line.substr(line.find(":")+2);

    return cpu;
}

int main()
{
	string user = getuser();
	string hostname = gethostname("/etc/hostname");
	cout << user << "@" << hostname << endl;
	string upTime = getUpTime("/proc/uptime");
    cout<<upTime<<endl;
    string cpu = getCpu("/proc/cpuinfo");
    cout<<cpu<<endl;
}