#include "fetch.h"
#include "color.h"

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

static void test_exec() {
    string result = exec("echo -n \"hello, world\""s);
    expect("hello, world"s, result, "exec() returns"s);
}

static void test_command()
{
    Command c = Command::exec("/usr/bin/echo -ne \"\n\n\n\""s);
    expect(3, c.getOutputLines(), "getOutputLines()"s);

    c = Command::exec("true"s);
    expect(0, c.getExitCode(), "Exit code"s);

    c = Command::exec("false"s);
    expect(1, c.getExitCode(), "Exit code"s);
}

void test_util()
{
    test_exec();
    test_command();
}
