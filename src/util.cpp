#include "color.h"
#include "fetch.h"

string exec(string command)
{
    char buffer[128];
    string result = "";
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        throw runtime_error("popen failed: \"" + command + "\"");
    }
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}

string getColor(string line)
{
    string color;
    if (line.substr(0, line.find(" ")) == "RED")
    {
        color = RED;
    }
    else if (line.substr(0, line.find(" ")) == "BLACK")
    {
        color = BLACK;
    }
    else if (line.substr(0, line.find(" ")) == "GREEN")
    {
        color = GREEN;
    }
    else if (line.substr(0, line.find(" ")) == "YELLOW")
    {
        color = YELLOW;
    }
    else if (line.substr(0, line.find(" ")) == "BLUE")
    {
        color = BLUE;
    }
    else if (line.substr(0, line.find(" ")) == "MAGENTA")
    {
        color = MAGENTA;
    }
    else if (line.substr(0, line.find(" ")) == "CYAN")
    {
        color = CYAN;
    }
    else if (line.substr(0, line.find(" ")) == "WHITE")
    {
        color = WHITE;
    }
    else if (line.substr(0, line.find(" ")) == "BBLACK")
    {
        color = BBLACK;
    }
    else if (line.substr(0, line.find(" ")) == "BGRAY")
    {
        color = BGRAY;
    }
    else if (line.substr(0, line.find(" ")) == "BRED")
    {
        color = BRED;
    }
    else if (line.substr(0, line.find(" ")) == "BGREEN")
    {
        color = BGREEN;
    }
    else if (line.substr(0, line.find(" ")) == "BYELLOW")
    {
        color = BYELLOW;
    }
    else if (line.substr(0, line.find(" ")) == "BBLUE")
    {
        color = BBLUE;
    }
    else if (line.substr(0, line.find(" ")) == "BMAGENTA")
    {
        color = BMAGENTA;
    }
    else if (line.substr(0, line.find(" ")) == "BCYAN")
    {
        color = BCYAN;
    }
    else if (line.substr(0, line.find(" ")) == "BWHITE")
    {
        color = BWHITE;
    }

    return color;
}

static void test_exec()
{
    string result = exec("echo \"hello, world\""s);
    expect("hello, world\n"s, result, "exec() returns"s);
}

static void test_Command()
{
    Command c = Command::exec("ls Makefile"s);
    expect("Makefile\n"s, c.getOutput(), "getOutput()"s);
    expect(1, c.getOutputLines(), "getOutputLines()"s);

    c = Command::exec("true"s);
    expect(0, c.getExitCode(), "Exit code"s);

    c = Command::exec("false"s);
    expect(1, c.getExitCode(), "Exit code"s);
}

static void test_Path()
{
    // directory
    auto p = Path::of("/etc"s);
    expect(false, p.is_regular_file(), "test -f "s + p.to_s());
    expect(true, p.is_directory(), "test -d "s + p.to_s());
    expect(true, p.is_executable(), "test -x "s + p.to_s());

    // executable regular file
    p = Path::of("/bin/sh"s);
    expect(true, p.is_regular_file(), "test -f "s + p.to_s());
    expect(false, p.is_directory(), "test -d "s + p.to_s());
    expect(true, p.is_executable(), "test -x "s + p.to_s());

    // non-executable regular file
    p = Path::of("Makefile"s);
    expect(true, p.is_regular_file(), "test -f "s + p.to_s());
    expect(false, p.is_directory(), "test -d "s + p.to_s());
    expect(false, p.is_executable(), "test -x "s + p.to_s());

    // not existence path
    p = Path::of("not_existence"s);
    expect(false, p.is_regular_file(), "test -f "s + p.to_s());
    expect(false, p.is_executable(), "test -x "s + p.to_s());
    expect(false, p.is_directory(), "test -d "s + p.to_s());
}

void test_util()
{
    test_exec();
    test_Path();
    test_Command();
}
