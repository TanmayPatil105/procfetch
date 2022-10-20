#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <unistd.h>
#include <stdexcept>
#include <ostream>

using namespace std;

// fetch.cpp

string getuser();

string gethostname(string path);

string getOS(string path);

string getHardwarePlatform();

string getHost(string path);

string getKernel(string path);

string getUpTime(string path);

string getRAM(string path);

string getSHELL(string path);

string getDE();

string getRES(string path);

string getTheme();

string getIcons();

string getCPU(string path);

int getCPUtemp(string path);

bool CpuTempCheck();

vector<string> getGPU();

string getPackages();

void print();

// util.cpp

string getColor(string);

string exec(string command);

void test_util();

template <typename T>
void expect(T want, T got, string msg) {
    if (want == got)
        return;

    cout << "Error: "s << msg << " ("s << want << "), but got ("s << got << ")"s
         << endl;

    exit(1);
}

class Path {
private:
    filesystem::path path;
    filesystem::file_status status;

    Path(filesystem::path path, filesystem::file_status status) {
        this->path = path;
        this->status = status;
    }

public:
    static Path of(string path) {
        return Path(filesystem::path(path), filesystem::status(path));
    }
    bool is_regular_fie() {
        return filesystem::is_regular_file(status);
    }
    bool is_directory() {
        return filesystem::is_directory(status);
    }
};
