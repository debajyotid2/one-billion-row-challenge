/* Utilities for reading, parsing and writing data

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

#ifndef _IOUTILS_H_
#define _IOUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "dtypes.h"
#include "format.h"

// Size of data buffer
#define BUFSIZE 256
// Default size of the initial DataRow buffer
#define DEFAULT_SIZE 10

DataRow parse_single_row(const char* row);
SADataRow sa_parse_single_row(const char* row);
DataRowGroup parse_raw_data(FILE* datafile);
void write_datarowgroup_serial(const String* data, size_t num_rows, const char* outfile);
void write_datarowgroup_threaded(const String* data, const char* outfile, size_t num_rows, size_t num_threads);

#endif // _IOUTILS_H_
