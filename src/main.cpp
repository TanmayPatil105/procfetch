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

struct {
    string theme = "def"s;
    string icons = "def"s;
    string CPU = "def"s;
    bool res_check_done =  false;
    bool res_check = false;
    string resolution;
    bool cpu_temp_check_done = false;
    bool cpu_temp_check = false;
    int cpu_temp;
    vector<string> gpus;
    bool gpus_done = false;
    string packages;
    bool packages_done = false;
} Info;


void *thread_1(void *arg){
    (void)arg;
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

    while(!Info.res_check_done);
    if (Info.res_check)
    {
        cout << title.text("Resolution : ")
            <<  Info.resolution << endl;
    }

    while(Info.theme == "def"s);
    cout << title.text("Theme : ") << Info.theme << endl;

    while(Info.CPU == "def"s);
    cout << title.text("Icons : ") << Info.icons << endl;

    while(Info.CPU == "def"s);
    cout << title.text("CPU : ") << Info.CPU << endl;

    while(!Info.cpu_temp_check_done);
    if (Info.cpu_temp_check)
    {
        cout << title.text("CPU Temperature : ") << float(Info.cpu_temp / 1000.0)
             << " °C" << endl;
    }

    while(!Info.gpus_done);
    for (auto gpu : Info.gpus)
    {
        cout << title.text("GPU : ") << gpu << endl;
    }

    return NULL;
}


void *thread_2(void *arg){
    (void)arg;

    if((Info.res_check = resCheck()))
    {
        Info.resolution = getRES("/sys/class/graphics/fb0/modes");
    }

    Info.res_check_done = true;
    
    Info.theme = getTheme();
    Info.icons = getIcons();
    Info.CPU = getCPU("/proc/cpuinfo");

    if((Info.cpu_temp_check = CpuTempCheck()))
    {
        Info.cpu_temp = getCPUtemp("/sys/class/thermal/thermal_zone0/temp");
    }

    Info.cpu_temp_check_done = true;
    
    Info.gpus = getGPU();
    Info.gpus_done =true;

    return NULL;
}

void *thread_3(void *arg){
    (void)arg;
    Info.packages = getPackages();
    Info.packages_done = true;
    return NULL;
}

/**
 * @returns Displays Info
 * @param show_battery
 */
void DisplayInfo(bool show_battery)
{
    auto title = Crayon{}.bright().green();

    pthread_t tid[3];
    pthread_create(&tid[0], NULL, thread_1, NULL);
    pthread_create(&tid[2], NULL, thread_3, NULL);
    pthread_create(&tid[1], NULL, thread_2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[1], NULL);

    cout << title.text("Packages : ") << Info.packages << endl;

    if (show_battery){
        print_battery("/sys/class/power_supply/BAT0/capacity");
    }

    cout << endl;
}
