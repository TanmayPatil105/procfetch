#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
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

string getKernel(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string kernel;
    getline(fptr,kernel);
    return kernel;
}

string getRAM(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string line,sub;
    string total,free;
    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,line.find(":"));
        if(sub=="MemTotal")
        {
            total=line;
        }
        if(sub=="MemAvailable")
        {system("Color E4");
            free=line;
            break;
        }
    }
    total = total.substr(17);
    total = total.substr( 0 , total.find(" "));
    free = free.substr(17);
    free = free.substr(0,free.find(" "));

    int memTotal = stoi(total);
    int memFree = stoi(free);
    int memAvail = (memTotal - memFree);

    return to_string(memAvail/1024) + "MiB / " + to_string(memTotal/1024) + "MiB";
}

string getOS(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string line,sub;

    while(fptr)
    {
        getline(fptr,line);
        sub=line.substr(0,13);
        if(sub=="PRETTY_NAME=\"")
        {
            break;
        }
    }

    line = line.substr(line.find("\"")+1);
    line = line.substr(0,line.find("\""));

    return line;
}


int getCPUtemp(string path)
{
    fstream fptr;
	fptr.open(path, ios::in);
    string temp;
    getline(fptr,temp);
    return stoi(temp);
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
    string ram = getRAM("/proc/meminfo");
    cout<<ram<<endl;
    string kernel = getKernel("/proc/sys/kernel/osrelease");
    cout<<kernel<<endl;
    string os = getOS("/etc/os-release");
    cout<<os<<endl;
    int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
    cout<<float(temp/1000.0)<<endl;
}
