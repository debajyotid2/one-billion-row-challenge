/* Main
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

#include <matrix.h>
#include <sys/time.h>
#include "src/dtypes.h"
#include "src/io_utils.h"
#include "src/args.h"
#include "src/generate_data.h"

#define DEBUG 0
#define TIME 1

/// Program options
static struct argp_option options[] = {
    {"raw_data_path", 'D', "RAW_DATA_PATH", 0, "Path to weather_stations.txt containing locations and mean temperatures"},
    {"n_rows", 'N', "N_ROWS", 0, "Number of rows to generate"},
    {"seed", 'S', "SEED", 0, "Seed for randomness"},
    {0}
};

// Argp argument parser configuration
const char* argp_program_version = "v.0.0.1";
const char* argp_program_bug_address = "the issue tracker at https://github.com/debajyotid2/one-billion-row-challenge.git";

static char doc[] = "Generates temperature data for the one billion row challenge";

/// Function to parse arguments option by option
static error_t parse_opt(int key, char* arg, struct argp_state* state) {
    struct arguments *arguments = (struct arguments*)(state->input);

    switch (key) {
        case 'N':
            printf("Setting n_rows to %s...\n", arg);
            arguments->n_rows = atoi(arg);
            break;
        case 'S':
            printf("Setting seed to %s...\n", arg);
            arguments->seed = atoi(arg);
            break;
        case 'D':
            memset(arguments->raw_data_path, 0x0, sizeof(arguments->raw_data_path));
            strncpy(arguments->raw_data_path, arg, strlen(arg) * sizeof(char));
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// Argument parser
static struct argp argparser = {options, parse_opt, 0, doc};

int main(int argc, char** argv) {
    struct arguments arg_vals;

    // Parse arguments
    init_arguments(&arg_vals);
    argp_parse(&argparser, argc, argv, 0, 0, &arg_vals);
    print_arguments(&arg_vals);

    FILE* datafile = fopen(arg_vals.raw_data_path, "r");

    // Exit if file could not be opened
    if (datafile == NULL) {
        fprintf(stderr, "Could not read %s.\n", arg_vals.raw_data_path);
        return 2;
    }
    
    size_t num_threads = 16;

#if TIME
    struct timeval start, end;
    long duration = 0; // microseconds
    
    gettimeofday(&start, NULL);
#endif // TIME

    // Parse raw data
    printf("Parsing raw data ...\n");
    DataRowGroup parsed_data = parse_raw_data(datafile);
    printf("Done.\n");

#if TIME
    gettimeofday(&end, NULL);
    duration = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Parsing raw data took %g milliseconds.\n", (double)duration / 1000.0);
#endif // TIME

#if DEBUG
    printf("Parsed data:\n");
    datarowgroup_print(&parsed_data);
#endif // DEBUG
    
#if TIME
    gettimeofday(&start, NULL);
#endif // TIME

    // Random sample with replacement from parsed data
    printf("Sampling %zu rows from parsed data ...\n", arg_vals.n_rows);
    String* sampled_data = generate_random_temperature_sample_threaded(&parsed_data, arg_vals.n_rows, arg_vals.seed, num_threads);
    printf("Done.\n");
    
#if TIME
    gettimeofday(&end, NULL);
    duration = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Sampling data took %g milliseconds.\n", (double)duration / 1000.0);
#endif // TIME

#if DEBUG
    printf("Sampled data:\n");
    datarowgroup_print(&sampled_data);
#endif // DEBUG

#if TIME
    gettimeofday(&start, NULL);
#endif // TIME
   
    // Write the sampled data to a file
    printf("Writing data ...\n");
    const char* outfile = "../data/output.txt";
    write_datarowgroup_threaded(sampled_data, outfile, arg_vals.n_rows, num_threads);
    printf("Done.\n");

#if TIME
    gettimeofday(&end, NULL);
    duration = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Writing data took %g milliseconds.\n", (double)duration / 1000.0);
#endif // TIME

    // Release all buffers
    datarowgroup_destroy(&parsed_data);
    for (size_t i = 0; i < arg_vals.n_rows; ++i)
        string_destroy(sampled_data[i]);
    free(sampled_data);
    
    fclose(datafile);
}
