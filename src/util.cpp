/**
 * @file
 */
#include "fetch.h"

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
    test_Crayon();
}
