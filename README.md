# procfetch 
[![codecov](https://codecov.io/gh/TanmayPatil105/procfetch/branch/main/graph/badge.svg?token=QR6JGV3862)](https://codecov.io/gh/TanmayPatil105/procfetch)
![](https://github.com/TanmayPatil105/procfetch/actions/workflows/unit-test.yml/badge.svg)
![](https://github.com/TanmayPatil105/procfetch/actions/workflows/doxygen-gh-pages.yml/badge.svg)

<p align="center">
<img src="https://user-images.githubusercontent.com/92677342/205502731-328ab040-1df7-4b1b-bfa2-c5b84adafb85.png" width="1000">
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
* Graphviz -2.42.2 or later

for contributing

* ClangFormat - 14.0.0 or later

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
```
$ git clone https://github.com/TanmayPatil105/procfetch.git
 ```

```
$ cd procfetch
```

```
$ chmod +x install.sh
```
```
$ ./install.sh
```
```
$ sudo chmod +x /usr/bin/procfetch
```
## Uninstallation
```
$ chmod +x uninstall.sh
```
```
$ ./uninstall.sh
```

![](./images/tty.gif)

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

#### Disclamer :
Tried and tested on Debian, Ubuntu and ParrotOS.

