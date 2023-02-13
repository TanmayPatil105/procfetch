/**
 * @file
 */
#include "fetch.h"
string Context::PACKAGE_DELIM = "; "s;

/**
 * @returns gets the username
 */
string getuser()
{
    return getenv("USER");
}

/**
 * @returns gets the hostname
 * @param path
 */
string gethostname(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string hostname;
    getline(fptr, hostname);
    return hostname;
}

/**
 * @returns gets name of Operating System
 * @param path
 */
string getOS(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string os_name, variable_name, quote;

    while (fptr)
    {
        getline(fptr, os_name);
        variable_name = os_name.substr(0, 13);
        if (variable_name == "PRETTY_NAME=\"")
        {
            quote = "\"";
            break;
        }
        if (variable_name == "PRETTY_NAME=\'")
        {
            quote = "\'";
            break;
        }
    }

    os_name = os_name.substr(os_name.find(quote) + 1);
    os_name = os_name.substr(0, os_name.find(quote));

    return os_name;
}

/**
 * @returns gets HardWare Platform
 */
string getHardwarePlatform()
{
    string s = Command::exec("uname -m"s).getOutput();
    s = s.substr(0, s.find("\n"));
    return " " + s;
}

/**
 * @returns gets Host
 * @param path
 */
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

/**
 * @returns gets kernel
 * @param path
 */
string getKernel(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string kernel;
    getline(fptr, kernel);
    return kernel;
}

/**
 * @returns get Uptime
 * @param path
 */
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
        timeS = to_string(d) + " d, " + to_string(h % 24) + " hours, " +
                to_string(m % 60) + " mins";
    }

    return timeS;
}

/**
 * @returns gets RAM usage details
 * @param path
 */
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
    size_t i;
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

    return to_string(memAvail / 1024) + "MiB / " + to_string(memTotal / 1024) +
           "MiB";
}

/**
 * @returns gets type of shell
 * @param path
 */
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

/**
 * @returns gets the Desktop Environment
 */
string getDE()
{
    return getenv("XDG_CURRENT_DESKTOP");
}

/**
 * @returns gets current Screen Resolution
 * @param path
 */
string getRES(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string res;
    getline(fptr, res);
    res = res.substr(2);
    return res.substr(0, res.find("p"));
}

/**
 * @returns gets current Theme info
 */
string getTheme()
{
    auto c =
        Command::exec("gsettings get org.gnome.desktop.interface gtk-theme"s);
    auto s = c.getOutput();
    return s.substr(1, s.find("\'", 1) - 1);
}

/**
 * @returns gets current Icon info
 */
string getIcons()
{
    auto c =
        Command::exec("gsettings get org.gnome.desktop.interface icon-theme"s);
    auto s = c.getOutput();
    return s.substr(1, s.find("\'", 1) - 1);
}

/**
 * @returns gets CPU info
 * @param path
 */
string getCPU(string path)
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

/**
 * @returns gets CPU temp
 * @param path
 */
int getCPUtemp(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string temp;
    getline(fptr, temp);
    return stoi(temp);
}

/**
 * @returns checks for CPUtemp file
 */
bool CpuTempCheck()
{
    if (Path::of("/sys/class/thermal/thermal_zone1"s).isDirectory())
    {
        return true;
    }
    return false;
}

/**
 * @returns gets vendor of Internal and External GPU
 */
