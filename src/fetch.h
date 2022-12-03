#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

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
void expect(const T &want, const T &got, const string &msg)
{
    if (want == got)
        return;

    cout << "Error: "s << msg << " ("s << want << "), but got ("s << got << ")"s
         << endl;

    exit(1);
}

class Command
{
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
    static Command exec(const string &cmd)
    {
        Command result = Command();

        FILE *pipe = popen(cmd.c_str(), "r");
        if (!pipe)
        {
            throw runtime_error("popen failed: \""s + cmd + "\""s);
        }

        int c;
        while ((c = fgetc(pipe)) != EOF)
        {
            if (c == '\n')
            {
                result.lines += 1;
            }
            result.output += c;
        }
        // Don't concise below 2 lines. It must be assigned to a variable for
        // macOS.
        int n = pclose(pipe);
        result.exit_code = WEXITSTATUS(n);

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

class Path
{
  private:
    filesystem::path path;
    filesystem::file_status status;

    Path(filesystem::path path, filesystem::file_status status)
    {
        this->path = path;
        this->status = status;
    }

  public:
    static Path of(const string &path)
    {
        return Path(filesystem::path(path), filesystem::status(path));
    }
    bool is_regular_file()
    {
        return filesystem::is_regular_file(status);
    }
    bool is_executable()
    {
        if (status.permissions() == filesystem::perms::unknown)
            return false;
        return (status.permissions() & filesystem::perms::others_exec) !=
               filesystem::perms::none;
    }
    bool is_directory()
    {
        return filesystem::is_directory(status);
    }
    string to_s()
    {
        return path.string();
    }
};
