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
#include <sys/types.h>
#include "src/dtypes.h"
#include "src/io_utils.h"
#include "src/hash_table.h"

#define BUFSIZE 256

typedef struct {
    double min, max, mean;
    size_t num_lines;
} Stats;

void stats_init(Stats** statrow, double min, double max, double mean) {
    if (statrow==NULL) return;
    *statrow = (Stats*)malloc(sizeof(Stats));
    (*statrow)->min = min;
    (*statrow)->max = max;
    (*statrow)->mean = mean;
    (*statrow)->num_lines = 1;
}

void stats_print(Stats* statrow) {
    if (statrow==NULL) return;
    printf("=%.1f/%.1f/%.1f\n", statrow->min, statrow->max, statrow->mean);
}

void* datarow_to_statsnode(DataRow* row) {
    if (row==NULL) return NULL;
    
    Stats* statrow;
    stats_init(&statrow, row->temperature, row->temperature, row->temperature);
    return (void*)statrow;
}

size_t myhash(void* key, hash_table_t* table) {
    if (table==NULL) {
        fprintf(stderr, "hash table pointer is null.\n");
        exit(1);
    }
    String* row = (String*)key;
    if (row==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    size_t hash = 0;
    for (size_t i=0; i<row->length; ++i) {
        hash += hash * 97 + (size_t)row->data[i];
    }

    return hash % ht_capacity(table);
}

size_t djb2(void* key, hash_table_t* table) {
    if (table==NULL) {
        fprintf(stderr, "hash table pointer is null.\n");
        exit(1);
    }
    String* row = (String*)key;
    if (row==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    size_t hash = 5381;
    for (size_t i=0; i<row->length; ++i) {
        hash = ((hash << 5) + hash) + (size_t)row->data[i];
    }

    return hash % ht_capacity(table);
}

void print_stats(hash_table_t* table) {
    if (table==NULL) return;
    for (size_t i=0; i<ht_capacity(table); ++i) {
        KeyValuePair kv = ht_at_index(table, i);
        String* key = (String*)kv.key;
        Stats* value = (Stats*)kv.value;
        if (key==NULL) continue;
        if (value==NULL) continue;
        string_print(key);
        stats_print(value);
    }
}

bool key_equal(void* key1, void* key2) {
    if (key1==NULL || key2==NULL) {
        fprintf(stderr, "null pointer given.\n");
        exit(1);
    }
    return string_equal((String*)key1, (String*)key2);
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
    size_t num_collisions = 0;
    hash_function hashfunc = &myhash; 
    ht_init(&cities, 50000, hashfunc, key_equal);
    
    unsigned long num_lines = 0;
    bool table_full = false;
    while (fgets(buf, BUFSIZE * sizeof(char), infile) && !table_full) {
        num_lines++;
        DataRow* row = (DataRow*)malloc(sizeof(DataRow));
        *row = parse_single_row(buf);
        void* value = datarow_to_statsnode(row);

        if (!ht_insert(cities, row->location, value)) {
            if (ht_size(cities) >= ht_capacity(cities)) {
                table_full = true;
            }
            KeyValuePair kv = ht_at(cities, row->location);
            Stats* stats = (Stats*)kv.value;
            
            stats->min = stats->min > row->temperature ? row->temperature: stats->min;
            stats->max = stats->max < row->temperature ? row->temperature: stats->max;                                               
            stats->mean = (stats->mean * (double)stats->num_lines + row->temperature) / (double)(stats->num_lines + 1.0);
            stats->num_lines++;
            
            string_destroy(*(row->location));
            free(row->location);
            free(value);
        }
        free(row);
    }
    
    fclose(infile);

    printf("Lines of input file covered: %zu\n", num_lines);
    printf("Size: %zu, capacity: %zu, num_collisions: %zu\n", ht_size(cities), ht_capacity(cities), num_collisions);
    
    print_stats(cities);

    for (size_t i=0; i<ht_capacity(cities); ++i) {
        KeyValuePair kv = ht_at_index(cities, i);
        if (kv.key == NULL) continue;
        string_destroy(*(String*)(kv.key));
        free(kv.key);
        free(kv.value);
    }
    ht_destroy(cities);
    free(cities);
    
    return EXIT_SUCCESS;
}
