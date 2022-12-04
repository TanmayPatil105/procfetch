/**
 * @file
 */
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
#include <cctype>

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

void print(string color);

// util.cpp

string getColor(string);

string exec(string command);

void test_util();

/**
 * tests that want and got are equal.
 * Fails with the supplied failure message then halt.
 * @param want
 * @param got
 * @param msg
 */
template <typename T>
void expect(const T &want, const T &got, const string &msg)
{
    if (want == got)
        return;

    cout << "Error: "s << msg << " ("s << want << "), but got ("s << got << ")"s
         << endl;

    exit(1);
}

/**
 * Command executes a command in a subshell(shell runs on a forked process).
 *
 * Sample code:
 * ```cpp
 * auto c = Command::exec("ls -l");
 * if (c.getExitcode() == 0) {
 *     std::cout << c.getOutput();
 * }
 * ```
 */
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
    /**
     * Executes the specified command in a subshell.
     * @param cmd containing the command to call and its arguments
     * @returns Command object for getting the results.
     * @throws runtime_error failed to popen(3)
     */
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

    /**
     * @returns get contents written by the command to standard output
     */
    string getOutput()
    {
        return output;
    }

    /**
     * @returns get the new line counts of the output.
     */
    int getOutputLines()
    {
        return lines;
    }

    /**
     * @returns get the exit code of the command.
     */
    int getExitCode()
    {
        return exit_code;
    }
};

/**
 * A Path represents a path.
 */
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
    /**
     * @returns Path object
     * @param path
     */
    static Path of(const string &path)
    {
        return Path(filesystem::path(path), filesystem::status(path));
    }

    /**
     * @returns exists and is a regular file
     */
    bool is_regular_file()
    {
        return filesystem::is_regular_file(status);
    }

    /**
     * @returns exists and is a executable(or searchable)
     */
    bool is_executable()
    {
        if (status.permissions() == filesystem::perms::unknown)
            return false;
        return (status.permissions() & filesystem::perms::others_exec) !=
               filesystem::perms::none;
    }

    /**
     * @returns exists and is a directory
     */
    bool is_directory()
    {
        return filesystem::is_directory(status);
    }

    /**
     * @returns readable reprsentation for dev.
     */
    string to_s()
    {
        return path.string();
    }
};
