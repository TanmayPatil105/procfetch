# procfetch 
[![codecov](https://codecov.io/gh/TanmayPatil105/procfetch/branch/main/graph/badge.svg?token=QR6JGV3862)](https://codecov.io/gh/TanmayPatil105/procfetch)
![](https://github.com/TanmayPatil105/procfetch/actions/workflows/unit-test.yml/badge.svg)
![](https://github.com/TanmayPatil105/procfetch/actions/workflows/doxygen-gh-pages.yml/badge.svg)

<p align="center">
<img src="./images/procfetch.gif" width="700" height="700">
</p>

#### procfetch is a command-line tool to fetch system information and display it on the screen. 
#### It is written in `C++`. 
#### Project inspiration : `neofetch` - a command line system information tool

### Information displayed by procfetch
#### Static characteristics

* Host (User)
* CPU
* Kernel
* OS
* Shell
* Desktop Enviroment
* Resolution
* Theme
* Icons
* GPU (Integrated & Dedicated)
* Package count

#### Dynamic characteristics
* RAM
* Uptime
* Temperature

## Prerequisites

for building

* GNU Make - 4.3 or later
* GCC - 11.3.0 or later

for generating API documents

* Doxygen - 1.9.1 or later
* Graphviz -2.43.0 or later

for contributing

* ClangFormat - 14.0.0 or later

for generating gif

* vhs - 0.2.0 or later

## Documentation

See the [documentation](https://tanmaypatil105.github.io/procfetch/)

## Installation

### Brew
```
$ brew tap TanmayPatil105/tap
```
```
$ brew install procfetch
```

### Manually
```bash
$ git clone https://github.com/TanmayPatil105/procfetch.git; cd procfetch
 ```
```bash
$ ./configure
```
```bash
$ sudo make install
```

## Uninstallation
```bash
$ sudo make uninstall
```

<hr/>

## Usage

- `-v` to show version
```bash
$ procfetch -v
```

- `-a` to change color of ascii_art
```bash
$ procfetch -a cyan
```

- `-d` to change print ascii_art of different distros
```bash
$ procfetch -d Manjaro
```

- `-b` to display battery percentage
```bash
$ procfetch -b
```

## Customise

Add the following lines to your `.bash_profile`, `.bashrc` or `.zshrc` to customise `procfetch`

```bash
# print default ascii art of Manjaro
alias procfetch="procfetch -d Manjaro"
```

```bash
# print default ascii art of Kali with color cyan
alias procfetch="procfetch -d Kali -a cyan"
```

```bash
# print battery percentage
alias procfetch="procfetch -b"
```

## Docker

Define environment variable `PROCFETCH` in the actual project home directory.

```sh
$ PROCFETCH=$HOME/procfetch
```

Build Docker image

```sh
$ docker build -t procfetch .
```

Build with Docker container
```sh
$ docker run --rm -v $PROCFETCH:/procfetch -w /procfetch procfetch make
```

Run with Docker container
```sh
$ docker run --rm -v $PROCFETCH:/procfetch -w /procfetch procfetch make run
```

Test with Docker container
```sh
$ docker run --rm -v $PROCFETCH:/procfetch -w /procfetch procfetch make check
```

### Contributing

Make sure reading [Contributing.md](https://github.com/TanmayPatil105/procfetch/blob/main/CONTRIBUTING.md) before Contributing

