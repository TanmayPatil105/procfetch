/**
 * @file
 */
#include "color.h"
#include "fetch.h"

void DisplayInfo();

/**
 * @returns
 * @param argc
 * @param argv
 */
int main(int argc, char *argv[])
{
    bool test_mode = false;
    string color_name = "def"s;
    string distro_name = "def"s;

    int opt;
    while ((opt = getopt(argc, argv, "ta:d:")) != -1)
    {
        switch (opt)
        {
        case 't':
            test_mode = true;
            break;
        case 'a':
            color_name = string(optarg);
            break;
        case 'd':
            distro_name = string(optarg);
            break;
        default:
            return 1;
        }
    }

    if (test_mode)
    {
        test_util();
        cout << "========================"s << endl
             << " All unit tests passed. "s << endl
             << "========================"s << endl;
        return 0;
    }

    if (optind != argc)
    {
        cout << "Error: "s << argv[0] << ": unknown argument: "s << argv[optind]
             << endl;
        return 1;
    }

    print(color_name, distro_name);
    DisplayInfo();

    return 0;
}

/**
 * @returns Displays Info
 */
void DisplayInfo()
{
    auto title = Color{}.bright().green();
    string user = getuser();
    string hostname = gethostname("/etc/hostname");
    string username = YELLOW + user + RESET + "@" + YELLOW + hostname;
    cout << UNDERSCORE << username << RESET << endl;
    cout << endl;

    cout << title.text("OS : ") << getOS("/etc/os-release")
         << getHardwarePlatform() << endl;
    cout << title.text("Host : ") << getHost("/sys/devices/virtual/dmi/id/")
         << endl;
    cout << title.text("Kernel : ") << getKernel("/proc/sys/kernel/osrelease")
         << endl;
    cout << title.text("UpTime : ") << getUpTime("/proc/uptime") << endl;
    cout << title.text("RAM : ") << getRAM("/proc/meminfo") << endl;
    cout << title.text("shell : ") << getSHELL("/etc/passwd") << endl;
    cout << title.text("DE : ") << getDE() << endl;
    cout << title.text("Resolution : ")
         << getRES("/sys/class/graphics/fb0/modes") << endl;
    cout << title.text("Theme : ") << getTheme() << endl;
    cout << title.text("Icons : ") << getIcons() << endl;
    cout << title.text("CPU : ") << getCPU("/proc/cpuinfo") << endl;

    if (CpuTempCheck())
    {
        int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
        cout << title.text("CPU Temperature : ") << float(temp / 1000.0)
             << " °C" << endl;
    }

    vector<string> gpus = getGPU();
    for (auto gpu : gpus)
    {
        cout << title.text("GPU : ") << gpu << endl;
    }

    cout << title.text("Packages : ") << getPackages() << endl;
    cout << endl;
}
