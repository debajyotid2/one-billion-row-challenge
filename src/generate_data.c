/* Functions for generating data for the one billion row challenge.

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

#include "generate_data.h"
#include "format.h"

typedef struct {
    size_t low, high;
    size_t seed;
    DataRowGroup* source;
    String* destination;
} _SampleRowsArg;

void _samplerowsarg_init(_SampleRowsArg** arg, 
                         size_t low, 
                         size_t high, 
                         size_t seed, 
                         DataRowGroup* source, 
                         String* destination) {
    if (arg==NULL) {
        perror("Error: _SampleRowsArg ptr is null.\n");
        return;
    }
    if (low>high) {
        perror("Error: low must be <= high.\n");
    }
    if (source==NULL || destination==NULL) {
        perror("Error: Either source or destination ptr is null.\n");
        return;
    }

    *arg = (_SampleRowsArg*)malloc(sizeof(_SampleRowsArg));
    (*arg)->low = low;
    (*arg)->high = high;
    (*arg)->seed = seed;
    (*arg)->source = source;
    (*arg)->destination = destination;
}

void _samplerowsarg_destroy(void* arg) {
    if (arg==NULL) return;
    _SampleRowsArg* _arg = (_SampleRowsArg*)arg;
    free(_arg);
}

/// From the data from a datarow, generate a new datarow with temperature
/// sampled from a Gaussian distribution with mean as the temperature of the 
/// argument data and standard deviation STDDEV
DataRow sample_temperature(DataRow* data) {
    if (data==NULL) {
        perror("Error: null datarow pointer provided.");
        abort();
    }
    DataRow res;
    res.location = (String*)malloc(sizeof(String));
    *(res.location) = string_copy(data->location);
    res.temperature = data->temperature;
    double _x = (double)rand()/(double)(RAND_MAX);
    res.temperature = data->temperature + STDDEV * exp(-0.5*(_x * _x) / sqrt(2*M_PI));
    return res;
};

/// Sample n_sample rows with replacement from the provided DataRowGroup group,
/// sample temperatures for each row.
String* generate_random_temperature_sample_serial(DataRowGroup *group, size_t n_samples, size_t seed) {
    if (group==NULL) {
        perror("Error: null datarowgroup pointer provided.");
        abort();
    }
    if (n_samples == 0 || n_samples > ONE_BILLION) {
        perror("Error: n_samples must be between one and one billion.");
        abort();
    }

    srand(seed);

    String* res = (String*)calloc(n_samples, sizeof(String));
    DataRow row;

    // Generate indices to sample
    IntMatrix idxs = intmat_create(n_samples, 1);
    intmat_fill_random(&idxs, 0, group->num_rows, true, seed);

    // Generate data
    for (size_t i = 0; i < n_samples; ++i) {
        row = sample_temperature(&group->data[idxs.data[i]]);
        res[i] = format_datarow(&row);
        datarow_destroy(row);
    }

    // Destroy idx matrix
    intmat_destroy(&idxs);

    return res;
}

void* _sample_rows(void* arg) {
    _SampleRowsArg* _arg = (_SampleRowsArg*)arg;

    if (_arg->low>=_arg->high)
        return NULL;

    unsigned int seed = _arg->seed;
    DataRow row;

    // Generate data
    for (size_t i = _arg->low; i < _arg->high; ++i) {
        size_t idx = rand_r(&seed) % _arg->source->num_rows;
        row = sample_temperature(&(_arg->source->data[idx]));
        _arg->destination[i] = format_datarow(&row);
        datarow_destroy(row);
        seed *= i;
    }

    return NULL;
}

/// Sample n_sample rows with replacement from the provided DataRowGroup group,
/// sample temperatures for each row. This is done using multithreading.
String* generate_random_temperature_sample_threaded(DataRowGroup *group, size_t n_samples, size_t seed, size_t num_threads) {
    if (group==NULL) {
        perror("Error: null datarowgroup pointer provided.");
        abort();
    }
    if (n_samples == 0 || n_samples > ONE_BILLION) {
        perror("Error: n_samples must be between one and one billion.");
        abort();
    }
    if (num_threads==0) {
        perror("Error: num_threads must be at least 1.");
    }

    srand(seed);

    String* res = (String*)calloc(n_samples, sizeof(String));
    
    YATPool* pool;

    size_t fac = 16 * (size_t)log2(n_samples);
    fac = n_samples == 1 ? 1: fac;
    size_t num_tasks = n_samples / (fac * num_threads);
    num_tasks = (n_samples % (fac * num_threads) == 0)? num_tasks: num_tasks+1;

    yatpool_init(&pool, num_threads, num_tasks);

    for (size_t i=0; i<num_tasks; ++i) {
        Task* task;
        _SampleRowsArg* arg;
        size_t low = i * fac * num_threads;
        size_t high = (i + 1) * fac * num_threads;
        if (high > n_samples)
            high = n_samples;
        if (low > high) 
            low = high;
        _samplerowsarg_init(&arg, low, high, seed, group, res);
        task_init(&task, &_sample_rows, arg, &_samplerowsarg_destroy);

        yatpool_put(pool, task);
    }
    yatpool_wait(pool);

    yatpool_destroy(pool);

    return res;
}
