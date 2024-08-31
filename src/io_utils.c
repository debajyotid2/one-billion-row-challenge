/* Utilities for reading, parsing and writing data.

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

#include "io_utils.h"

/// Parse a single row of data from a given input array
/// The data is expected to be present in the format of 
/// fooo;124.0
DataRow parse_single_row(char* row) {
    assert(row != NULL);
    char* tmp = row;
    int ctr = 0;
    DataRow row_data;
    
    // Parse location
    while(*tmp != '\n') {
        if (*tmp == ';') {
            row_data.location = string_create(row, ctr);
            ctr = 0;
            break;
        }
        tmp++;
        ctr++;
    }
    tmp++;

    // Parse temperature
    while(*tmp != '\n') {
        tmp++;
        ctr++;
    }

    String temp = string_create(tmp-ctr, ctr);
    row_data.temperature = atof(temp.data);
    string_destroy(temp);

    return row_data;
}

/// Parse raw data from a source file handle
DataRowGroup parse_raw_data(FILE* datafile) { 
    if (datafile == NULL)
        perror("File handle is a null pointer.");

    // Read the data line by line into DataRow objects
    size_t lineno = 0;
    DataRowGroup parsed_data;
    parsed_data.data = (DataRow*)calloc(DEFAULT_SIZE, sizeof(DataRow));
    parsed_data.num_rows = 0;
 
    // Buffer for reading lines
    char buf[BUFSIZE];
    memset(buf, 0x0, BUFSIZE * sizeof(char));
    
    while (fgets(buf, sizeof(buf), datafile)) {
        // Skip first two lines
        if (lineno < 2) {
            lineno++;
            continue;
        }
        // Parse a single row
        parsed_data.data[lineno-2] = parse_single_row(buf);
        lineno++;

        // Increase buffer space if end of buffer reached
        if ((lineno-2) % DEFAULT_SIZE == 0) {
            parsed_data.data = (DataRow*)realloc(parsed_data.data, ((lineno-2)+DEFAULT_SIZE) * sizeof(DataRow));
        }
    }
    parsed_data.num_rows = lineno-2;
    return parsed_data;
}




