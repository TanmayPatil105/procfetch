#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include "color.h"
using namespace std;

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

    return line;
}

string getHardwarePlatform()
{
    string s = exec("uname -m");
    s = s.substr(0, s.find("\n"));
    return " " + s;
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

    string host = n1 + " " + n2;

    return host;
}

string getKernel(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string kernel;
    getline(fptr, kernel);
    return kernel;
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

string getRAM(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string line, sub, shmem;
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
        }
        if (sub == "Buffers")
        {
            shmem = line;
            break;
        }
    }
    int i;
    for (i = 0; i < total.size(); i++)
    {
        if (isdigit(total[i]))
        {
            break;
        }
    }
    total = total.substr(i);
    total = total.substr(0, total.find(" "));
    for (i = 0; i < free.size(); i++)
    {
        if (isdigit(free[i]))
        {
            break;
        }
    }
    free = free.substr(i);
    free = free.substr(0, free.find(" "));

    for (i = 0; i < shmem.size(); i++)
    {
        if (isdigit(shmem[i]))
        {
            break;
        }
    }

    shmem = shmem.substr(i);
    shmem = shmem.substr(0, shmem.find(" "));

    int memTotal = stoi(total);
    int memFree = stoi(free);
    int memAvail = (memTotal - memFree) - stoi(shmem);

    return to_string(memAvail / 1024) + "MiB / " + to_string(memTotal / 1024) + "MiB";
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

int getCPUtemp(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string temp;
    getline(fptr, temp);
    return stoi(temp);
}

vector<string> getGPU()
{
    vector<string> gpu;
    string igpu = exec("lspci | grep -E  \"VGA|3D|Display\"");
    int temp = 0, k = 0;

    for (int i = 0; i < igpu.size(); i++)
    {
        if (igpu[i] == '\n')
        {
            gpu.push_back(igpu.substr(temp, i - temp));
            gpu[k] = gpu[k].substr(gpu[k].find(": ") + 2);
            gpu[k] = gpu[k].substr(0, gpu[k].find(" ("));
            temp = i + 1;
            k++;
        }
    }
    return gpu;
}

