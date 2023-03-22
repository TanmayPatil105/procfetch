/**
 * @file
 */
#include "fetch.h"

static void test_Command()
{
    auto c = Command::exec("ls Makefile"s);
    expect("Makefile\n"s, c->getOutput(), "getOutput()"s);
    expect(1, c->getOutputLines(), "getOutputLines()"s);

    c = Command::exec("true"s);
    expect(0, c->getExitCode(), "Exit code"s);

    c = Command::exec("false"s);
    expect(1, c->getExitCode(), "Exit code"s);
}

static void test_Command_async()
{
    string out;
    int lines;
    int status[2];    
    Command::exec_async("ls Makefile"s, [&](auto c){
        out = c->getOutput();
        lines = c->getOutputLines();
    });
    Command::exec_async("true"s, [&](auto c){
        status[0] = c->getExitCode();
    });
    Command::exec_async("false"s, [&](auto c){
        status[1] = c->getExitCode();
    });

    Command::wait();
    
    expect("Makefile\n"s, out, "getOutput()"s);
    expect(1, lines, "getOutputLines()"s);
    expect(0, status[0], "Exit code"s);
    expect(1, status[1], "Exit code"s);
}

static void test_Path()
{
    // directory
    auto p = Path::of("/etc"s);
    expect(false, p.isRegularFile(), "test -f "s + p.toString());
    expect(true, p.isDirectory(), "test -d "s + p.toString());
    expect(true, p.isExecutable(), "test -x "s + p.toString());

    // executable regular file
    p = Path::of("/bin/sh"s);
    expect(true, p.isRegularFile(), "test -f "s + p.toString());
    expect(false, p.isDirectory(), "test -d "s + p.toString());
    expect(true, p.isExecutable(), "test -x "s + p.toString());

    // non-executable regular file
    p = Path::of("Makefile"s);
    expect(true, p.isRegularFile(), "test -f "s + p.toString());
    expect(false, p.isDirectory(), "test -d "s + p.toString());
    expect(false, p.isExecutable(), "test -x "s + p.toString());

    // not existence path
    p = Path::of("not_existence"s);
    expect(false, p.isRegularFile(), "test -f "s + p.toString());
    expect(false, p.isExecutable(), "test -x "s + p.toString());
    expect(false, p.isDirectory(), "test -d "s + p.toString());
}

static void test_Crayon()
{
    Crayon style;
    style = Crayon{}.bright();
    expect("\033[1mKombu\033[0;m"s, style.text("Kombu"), ""s);
    style = style.red();
    expect("\033[1m\033[0;31mWAKAME\033[0;m"s, style.text("WAKAME"), ""s);

    style = Crayon{}.color("RED"s);
    expect("\033[0;31mHIJIKI\033[0;m"s, style.text("HIJIKI"), ""s);
}

/**
 * Tests belows.
 * * class Path
 * * class Command
 */
void test_util()
{
    test_Path();
    test_Command();
    test_Command_async();    
    test_Crayon();
}
