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
#include "dtypes.h"

// Size of data buffer
#define BUFSIZE 256
// Default size of the initial DataRow buffer
#define DEFAULT_SIZE 10

DataRow parse_single_row(char* row);
DataRowGroup parse_raw_data(FILE* datafile);

#endif // _IOUTILS_H_
