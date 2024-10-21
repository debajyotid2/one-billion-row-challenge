# One Billion Row Challenge

Inspired by the original [One billion row challenge](https://github.com/gunnarmorling/1brc), one billion rows of data (~12 GB) are consumed and processed in the least time-consuming way possible! The data source is from the [original repository](https://github.com/gunnarmorling/1brc/tree/main/data).

## The Challenge

One billion rows of data containing temperature information for different locations is read in and the minimum, maximum and mean temperatures for each location calculated and recorded.
The following snippet shows what the input data looks like:
```
...
Tokyo;35.6897
Jakarta;-6.1750
Delhi;28.6100
Guangzhou;23.1300
Mumbai;19.0761
Manila;14.5958
...
```

The statistics are then output in the format `<CITY>=<MIN>/<MEAN>/<MAX>`
```
{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0, Accra=-10.1/26.4/66.4, Addis Ababa=-23.7/16.0/67.0, Adelaide=-27.8/17.3/58.5, ...}
```

## How to build

Only Linux-based operating systems are supported as of now.

### Installing dependencies

`g++` (`gcc` version 11.4.0) is the default compiler used for building the project. `cmake` (version 3.27+) is the build system used.

The project depends on [matlibr](https://github.com/debajyotid2/matlibr.git), [yatpool](https://github.com/debajyotid2/yatpool.git) and [Criterion](https://github.com/Snaipe/Criterion.git) as third party dependencies. An easy way to install these is to use the `setup_dependencies.sh` script. Currently it only works with `dnf` - a package manager for Fedora, for installing `libgit2`, `meson` and `libffi`. If you do not have `dnf`, then please install them separately from the package manager for your Linux distribution.

To use `setup_dependencies.sh`, please run
```
cd scripts
source setup_dependencies.sh
```

### Building

To build the tests and binaries for generating the measurements and analyzing them, run
```
cd scripts
source build.sh
```
To build in either debug or profile mode, the `DEBUG` or `PROFILE` flags can be set to 1 when calling the build script.

### Running

To create the temperature data, run 
```
cd build
./create_measurements -D <path to source data> -N <number of rows to generate>
```
The source data in this case is a text file containing the names of the cities and mean temperature measurements from which temperatures will be sampled for the output. For one billion rows, the time taken is approximately 3 minutes (with 16 threads).

To run the code that analyzes the temperature data and calculates statistics, run
```
cd build
./analyze <path to temperature data>
```

## License

[AGPL 3.0](https://www.gnu.org/licenses/agpl-3.0.en.html)
