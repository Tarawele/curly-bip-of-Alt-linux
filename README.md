# bipac(Binary Package Comparison) of Alt Linux
<!-- You need to make a shared library and  a CLI utility for it which :  receives lists of binary packages of 2 branches(from end points), then compares the received lists of packages and outputs JSON (you need to come up with a structure), which will display with certains conditions  and outputs JSON. -->

# CLI Binary Package Comparison Utility
<!-- myCLIUtility = bipac -->
## Overview
This CLI utility is designed to compare binary packages between different branches in the ALT Linux database.

## Dependencies
- cURL library (libcurl)
- JSON library for C++ (rapidjson)

Ensure that the dependencies are installed on the system.
Install dependencies using the package manager:

sudo apt-get install libcurl-devel rapidjson-devel

## Building the Project
To build the project from source, follow these steps:
1. Clone the repository:
   git clone <repository_url>
   cd <repository_directory>

2. Compile the source files with the shared library:
	rm -rf *.o libs/ /bin/bipac
	g++ -fpic -c getPackages.cpp parsePackages.cpp comparePackages.cpp
	g++ -fpic -o libbipac.so.1.0.0 *.o -shared -Wl,-soname,libbipac.so.1
	mkdir libs
	mv libbipac.so.1.0.0 ./libs/ && cd ./libs/
	ln -s libbipac.so.1.0.0 libbipac.so.1 && ln -s libbipac.so.1  libbipac.so
	LD_LIBRARY_PATH=$(pwd)/libs && export LD_LIBRARY_PATH
	cd ../
	g++ bipac.cpp -o bipac -I include/ -L libs/ -lbipac -lcurl
## Installation
2. Place the executable in a directory accessible from the command line.
   sudo cp bipac /bin/
## Quick installation 
	Execute or run installBipacOnMyLinux.sh on command line
	```bash
	installBipacOnMyLinux.sh
	```
## Running the CLI Utility
Execute the CLI utility with the following command pattern:
```bash
bipac <branch_1> <branch_2>
```

Example:
```bash
bipac sisyphus p10
```

## Interpreting the JSON Output
The JSON output structure includes:
- Packages unique to Branch 2
- Packages unique to Branch 1
- Packages with higher version-release in Branch 1 compared to Branch 2

## Troubleshooting
If you encounter any issues during installation or usage, please feel free to contact us at turaman99@gmail.com
