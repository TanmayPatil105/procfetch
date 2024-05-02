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

static void test_Command_exception()
{
    int flow = 0;
    try
    {
        Command::exec("./not-executable"s);
    }
    catch (const runtime_error &e)
    {
        flow |= 0b1;
    }
    expect(0b1, flow, "catch exception");
}

static void test_Command_async()
{
    string out;
    int lines;
    int status[2];
    Command::exec_async("ls Makefile"s, [&](auto c) {
        out = c->getOutput();
        lines = c->getOutputLines();
    });
    Command::exec_async("true"s, [&](auto c) { status[0] = c->getExitCode(); });
    Command::exec_async("false"s,
                        [&](auto c) { status[1] = c->getExitCode(); });

    Command::wait();

    expect("Makefile\n"s, out, "getOutput()"s);
    expect(1, lines, "getOutputLines()"s);
    expect(0, status[0], "Exit code"s);
    expect(1, status[1], "Exit code"s);
}

static void test_Command_async2()
{
    string out;
    int lines;
    auto cmd = Path::of("/bin/ls"s);

    Command::exec_async(cmd, "Makefile"s, [&](auto c) {
        out = c->getOutput();
        lines = c->getOutputLines();
    });
    Command::wait();

    expect("Makefile\n"s, out, "getOutput()"s);
    expect(1, lines, "getOutputLines()"s);
}

static void test_Command_async_exception()
{
    int status = 0;

    Command::exec_async("./not-executable"s,
                        [&](auto c) { status = c->getExitCode(); });
    Command::wait();
    auto size = Command::getExceptions().size();

    expect((size_t)1, size, "1 exception");
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

    // directory is not empty
    p = Path::of("/bin"s);
    vector<Path> directoryContents = p.getDirectoryContents();
    expect(true, !directoryContents.empty(), "Directory is not empty");

    // getFilename()
    expect("bar.txt"s, Path::of("/foo/bar.txt").getFilename().toString(),
           "file"s);
    expect("etc"s, Path::of("/etc").getFilename().toString(), "directory"s);
    expect(""s, Path::of("/foo/bar/").getFilename().toString(), "none"s);
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

static void testhelper_Options(string msg, int argc, const char *argv[],
                               Options expect, int expect_optind)
{
    optind = 1;
    auto options = Options(argc, (char **)argv);

    expect((int)expect.mode, (int)options.mode, msg + ": Options.mode"s);
    expect(expect.color_name, options.color_name,
           msg + ": Options.color_name"s);
    expect(expect.distro_name, options.distro_name,
           msg + ": Options.distro_name"s);
    expect(expect.show_battery, options.show_battery,
           msg + ": Options.show_battery"s);
    expect(expect_optind, optind, msg + ": optind"s);
}

static void test_Options_default()
{
    int argc = 1;
    const char *argv[] = {"procfetch", NULL};
    Options expect;
    expect.mode = Mode::NORMAL;
    expect.color_name = "def"s;
    expect.distro_name = "def"s;
    expect.show_battery = false;

    testhelper_Options("default", argc, argv, expect, 1);
}

static void test_Options_full()
{
    int argc = 6;
    const char *argv[] = {"procfetch", "-a", "cyan", "-d",
                          "Manjaro",   "-b", "arg",  NULL};
    Options expect;
    expect.mode = Mode::NORMAL;
    expect.color_name = "cyan"s;
    expect.distro_name = "Manjaro"s;
    expect.show_battery = true;

    testhelper_Options("full", argc, argv, expect, 6); // remains last "arg"
}

static void test_Options_test()
{
    int argc = 2;
    const char *argv[] = {"procfetch", "-t", NULL};

    Options expect;
    expect.mode = Mode::EXEC_TEST;
    expect.color_name = "def"s;
    expect.distro_name = "def"s;
    expect.show_battery = false;

    testhelper_Options("test", argc, argv, expect, 2);
}

static void test_Options_version()
{
    int argc = 2;
    const char *argv[] = {"procfetch", "-v", NULL};

    Options expect;
    expect.mode = Mode::SHOW_VERSION;
    expect.color_name = "def"s;
    expect.distro_name = "def"s;
    expect.show_battery = false;

    testhelper_Options("version", argc, argv, expect, 2);
}

static void test_Options()
{
    test_Options_default();
    test_Options_full();
    test_Options_test();
    test_Options_version();
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
    test_Command_exception();
    test_Command_async();
    test_Command_async2();
    test_Command_async_exception();
    test_Crayon();
    test_Options();
}
