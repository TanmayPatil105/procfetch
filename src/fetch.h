/**
 * @file
 */
#pragma once

#include "color.h"
#include "config.h"
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string>
#include <thread>
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

bool resCheck();

string getRES(string path);

string getTheme();

string getIcons();

string getCPU(string path);

bool CpuTempCheck();

int getCPUtemp(string path);

vector<string> getGPU();

string getPackages();

void printBattery(string path);

void print(string color, string distro_name);

void test_fetch();

// util.cpp

string getColor(string);

void test_util();

/**
 * Tests that want and got are equal.
 * Fails with the supplied failure message, file and line then halt.
 * @param want
 * @param got
 * @param msg
 * @param file
 * @param line
 */
template <typename T>
void expect1(const T &want, const T &got, const string &msg, const char *file,
             int line)
{
    if (want == got)
        return;

    if (msg.length() != 0)
        cout << file << ":"s << line << ": Error: "s << msg << " want ("s
             << want << "), but got ("s << got << ")"s << endl;
    else
        cout << file << ":"s << line << ": Error: want ("s << want
             << "), but got ("s << got << ")"s << endl;

    exit(1);
}
#define expect(want, got, msg) expect1(want, got, msg, __FILE__, __LINE__)

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
     * @returns the filename of the path.
     */
    Path getFilename()
    {
        filesystem::path p = this->path.filename();
        return Path(p, filesystem::status(p));
    }

    /**
     * @returns exists and is a regular file
     */
    bool isRegularFile()
    {
        return filesystem::is_regular_file(status);
    }

    /**
     * @returns exists and is a executable(or searchable)
     */
    bool isExecutable()
    {
        if (status.permissions() == filesystem::perms::unknown)
            return false;
        return (status.permissions() & filesystem::perms::others_exec) !=
               filesystem::perms::none;
    }

    /**
     * @returns exists and is a directory
     */
    bool isDirectory()
    {
        return filesystem::is_directory(status);
    }

    /**
     * @returns readable reprsentation for dev.
     */
    string toString() const
    {
        return path.string();
    }

    /**
     * @returns a vector containing absolute paths to contents at the given path
     */
    vector<Path> getDirectoryContents()
    {
        vector<Path> contents;
        if (!this->isDirectory())
        {
            return contents;
        }

        for (const auto &entry : filesystem::directory_iterator(path))
        {
            contents.push_back(Path::of(entry.path()));
        }
        return contents;
    }
};

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
  public:
    typedef std::function<void(Command *)> func_type;

  private:
    int exit_code;
    string output;
    int lines;
    static std::vector<std::thread> ths;
    static std::vector<std::runtime_error> exceptions;
    static std::mutex mtx;

    Command()
    {
        output = string();
        lines = 0;
    }

  public:
    /**
     * Wait for all threads to be finished.
     */
    static void wait()
    {
        for (auto &t : ths)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }

    /**
     * @returns exceptions
     */
    static vector<runtime_error> &getExceptions()
    {
        return exceptions;
    }

    /**
     * Executes the specified command in a new thread.
     * @param cmd containing the command to call and its arguments
     * @param func to be performed after cmd is finished
     */
    static void exec_async(const string &cmd, const func_type &func)
    {
        ths.push_back(std::thread([=]() {
            try
            {
                auto result = exec(cmd);
                func(result);
            }
            catch (const runtime_error &e)
            {
                std::lock_guard<std::mutex> lock(mtx);
                exceptions.push_back(e);
            }
        }));
    }

    /**
     * Executes the specified command in a new thread.
     * @param cmd containing the command
     * @param arg arguments
     * @param func to be performed after cmd is finished
     */
    static void exec_async(const Path &cmd, const string &arg,
                           const func_type &func)
    {
        exec_async(cmd.toString() + " " + arg, func);
    }

    /**
     * Executes the specified command.
     * @param cmd containing the command to call and its arguments
     * @returns Command object for getting the results.
     * @throws runtime_error failed to popen(3)
     */
    static Command *exec(const string &cmd)
    {
        auto result = new Command();

        if (cmd == "./not-executable"s)
        {
            throw runtime_error("popen failed: \""s + cmd + "\""s);
        }

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
                result->lines += 1;
            }
            result->output += c;
        }
        // Don't concise below 2 lines. It must be assigned to a variable for
        // macOS.
        int n = pclose(pipe);
        result->exit_code = WEXITSTATUS(n);

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
 * String Styling for terminal.
 */
class Crayon
{
  private:
    string escape_codes;
    inline static map<string, string> m = {
        {"RED", RED},         {"GREEN", GREEN},   {"BLACK", BLACK},
        {"YELLOW", YELLOW},   {"BLUE", BLUE},     {"MAGENTA", MAGENTA},
        {"CYAN", CYAN},       {"WHITE", WHITE},   {"BBLACK", BBLACK},
        {"BGRAY", BGRAY},     {"BRED", BRED},     {"BGREEN", BGREEN},
        {"BYELLOW", BYELLOW}, {"BBLUE", BBLUE},   {"BMAGENTA", BMAGENTA},
        {"BCYAN", BCYAN},     {"BWHITE", BWHITE},
    };

    static string getColor(string s)
    {
        return m[s];
    }

  public:
    /**
     * default constructor
     */
    Crayon()
    {
        escape_codes = ""s;
    }
    /**
     * set bright mode
     */
    Crayon bright()
    {
        escape_codes += BRIGHT;
        return *this;
    }
    /**
     * set underscore mode
     */
    Crayon underscore()
    {
        escape_codes += UNDERSCORE;
        return *this;
    }
    /**
     * set color
     * @param color
     */
    Crayon color(string color)
    {
        escape_codes += getColor(color);
        return *this;
    }
    /** set color red */
    Crayon red()
    {
        escape_codes += RED;
        return *this;
    }
    /** set color green */
    Crayon green()
    {
        escape_codes += GREEN;
        return *this;
    }
    /** set color yellow */
    Crayon yellow()
    {
        escape_codes += YELLOW;
        return *this;
    }
    /**
     * @param s
     * @returns styled text
     */
    string text(string s)
    {
        return escape_codes + s + RESET;
    }
};

class Context
{
  public:
    static string PACKAGE_DELIM;
};

enum class Mode
{
    NORMAL,
    EXEC_TEST,
    SHOW_VERSION,
};

class Options
{
  public:
    Mode mode = Mode::NORMAL;
    string color_name = "def"s;
    string distro_name = "def"s;
    bool show_battery = false;

    Options(){};
    Options(int argc, char *argv[])
    {
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
                exit(1);
            }
        }
    }
};