vector<string> getGPU()
{
    vector<string> gpu;
    auto c = Command::exec("lspci | grep -E  \"VGA|3D|Display\"");
    string igpu = c.getOutput();
    int temp = 0, k = 0;

    for (size_t i = 0; i < igpu.size(); i++)
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

/**
 * @returns gets count of all packages installed
 */
string getPackages()
{
    struct rec
    {
        string name;
        int count; // -1: not supported
    };
    vector<rec> pkgs;

    if (Path::of("/bin/dpkg"s).isExecutable())
    {
        auto c = Command::exec("dpkg -l"s);
        pkgs.push_back(rec{"dpkg"s, c.getOutputLines()});
    }
    if (Path::of("/bin/snap"s).isExecutable())
    {
        auto c = Command::exec("snap list"s);
        pkgs.push_back(rec{"snap"s, c.getOutputLines()});
    }
    if (Path::of("/bin/pacman"s).isExecutable())
    {
        auto c = Command::exec("pacman -Q"s);
        pkgs.push_back(rec{"pacman"s, c.getOutputLines()});
    }
    if (Path::of("/bin/flatpak"s).isExecutable())
    {
        auto c = Command::exec("flatpak list"s);
        pkgs.push_back(rec{"flatpak"s, c.getOutputLines()});
    }
    if (Path::of("/var/lib/rpm"s).isExecutable())
    {
        auto c = Command::exec("rpm -qa"s);
        pkgs.push_back(rec{"rpm"s, c.getOutputLines()});
    }
    if (Path::of("/bin/npm"s).isExecutable())
    {
        auto c = Command::exec("npm list"s);
        pkgs.push_back(rec{"npm"s, c.getOutputLines()});
    }
    if (Path::of("/bin/emerge"s).isExecutable()) // gentoo
    {
        pkgs.push_back(rec{"portage"s, -1});
    }
    if (Path::of("/bin/xbps-install"s).isExecutable()) // void linux
    {
        auto c = Command::exec("flatpak list"s);
        pkgs.push_back(rec{"xbps"s, c.getOutputLines()});
    }
    if (Path::of("/bin/dnf"s).isExecutable()) // fedora
    {
        auto c = Command::exec("dnf list installed"s);
        pkgs.push_back(rec{"dnf"s, c.getOutputLines()});
    }
    if (Path::of("/bin/zypper"s).isExecutable()) // opensuse
    {
        auto c = Command::exec("zypper se --installed-only"s);
        pkgs.push_back(rec{"zypper"s, c.getOutputLines()});
    }
    if (Path::of("/home/linuxbrew/.linuxbrew/bin/brew"s).isExecutable())
    {
        auto c = Command::exec("brew list | { tr '' '\n'; }"s);
        pkgs.push_back(rec{"brew"s, c.getOutputLines()});
    }

    auto red = Crayon{}.red();
    auto pkg = ""s;
    for (auto p : pkgs)
    {
        if (p.count < 0)
        {
            pkg +=
                "not supported "s + red.text(p.name) + Context::PACKAGE_DELIM;
            continue;
        }
        pkg += to_string(p.count) + " "s + red.text(p.name) +
               Context::PACKAGE_DELIM;
    }

    return pkg;
}

/**
 * @param art
 * @param color_name
 */
void print_process(string art, string color_name)
{
    string path = "/usr/share/procfetch/ascii/" + art;
    fstream fptr;
    fptr.open(path, ios::in);
    string txt;
    getline(fptr, txt);
    auto style = Crayon{}.bright();
    if (color_name == "def")
    {
        style = style.color(txt.substr(0, txt.find(" ")));
    }
    else
    {
        transform(color_name.begin(), color_name.end(), color_name.begin(),
                  ::toupper);
        style = style.color(color_name.substr(0, color_name.find(" ")));
    }
    cout << style.text(""s) << endl;
    while (fptr)
    {
        getline(fptr, txt);
        cout << style.text(txt) << endl;
    }
    fptr.close();
}

/**
 * Utility to print ascii art of Distro
 * @param color_name
 * @param distro_name
 */
void print(string color_name, string distro_name)
{
    string os = distro_name;

    if (distro_name == "def")
    {
        os = getOS("/etc/os-release");
    }

    map<string, string> ascii_arts = {{"Ubuntu", "ubuntu.ascii"},
                                      {"Debian", "debian.ascii"},
                                      {"Fedora", "fedora.ascii"},
                                      {"Red Hat", "redhat.ascii"},
                                      {"Arch Linux", "arch.ascii"},
                                      {"Manjaro", "manjaro.ascii"},
                                      {"Archcraft", "archcraft.ascii"},
                                      {"Kali", "kali.ascii"},
                                      {"Parrot", "parrot.ascii"},
                                      {"OpenSuse", "opensuse.ascii"},
                                      {"Linux Mint", "linuxmint.ascii"},
                                      {"EndeavourOS", "endeavouros.ascii"},
                                      {"Pop!_OS", "pop!_os.ascii"},
                                      {"Gentoo", "gentoo.ascii"},
                                      {"elementary OS", "elementaryos.ascii"},
                                      {"Slackware", "slackware.ascii"},
                                      {"Asahi Linux", "asahi.ascii"},
                                      {"Peppermint", "peppermintos.ascii"},
                                      {"CentOS", "centos.ascii"},
                                      {"Lubuntu", "lubuntu.ascii"},
                                      {"Navy Linux", "navylinux.ascii"},
                                      {"BlackArch", "blackarch.ascii"},
                                      {"SteamOS", "steamos.ascii"},
                                      {"MX", "mxlinux.ascii"},
                                      {"Linux Lite", "linuxlite.ascii"},
                                      {"Bodhi", "bodhilinux.ascii"},
                                      {"Xubuntu", "xubuntu.ascii"},
                                      {"Kubuntu", "kubuntu.ascii"},
                                      {"Rocky", "rocky.ascii"},
                                      {"Deepin", "deepin.ascii"},
                                      {"Zorin", "zorin.ascii"},
                                      {"Garuda", "garudalinux.ascii"}};

    for (const auto &[key, value] : ascii_arts)
    {
        if (os.find(key) != string::npos)
        {
            print_process(value, color_name);
            return;
        }
    }

    print_process("linux.ascii", color_name);

    return;
}
