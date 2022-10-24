#include "color.h"
#include "fetch.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && !"-t"s.compare(argv[1]))
    {
        test_util();
        cout << "========================"s << endl
             << " All unit tests passed. "s << endl
             << "========================"s << endl;
        return 0;
    }

    print();
    string user = getuser();
    string hostname = gethostname("/etc/hostname");
    string username = YELLOW + user + RESET + "@" + YELLOW + hostname;
    cout << UNDERSCORE << username << RESET << endl;
    cout << endl;
    string os = getOS("/etc/os-release");
    cout << BRIGHT << GREEN << "OS : " << RESET << os << getHardwarePlatform()
         << endl;
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
    string cpu = getCPU("/proc/cpuinfo");
    cout << BRIGHT << GREEN << "CPU : " << RESET << cpu << endl;
    if (CpuTempCheck())
    {
        int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
        cout << BRIGHT << GREEN << "CPU Temperature : " << RESET
             << float(temp / 1000.0) << " °C" << endl;
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
