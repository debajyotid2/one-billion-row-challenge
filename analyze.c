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

#include <stdio.h>
#include <stdlib.h>
#include "src/dtypes.h"
#include "src/io_utils.h"
#include "src/hash_table.h"
#include "src/linked_list.h"

#define BUFSIZE 256

typedef struct {
    String location;
    double min, max, mean;
    size_t num_lines;
} Stats;

void stats_init(Stats** statrow, String location, double min, double max, double mean) {
    if (statrow==NULL) return;
    *statrow = (Stats*)malloc(sizeof(Stats));
    (*statrow)->location = location;
    (*statrow)->min = min;
    (*statrow)->max = max;
    (*statrow)->mean = mean;
    (*statrow)->num_lines = 0;
}

void stats_print(Stats* statrow) {
    if (statrow==NULL) return;
    string_print(&statrow->location);
    printf("=%.1f/%.1f/%.1f\n", statrow->min, statrow->max, statrow->mean);
}

void stats_destroy(Stats* statrow) {
    if (statrow==NULL) return;
    string_destroy(statrow->location);
}

ll_node_t* datarow_to_statsnode(DataRow* row) {
    if (row==NULL) return NULL;
    
    Stats* statrow;
    stats_init(&statrow, row->location, row->temperature, row->temperature, row->temperature);
    return ll_create_node(0, statrow);
}

size_t hashfunc(ll_node_t* node, hash_table_t* table) {
    if (node==NULL) {
        fprintf(stderr, "node pointer is null.\n");
        exit(1);
    }
    Stats* row = (Stats*)ll_get_node_data(node);
    if (row==NULL) {
        fprintf(stderr, "node data pointer is null.\n");
        exit(1);
    }
    size_t hash = 0;
    for (size_t i=0; i<row->location.length; ++i) {
        hash += (size_t)row->location.data[i];
    }

    return (hash * (size_t)row->location.data[1]) % ht_capacity(table);
}

int main(int argc, char** argv) {
    if (argc!=2) {
        fprintf(stderr, "Usage: %s <path to measurements txt file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* infile = fopen(argv[1], "r");

    if (!infile) {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char buf[BUFSIZE] = {'\0'};

    hash_table_t* cities;
    ht_init(&cities, 100, &hashfunc);
    
    unsigned long num_lines = 0;
    while (fgets(buf, BUFSIZE * sizeof(char), infile)) {
        num_lines++;
        if (num_lines < 3) continue;
        DataRow row = parse_single_row(buf);
        ll_node_t* node = datarow_to_statsnode(&row);
        if (!ht_insert_node(cities, node)) {
            Stats* stats = (Stats*)ht_at(hashfunc(node, cities), cities);
            if (string_equal(&row.location, &stats->location)) {
                stats->min = stats->min > row.temperature ? row.temperature: stats->min;
                stats->max = stats->max < row.temperature ? row.temperature: stats->max;
                stats->mean = (stats->mean * (double)stats->num_lines + row.temperature) / (double)(stats->num_lines + 1.0);
                stats->num_lines++;
            }
            printf("HASH COLLISION!\n");
            continue;
            // break;
        }
        ((Stats*)ll_get_node_data(node))->num_lines++;
        printf("Size: %zu, capacity: %zu\n", ht_size(cities), ht_capacity(cities));
    }
    ht_print_table(cities);
    
    for (size_t i=0; i<ht_capacity(cities); ++i) {
        Stats* val = (Stats*)ht_at(i, cities);
        if (val==NULL) continue;
        printf("Key %zu: ", i);
        stats_print(val);
    }

    fclose(infile);

    return EXIT_SUCCESS;
}
