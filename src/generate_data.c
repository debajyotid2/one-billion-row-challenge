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
#include "dtypes.h"
#include <matrix.h>

/// From the data from a datarow, generate a new datarow with temperature
/// sampled from a Gaussian distribution with mean as the temperature of the 
/// argument data and standard deviation STDDEV
DataRow sample_temperature(DataRow* data) {
    if (data==NULL) {
        perror("Error: null datarow pointer provided.");
        abort();
    }
    DataRow res = { string_copy(&data->location), data->temperature };
    double _x = (double)rand()/(double)(RAND_MAX);
    res.temperature = data->temperature + STDDEV * exp(-0.5*(_x * _x) / sqrt(2*M_PI));
    return res;
};

/// Sample n_sample rows with replacement from the provided DataRowGroup group,
/// sample temperatures for each row and output as a new DataRowGroup.
DataRowGroup generate_random_temperature_sample(DataRowGroup *group, size_t n_samples, size_t seed) {
    if (group==NULL) {
        perror("Error: null datarowgroup pointer provided.");
        abort();
    }
    if (n_samples == 0 || n_samples > ONE_BILLION) {
        perror("Error: n_samples must be between one and one billion.");
        abort();
    }

    srand(seed);

    // Create a new DataRowGroup
    DataRowGroup res = datarowgroup_create(n_samples);

    // Generate indices to sample
    IntMatrix idxs = intmat_create(n_samples, 1);
    intmat_fill_random(&idxs, 0, group->num_rows, true, seed);

    // Generate data
    for (size_t i = 0; i < n_samples; ++i) {
        res.data[i] = sample_temperature(&group->data[idxs.data[i]]);
    }

    // Destroy idx matrix
    intmat_destroy(&idxs);

    return res;
}