string getPackages()
{
    string pkg = "";
    int count = 0;
    if (exec(" [ -f \"/bin/dpkg\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string dpkg = exec(" dpkg -l | wc -l ");
        pkg += dpkg.substr(0, dpkg.size() - 1) + RED + " dpkg; " + RESET;
    }
    if (exec(" [ -f \"/bin/snap\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string snap = exec(" snap list | wc -l ");
        pkg += snap.substr(0, snap.size() - 1) + RED + " snap; " + RESET;
    }
    if (exec(" [ -f \"/bin/pacman\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string pacman = exec(" pacman -Q | wc -l  ");
        pkg += pacman.substr(0, pacman.size() - 1) + RED + " pacman; " + RESET;
    }
    if (exec(" [ -f \"/bin/flatpak\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string flatpak = exec(" flatpak list | wc -l ");
        pkg += flatpak.substr(0, flatpak.size() - 1) + RED + " flatpak; " + RESET;
    }
    if (exec(" [ -f \"/var/lib/rpm\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string rpm = exec(" rpm -qa | wc -l ");
        pkg += rpm.substr(0, rpm.size() - 1) + RED + " rpm; " + RESET;
    }
    if (exec(" [ -f \"/bin/npm\" ] && echo \"1\"|wc -l  ").size() > 1)
    {
        string npm = exec(" npm list | wc -l ");
        pkg += npm.substr(0, npm.size() - 1) + RED + " npm; " + RESET;
    }
    if (exec(" [ -f \"/bin/emerge\" ] && echo \"1\"|wc -l  ").size() > 1) // gentoo
    {
        string portage = exec("echo -n $(cd /var/db/pkg && ls -d */* | wc -l");
        pkg += portage.substr(0, portage.size() - 1) + RED + " portage; " + RESET;
    }
    if (exec(" [ -f \"/bin/xbps-install\" ] && echo \"1\"|wc -l  ").size() > 1) // void linux
    {
        string xbps = exec(" flatpak list | wc -l ");
        pkg += xbps.substr(0, xbps.size() - 1) + RED + " xbps; " + RESET;
    }
    if (exec(" [ -f \"/bin/dnf\" ] && echo \"1\"|wc -l  ").size() > 1) // fedora
    {
        string dnf = exec(" dnf list installed| wc -l ");
        pkg += dnf.substr(0, dnf.size() - 1) + RED + " dnf; " + RESET;
    }
    if (exec(" [ -f \"/bin/yum\" ] && echo \"1\"|wc -l  ").size() > 1) // redhat
    {
        string yum = exec(" yum list installed | wc -l ");
        pkg += yum.substr(0, yum.size() - 1) + RED + " yum; " + RESET;
    }
    if (exec(" [ -f \"/bin/zypper\" ] && echo \"1\"|wc -l  ").size() > 1) // opensuse
    {
        string zypper = exec(" zypper se --installed-only | wc -l ");
        pkg += zypper.substr(0, zypper.size() - 1) + RED + " zypper; " + RESET;
    }

    return pkg;
}

string getColor(string line)
{
    string color;
    if (line.substr(0, line.find(" ")) == "RED")
    {
        color = RED;
    }
    else if (line.substr(0, line.find(" ")) == "BLACK")
    {
        color = BLACK;
    }
    else if (line.substr(0, line.find(" ")) == "GREEN")
    {
        color = GREEN;
    }
    else if (line.substr(0, line.find(" ")) == "YELLOW")
    {
        color = YELLOW;
    }
    else if (line.substr(0, line.find(" ")) == "BLUE")
    {
        color = BLUE;
    }
    else if (line.substr(0, line.find(" ")) == "MAGENTA")
    {
        color = MAGENTA;
    }
    else if (line.substr(0, line.find(" ")) == "CYAN")
    {
        color = CYAN;
    }
    else if (line.substr(0, line.find(" ")) == "WHITE")
    {
        color = WHITE;
    }
    else if (line.substr(0, line.find(" ")) == "BBLACK")
    {
        color = BBLACK;
    }
    else if (line.substr(0, line.find(" ")) == "BGRAY")
    {
        color = BGRAY;
    }
    else if (line.substr(0, line.find(" ")) == "BRED")
    {
        color = BRED;
    }
    else if (line.substr(0, line.find(" ")) == "BGREEN")
    {
        color = BGREEN;
    }
    else if (line.substr(0, line.find(" ")) == "BYELLOW")
    {
        color = BYELLOW;
    }
    else if (line.substr(0, line.find(" ")) == "BBLUE")
    {
        color = BBLUE;
    }
    else if (line.substr(0, line.find(" ")) == "BMAGENTA")
    {
        color = BMAGENTA;
    }
    else if (line.substr(0, line.find(" ")) == "BCYAN")
    {
        color = BCYAN;
    }
    else if (line.substr(0, line.find(" ")) == "BWHITE")
    {
        color = BWHITE;
    }

    return color;
}

void print_process(string art)
{
    string color;
    string path="/usr/share/procfetch/ascii/" + art;
    fstream fptr;
    fptr.open(path, ios::in);
    string txt;
    getline(fptr,txt);
    color=getColor(txt);
    cout<<color<<endl;
    while(fptr)
    {
        getline(fptr,txt);
        cout<<BRIGHT<<color<<txt<<endl;
    }
    fptr.close();
}

void print()
{
    string os = getOS("/etc/os-release");

    map<string, string> ascii_arts;

    ascii_arts["Ubuntu"] = {"ubuntu.ascii"};
    ascii_arts["Debian"] = {"debian.ascii"};
    ascii_arts["Fedora"] = {"fedora.ascii"};
    ascii_arts["Red Hat"] = {"redhat.ascii"};
    ascii_arts["Arch Linux"] = {"arch.ascii"};
    ascii_arts["Manjaro"] = {"manjaro.ascii"};
    ascii_arts["Archcraft"] = {"archcraft.ascii"};
    ascii_arts["Kali"] = {"kali.ascii"};
    ascii_arts["Parrot"] = {"parrot.ascii"};
    ascii_arts["OpenSuse"] = {"opensuse.ascii"};
    ascii_arts["Linux Mint"] = {"linuxmint.ascii"};
    ascii_arts["EndeavourOS"] = {"endeavouros.ascii"};

    for(auto it=ascii_arts.begin(); it!=ascii_arts.end(); it++)
    {
        if(os.find(it->first) != string::npos)
        {
            print_process(it->second);
            return;
        }
    }
}

int main()
{
    print();
    string user = getuser();
    string hostname = gethostname("/etc/hostname");
    string username = YELLOW + user + RESET + "@" + YELLOW + hostname;
    cout << UNDERSCORE << username << RESET << endl;
    cout << endl;
    string os = getOS("/etc/os-release");
    cout << BRIGHT << GREEN << "OS : " << RESET << os << getHardwarePlatform() << endl;
    string HOST = getHost("/sys/devices/virtual/dmi/id/");
    cout << BRIGHT << GREEN << "Host : " << RESET << HOST << endl;
    string kernel = getKernel("/proc/sys/kernel/osrelease");
    cout << BRIGHT << GREEN << "Kernel : " << RESET << kernel << endl;
    string upTime = getUpTime("/proc/uptime");
    cout << BRIGHT << GREEN << "UpTime : " << RESET << upTime << endl;
    string ram = getRAM("/proc/meminfo");
    cout << BRIGHT << GREEN << "RAM : " << RESET << ram << endl;
    string shell = getSHELL("/etc/passwd");
    cout << BRIGHT << GREEN << "shell : " << RESET << shell << endl;
    string DE = getDE();
    cout << BRIGHT << GREEN << "DE : " << RESET << DE << endl;
    string res = getRES("/sys/class/graphics/fb0/modes");
    cout << BRIGHT << GREEN << "Resolution : " << RESET << res << endl;
    string theme = getTheme();
    cout << BRIGHT << GREEN << "Theme : " << RESET << theme << endl;
    string icon = getIcons();
    cout << BRIGHT << GREEN << "Icons : " << RESET << icon << endl;
    string cpu = getCpu("/proc/cpuinfo");
    cout << BRIGHT << GREEN << "CPU : " << RESET << cpu << endl;
    int temp;
    if (HOST.find("VirtualBox") == string::npos)
    {
        int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
        cout << BRIGHT << GREEN << "CPU Temperature : " << RESET << float(temp / 1000.0) << " °C" << endl;
    }
    vector<string> gpu = getGPU();
    for (auto it : gpu)
    {
        cout << BRIGHT << GREEN "GPU : " << RESET << it << endl;
    }
    string pkg = getPackages();
    cout << BRIGHT << GREEN << "Packages : " << RESET << pkg << endl;
    cout << endl;
    return 0;
}
