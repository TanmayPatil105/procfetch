/**
 * @file
 */
#include "fetch.h"

void DisplayInfo(bool show_battery);
void test_suite();

/**
 * @returns
 * @param argc
 * @param argv
 */
int main(int argc, char *argv[])
{
    auto options = Options(argc, argv);
    if (optind != argc)
    {
        cout << "Error: "s << argv[0] << ": unknown argument: "s << argv[optind]
             << endl;
        return 1;
    }

    switch (options.mode)
    {
    case Mode::NORMAL:
        // no-op
        break;
    case Mode::SHOW_VERSION:
        cout << VERSION << endl;
        return 0;
    default:
        // MUST NOT BE HERE
        return 1;
    }

    // Mode::NORMAL
    print(options.color_name, options.distro_name);
    DisplayInfo(options.show_battery);

    return 0;
}

/**
 * @returns Displays Info
 * @param show_battery
 */
void DisplayInfo(bool show_battery)
{
    string delim = " : "s;
    auto title = Crayon{}.bright().green();
    auto ye = Crayon{}.yellow();
    string username = ye.text(getuser()) + "@" +
                      ye.text(gethostname("/proc/sys/kernel/hostname"));
    cout << Crayon{}.underscore().text(username) << endl;
    cout << endl;

    cout << title.text("OS") << delim << getOS("/etc/os-release")
         << getHardwarePlatform() << endl;
    cout << title.text("Host") << delim
         << getHost("/sys/devices/virtual/dmi/id/") << endl;
    cout << title.text("Kernel") << delim
         << getKernel("/proc/sys/kernel/osrelease") << endl;
    cout << title.text("UpTime") << delim << getUpTime("/proc/uptime") << endl;
    cout << title.text("RAM") << delim << getRAM("/proc/meminfo") << endl;
    cout << title.text("shell") << delim << getSHELL("/etc/passwd") << endl;
    cout << title.text("DE") << delim << getDE() << endl;

    if (resCheck())
    {
        cout << title.text("Resolution") << delim
             << getRES("/sys/class/graphics/fb0/modes") << endl;
    }

    cout << title.text("Theme") << delim << getTheme() << endl;
    cout << title.text("Icons") << delim << getIcons() << endl;
    cout << title.text("CPU") << delim << getCPU("/proc/cpuinfo") << endl;

    if (CpuTempCheck())
    {
        int temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
        cout << title.text("CPU Temperature") << delim << float(temp / 1000.0)
             << " °C" << endl;
    }

    vector<string> gpus = getGPU();
    for (auto gpu : gpus)
    {
        cout << title.text("GPU") << delim << gpu << endl;
    }

    cout << title.text("Packages") << delim << getPackages() << endl;

    if (show_battery)
    {
        printBattery("/sys/class/power_supply/");
    }

    cout << endl;
}
