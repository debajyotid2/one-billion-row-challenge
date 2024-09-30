/* Analyze.
                    GNU AFFERO GENERAL PUBLIC LICENSE
                       Version 3, 19 November 2007

    Copyright (C) 2024  Debajyoti Debnath

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <fstream>
#include <unordered_map>

#define DELIM ";"

struct Stats {
    double min;
    double max;
    double mean;
    size_t num_lines;

    Stats() {
        min = max = mean = 0.0;
        num_lines = 1;
    }

    Stats(double a_min, double a_max, double a_mean) {
        min = a_min;
        max = a_max;
        mean = a_mean;
        num_lines = 1;
    }
};

std::pair<std::string, double> parse_single_row(std::string& row) {
    std::string city = row.substr(0, row.find(DELIM));
    double temperature = std::stod(row.substr(row.find(DELIM) + 1, row.length()));
    return std::make_pair(city, temperature);
}


int main(int argc, char** argv) {
    std::fstream file;
    if (argc != 2) {
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <PATH TO TEMPERATURE TXT FILE>\n");
    }
    file.open(argv[1]);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file.\n");
    }

    std::string line;
    std::unordered_map<std::string, Stats>* cities = new std::unordered_map<std::string, Stats>();

    while (std::getline(file, line)) {
        auto res = parse_single_row(line);
        auto it = cities->find(res.first);
        if (it != cities->end()) {
            if (res.second < (it->second).min)
                it->second.min = res.second;
            if (res.second > it->second.max)
                it->second.max = res.second;
            it->second.mean = (it->second.mean * it->second.num_lines + res.second) / (it->second.num_lines + 1);
            it->second.num_lines++;
            continue;
        }
        (*cities)[res.first] = Stats(res.second, res.second, res.second);
    }

    for (const auto& it: *cities) {
        printf("%s=%.1f/%.1f/%.1f\n", it.first.data(), it.second.min, it.second.max, it.second.mean);
    }

    delete cities;
    
    file.close();
    return 0;
}
