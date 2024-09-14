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

#ifndef _GENERATE_DATA_H_
#define _GENERATE_DATA_H_

// Standard deviation for the temperature distribution
#define STDDEV 10.0

#define ONE_BILLION 1000000000

#include <math.h>
#include <matrix.h>
#include <yatpool.h>
#include "format.h"
#include "dtypes.h"

DataRow sample_temperature(DataRow* data);
String* generate_random_temperature_sample_serial(DataRowGroup* group, size_t n_samples, size_t seed);
String* generate_random_temperature_sample_threaded(DataRowGroup* group, size_t n_samples, size_t seed, size_t num_threads);

#endif // _GENERATE_DATA_H_
