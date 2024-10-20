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
#define MAX_TBL_SIZE 65536

typedef struct {
    double min, max, sum;
    size_t num_lines;
} Stats;

static void stats_init(Arena* arena, Stats** statrow, double min, double max, double sum);
static void stats_print(Stats* statrow);
static void* datarow_to_statsnode(Arena* arena, DataRow* row);
static char* sa_string_to_char_arr(Arena* arena, SAString* str);
static void* sa_datarow_to_statsnode(Arena* arena, SADataRow* row);
static size_t myhash(void* key, hash_table_t* table);
static size_t djb2(void* key, hash_table_t* table);
static void print_stats(hash_table_t* table);
static int keycmp(void* key1, void* key2);

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
    SADataRow row;
    Arena* arena;
    arena_create(&arena, (sizeof(SADataRow) + sizeof(String)) * MAX_TBL_SIZE);

    hash_table_t* cities;
    hash_function hashfunc = &myhash; 
    ht_init(&cities, MAX_TBL_SIZE, hashfunc, keycmp);
    
    unsigned long num_lines = 0;
    while (fgets(buf, BUFSIZE * sizeof(char), infile)) {
        num_lines++;
        row = sa_parse_single_row(buf);

        if (ht_twos_pow_contains(cities, &(row.location.data))) {
            KeyValuePair kv = ht_twos_pow_at(cities, &(row.location.data));
            Stats* stats = (Stats*)kv.value;
            
            stats->min = stats->min > row.temperature ? row.temperature: stats->min;
            stats->max = stats->max < row.temperature ? row.temperature: stats->max;                                               
            stats->sum += row.temperature;
            stats->num_lines++;
        } else {
            ht_twos_pow_insert(cities, sa_string_to_char_arr(arena, &row.location), sa_datarow_to_statsnode(arena, &row));
        }
    }
    
    fclose(infile);

    printf("Lines of input file covered: %zu\n", num_lines);
    printf("Size: %zu, capacity: %zu\n", ht_size(cities), ht_capacity(cities));
    
    print_stats(cities);
    
    arena_destroy(arena);
    ht_destroy(cities);
    free(cities);
    
    return EXIT_SUCCESS;
}

static void stats_init(Arena* arena, Stats** statrow, double min, double max, double sum) {
    if (statrow==NULL) return;
    *statrow = (Stats*)arena_allocate(arena, sizeof(Stats));
    (*statrow)->min = min;
    (*statrow)->max = max;
    (*statrow)->sum = sum;
    (*statrow)->num_lines = 1;
}

static void stats_print(Stats* statrow) {
    if (statrow==NULL) return;
    printf("=%.1f/%.1f/%.1f\n", statrow->min, statrow->max, statrow->sum / statrow->num_lines);
}

static void* datarow_to_statsnode(Arena* arena, DataRow* row) {
    if (row==NULL) return NULL;
    
    Stats* statrow;
    stats_init(arena, &statrow, row->temperature, row->temperature, row->temperature);
    return (void*)statrow;
}

static void* sa_datarow_to_statsnode(Arena* arena, SADataRow* row) {
    if (row==NULL) return NULL;
    
    Stats* statrow;
    stats_init(arena, &statrow, row->temperature, row->temperature, row->temperature);
    return (void*)statrow;
}

static size_t myhash(void* key, hash_table_t* table) {
    if (table==NULL) {
        fprintf(stderr, "hash table pointer is null.\n");
        exit(1);
    }
    char* row = (char*)key;
    if (row==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    size_t hash = 0;
    for (size_t i=0; i<strlen(row); ++i) {
        hash += hash * 97 + (size_t)row[i];
    }

    return hash & (ht_capacity(table)-1);
}

static size_t djb2(void* key, hash_table_t* table) {
    if (table==NULL) {
        fprintf(stderr, "hash table pointer is null.\n");
        exit(1);
    }
    char* row = (char*)key;
    if (row==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    size_t hash = 5381;
    for (size_t i=0; i<strlen(row); ++i) {
        hash = ((hash << 5) + hash) + (size_t)row[i];
    }

    return hash & (ht_capacity(table)-1);
}

static char* sa_string_to_char_arr(Arena* arena, SAString* str) {
    if (str==NULL) return NULL;
    char* res = (char*)arena_allocate(arena, (str->length+1) * sizeof(char));
    memcpy(res, str->data, str->length);
    res[str->length] = '\0';
    return res;
};

static void print_stats(hash_table_t* table) {
    if (table==NULL) return;
    for (size_t i=0; i<ht_capacity(table); ++i) {
        KeyValuePair kv = ht_at_index(table, i);
        char* key = (char*)kv.key;
        Stats* value = (Stats*)kv.value;
        if (key==NULL) continue;
        if (value==NULL) continue;
        printf("%s", key);
        stats_print(value);
    }
}

static int keycmp(void* dictkey, void* extkey) {
    if (dictkey==NULL || extkey==NULL) {
        fprintf(stderr, "null pointer given.\n");
        exit(1);
    }
    char* str1 = (char*)dictkey;
    char* str2 = (char*)extkey;
    return strcmp(str1, str2);
}
