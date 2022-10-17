#pragma once

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

bool CpuTempCheck(string path);

vector<string> getGPU();

string getPackages();

void print();

// util.cpp

string getColor(string);

string exec(string command);

class Command {
private:
    int exit_code;
    string output;
    int lines;

    Command()
    {
        output = string();
        lines = 0;
    }

public:
    static Command exec(string cmd)
    {
        Command result = Command();

        FILE *pipe = popen(cmd.c_str(), "r");
        if (!pipe)
        {
            throw runtime_error("popen failed: \"" + cmd + "\"");
        }

        int c;
        while ( (c = fgetc(pipe)) != EOF)
        {
            if (c == '\n') {
                result.lines += 1;
            }
            result.output += c;
        }
        result.exit_code = WEXITSTATUS(pclose(pipe));

        return result;
    }

    string getOutput()
    {
        return output;
    }

    int getOutputLines()
    {
        return lines;
    }

    int getExitCode()
    {
        return exit_code;
    }
};

void test_util();

template <typename T>
void expect(T want, T got, string msg) {
    if (want == got)
        return;

    cout << "Error: "s << msg << " ("s << want << "), but got ("s << got << ")"s
         << endl;

    exit(1);
}
