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

The statistics are then output in the format `<CITY>=<MIN>/<MAX>/<MEAN>`
```
{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0, Accra=-10.1/26.4/66.4, Addis Ababa=-23.7/16.0/67.0, Adelaide=-27.8/17.3/58.5, ...}
```

## License

[AGPL 3.0](https://www.gnu.org/licenses/agpl-3.0.en.html)
