/* Argument parsing

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

#include "args.h"

/// Initialize arguments to defaults
void init_arguments(struct arguments* arg_vals) {
    arg_vals->n_rows = 1000000000;
    arg_vals->seed = 42;

    char data_path[] = "../data/weather_stations.txt";
    strncpy(arg_vals->raw_data_path, data_path, sizeof(data_path));
}

/// Print arguments
void print_arguments(struct arguments* arg_vals) {
    printf(
        "Arguments:\n"
        "n_rows = %zu, seed = %zu,\n"
        "raw_data_path = %s\n",
        arg_vals->n_rows, arg_vals->seed,
        arg_vals->raw_data_path
   );
}
