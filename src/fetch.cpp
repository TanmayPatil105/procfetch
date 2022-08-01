#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <bits/stdc++.h>
#include "color.h"
using namespace std;

string getuser()
{
    return getenv("USER");
}

string exec(string command)
{
    char buffer[128];
    string result = "";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        return "popen failed!";
    }
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
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
    time = time.substr(0, time.find(" "));

    int m = stoi(time) / 60;
    int h = m / 60;
    int d = h / 24;

    string timeS;
    if (h == 0)
    {
        timeS = to_string(m % 60) + " mins";
    }
    else if (d == 0)
    {
        timeS = to_string(h % 24) + " hours, " + to_string(m % 60) + " mins";
    }
    else
    {
        timeS = to_string(d) + " d, " + to_string(h % 24) + " hours, " + to_string(m % 60) + " mins";
    }

    return timeS;
}

string getCpu(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string cpu, line, sub;

    while (fptr)
    {
        getline(fptr, line);
        sub = line.substr(0, 10);
        if (sub == "model name")
        {
            break;
        }
    }
    cpu = line.substr(line.find(":") + 2);

    return cpu;
}

string getHost(string path)
{
    fstream f1, f2;
    string p1, p2, n1, n2;
    p1 = path + "product_name";
    p2 = path + "product_version";

    f1.open(p1, ios::in);
    getline(f1, n1);
    f1.close();

    f2.open(p2, ios::in);
    getline(f2, n2);
    f2.close();

    string res = n1 + " " + n2;

    return res;
}

string getKernel(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string kernel;
    getline(fptr, kernel);
    return kernel;
}

string getRAM(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string line, sub;
    string total, free;
    while (fptr)
    {
        getline(fptr, line);
        sub = line.substr(0, line.find(":"));
        if (sub == "MemTotal")
        {
            total = line;
        }
        if (sub == "MemAvailable")
        {
            free = line;
            break;
        }
    }
    total = total.substr(17);
    total = total.substr(0, total.find(" "));
    free = free.substr(17);
    free = free.substr(0, free.find(" "));

    int memTotal = stoi(total);
    int memFree = stoi(free);
    int memAvail = (memTotal - memFree);

    return to_string(memAvail / 1024) + "MiB / " + to_string(memTotal / 1024) + "MiB";
}

string getOS(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string line, sub;

    while (fptr)
    {
        getline(fptr, line);
        sub = line.substr(0, 13);
        if (sub == "PRETTY_NAME=\"")
        {
            break;
        }
    }

    line = line.substr(line.find("\"") + 1);
    line = line.substr(0, line.find("\""));

    string s = exec("uname -i");

    string res = line + " " + s + "\x1b[A";

    return res;
}

string getSHELL(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string line, sub;
    while (fptr)
    {
        getline(fptr, line);
        sub = line.substr(0, line.find(":"));
        if (sub == getuser())
        {
            break;
        }
    }
    reverse(line.begin(), line.end());
    line = line.substr(0, line.find("/"));
    reverse(line.begin(), line.end());
    return line;
}

int getCPUtemp(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string temp;
    getline(fptr, temp);
    return stoi(temp);
}

string getDE()
{
    return getenv("XDG_CURRENT_DESKTOP");
}

string getRES(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string res;
    getline(fptr, res);
    res = res.substr(2);
    return res.substr(0, res.find("p"));
}

string getTheme()
{
    string theme = exec("gsettings get org.gnome.desktop.interface gtk-theme");
    theme = theme.substr(1);
    return theme.substr(0, theme.find("\'"));
}

string getIcons()
{
    string icon = exec(" gsettings get org.gnome.desktop.interface icon-theme");
    icon = icon.substr(1);
    return icon.substr(0, icon.find("\'"));
}   

string getTerminal()
{
    return getenv("PATH");
}

string getIgpu()
{
    string igpu = exec("lspci | grep \"VGA\"");
    igpu = igpu.substr(igpu.find(": ") + 2);
    return igpu;
}

string getEgpu()
{
    string egpu = exec("lspci | grep \"3D\"");
    egpu = egpu.substr(egpu.find(": ") + 2);
    return egpu;
}

void print()
{
    string os = getOS("/etc/os-release");

    if (os.find("Ubuntu") != string::npos)
    {
        string path = "../ascii/ubuntu.ascii";
        fstream fptr;
        fptr.open(path, ios::in);
        string txt;
        while (fptr)
        {
            getline(fptr, txt);
            cout << txt << endl;
        }
    }
}

int main()
{
    print();
    string user = getuser();
    string hostname = gethostname("/etc/hostname");
    cout << RED << user << RESET << "@" << RED << hostname << endl;
    for(int i = 0; i < hostname.size()*2; i++)
        cout << "_";
    cout <<  endl << endl;
    string host = getHost("/sys/devices/virtual/dmi/id/");
    cout << CYAN << "Host : " << RESET << host << endl;
    string upTime = getUpTime("/proc/uptime");
    cout << CYAN << "UpTime : " << RESET << upTime << endl;
    string cpu = getCpu("/proc/cpuinfo");
    cout << CYAN << "CPU : " << RESET << cpu << endl;
    string ram = getRAM("/proc/meminfo");
    cout << CYAN << "RAM : " << RESET << ram << endl;
    string kernel = getKernel("/proc/sys/kernel/osrelease");
    cout << CYAN << "Kernel : " << RESET << kernel << endl;
    string os = getOS("/etc/os-release");
    cout << CYAN << "OS : " << RESET << os << endl;
    int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
    cout << CYAN << "CPU Temperature : " << RESET << float(temp / 1000.0) << " °C" << endl;
    string shell = getSHELL("/etc/passwd");
    cout << CYAN << "shell : " << RESET << shell << endl;
    string DE = getDE();
    cout << CYAN << "DE : " << RESET << DE << endl;
    string res = getRES("/sys/class/graphics/fb0/modes");
    cout << CYAN << "Resolution : " << RESET << res << endl;
    string theme = getTheme();
    cout << CYAN << "Theme : " << RESET << theme << endl;
    string icon = getIcons();
    cout << CYAN << "Icons : " << RESET << icon << endl;
    // string igpu = getIgpu();
    // cout<<CYAN<<"GPU : "<<RESET<<igpu;
    // string egpu = getEgpu();
    // cout<<CYAN<<"GPU : "<<RESET<<egpu;
    cout << CYAN << endl;
}
