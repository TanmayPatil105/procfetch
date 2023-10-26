/**
 * @file
 */
#include "fetch.h"

void DisplayInfo(bool show_battery);

/**
 * @returns
 * @param argc
 * @param argv
 */
int main(int argc, char *argv[])
{
    Mode mode = Mode::NORMAL;
    string color_name = "def"s;
    string distro_name = "def"s;
    bool show_battery = false;

    int opt;
    while ((opt = getopt(argc, argv, "ta:d:vb")) != -1)
    {
        switch (opt)
        {
        case 't':
            mode = Mode::EXEC_TEST;
            break;
        case 'a':
            color_name = string(optarg);
            break;
        case 'd':
            distro_name = string(optarg);
            break;
        case 'b':
            show_battery = true;
            break;
        case 'v':
            mode = Mode::SHOW_VERSION;
            break;
        default:
            return 1;
        }
    }

    switch (mode)
    {
    case Mode::NORMAL:
        // no-op
        break;
    case Mode::EXEC_TEST:
        test_util();
        cout << Crayon{}.green().text("All unit tests passed."s) << endl;
        return 0;
    case Mode::SHOW_VERSION:
        cout << VERSION << endl;
        return 0;
    default:
        // MUST NOT BE HERE
        return 1;
    }

    // Mode::NORMAL
    if (optind != argc)
    {
        cout << "Error: "s << argv[0] << ": unknown argument: "s << argv[optind]
             << endl;
        return 1;
    }

    print(color_name, distro_name);
    DisplayInfo(show_battery);

    return 0;
}

/**
 * @returns Displays Info
 * @param show_battery
 */
void DisplayInfo(bool show_battery)
{
    auto title = Crayon{}.bright().green();
    auto ye = Crayon{}.yellow();
    string username =
        ye.text(getuser()) + "@" + ye.text(gethostname("/etc/hostname"));
    cout << Crayon{}.underscore().text(username) << endl;
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

    if (resCheck())
    {
        cout << title.text("Resolution : ")
             << getRES("/sys/class/graphics/fb0/modes") << endl;
    }

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

    if (show_battery)
    {
        print_battery("/sys/class/power_supply/BAT0/capacity");
    }

    cout << endl;
}
