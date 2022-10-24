#include "fetch.h"
#include "color.h"

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
    string s = Command::exec("uname -m"s).getOutput();
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
    auto c = Command::exec("gsettings get org.gnome.desktop.interface gtk-theme"s);
    auto s = c.getOutput();
    return s.substr(1, s.find("\'", 1) - 1);
}

string getIcons()
{
    auto c = Command::exec("gsettings get org.gnome.desktop.interface icon-theme"s);
    auto s = c.getOutput();
    return s.substr(1, s.find("\'", 1) - 1);
}

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

int getCPUtemp(string path)
{
    fstream fptr;
    fptr.open(path, ios::in);
    string temp;
    getline(fptr, temp);
    return stoi(temp);
}

bool CpuTempCheck()
{
    if (Path::of("/sys/class/thermal/thermal_zone1"s).is_directory())
    {
        return true;
    }
    return false;
}

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

string getPackages()
{
    string pkg = "";
    if (Path::of("/bin/dpkg"s).is_regular_fie())
    {
        auto c = Command::exec("dpkg -l"s);
        pkg += to_string(c.getOutputLines()) + RED + " dpkg; " + RESET;
    }
    if (Path::of("/bin/snap"s).is_regular_fie())
    {
        auto c = Command::exec("snap list"s);
        pkg += to_string(c.getOutputLines()) + RED + " snap; " + RESET;
    }
    if (Path::of("/bin/pacman"s).is_regular_fie())
    {
        auto c = Command::exec("pacman -Q"s);
        pkg += to_string(c.getOutputLines()) + RED + " pacman; " + RESET;
    }
    if (Path::of("/bin/flatpak"s).is_regular_fie())
    {
        auto c = Command::exec("flatpak list"s);
        pkg += to_string(c.getOutputLines()) + RED + " flatpak; " + RESET;
    }
    if (Path::of("/var/lib/rpm"s).is_regular_fie())
    {
        auto c = Command::exec("rpm -qa"s);
        pkg += to_string(c.getOutputLines()) + RED + " rpm; " + RESET;
    }
    if (Path::of("/bin/npm"s).is_regular_fie())
    {
        auto c = Command::exec("npm list"s);
        pkg += to_string(c.getOutputLines()) + RED + " npm; " + RESET;
    }
    if (Path::of("/bin/emerge"s).is_regular_fie()) // gentoo
    {
        pkg += "not supported"s + RED + " portage; " + RESET;
    }
    if (Path::of("/bin/xbps-install"s).is_regular_fie()) // void linux
    {
        auto c = Command::exec("flatpak list"s);
        pkg += to_string(c.getOutputLines()) + RED + " xbps; " + RESET;
    }
    if (Path::of("/bin/dnf"s).is_regular_fie()) // fedora
    {
        auto c = Command::exec("dnf list installed"s);
        pkg += to_string(c.getOutputLines()) + RED + " dnf; " + RESET;
    }
    if (Path::of("/bin/zypper"s).is_regular_fie()) // opensuse
    {
        auto c = Command::exec("zypper se --installed-only"s);
        pkg += to_string(c.getOutputLines()) + RED + " zypper; " + RESET;
    }

    return pkg;
}

void print_process(string art)
{
    string color;
    string path = "/usr/share/procfetch/ascii/" + art;
    fstream fptr;
    fptr.open(path, ios::in);
    string txt;
    getline(fptr, txt);
    color = getColor(txt);
    cout << color << endl;
    while (fptr)
    {
        getline(fptr, txt);
        cout << BRIGHT << color << txt << endl;
    }
    fptr.close();
}

void print()
{
    string os = getOS("/etc/os-release");

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
                                      {"Rocky", "rockylinux.ascii"},
                                      {"Deepin", "deepin.ascii"},
                                      {"Zorin", "zorin.ascii"},
                                      {"Garuda", "garudalinux.ascii"}};

    for (const auto &[key, value] : ascii_arts)
    {
        if (os.find(key) != string::npos)
        {
            print_process(value);
            return;
        }
    }
}
