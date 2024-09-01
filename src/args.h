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

#ifndef _ARGS_H_
#define _ARGS_H_

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Struct to hold all arguments
struct arguments {
    size_t n_rows;
    size_t seed;
    char raw_data_path[1024];
};

void init_arguments(struct arguments* arg_vals);
void print_arguments(struct arguments* arg_vals);

#endif // _ARGS_H_
