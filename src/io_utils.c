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
#include "dtypes.h"

/// Parse a single row of data from a given input array
/// The data is expected to be present in the format of 
/// fooo;124.0
DataRow parse_single_row(const char* row) {
    assert(row != NULL);
    char* tmp = (char *)row;
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
    if (datafile == NULL) {
        perror("File handle is a null pointer.");
        abort();
    }

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

/// Format a DataRow in the form of "foo;123.44"
String format_datarow(const DataRow* row) {
    // Format the data into the desired format
    char formatted[BUFSIZE];
    if (snprintf(formatted, sizeof(formatted), "%s;%.2f", row->location.data, row->temperature) < 0) {
        perror("Error formatting data row.");
        abort();
    }

    printf("Formatted: %s\n", formatted);
    printf("Length of formatted: %zu\n", strlen(formatted));

    return string_create(formatted, strlen(formatted));
}

/// Write a DataRowGroup to a txt file
void write_datarowgroup(const DataRowGroup* group, const char* outfile) {
    if (group == NULL) {
        perror("Error: DataRowGroup pointer provided is null.");
        abort();
    }
   
    if (outfile == NULL) {
        perror("Error: Outfile name provided is null.");
        abort();
    }

    // Open outfile for writing
    FILE* out = fopen(outfile, "w");
    if (out == NULL) {
        perror("Error: Could not open output file to write.");
        abort();
    }
    
    // Format datarows and write to outfile
    for (size_t i = 0; i < group->num_rows; ++i) {
        String formatted = format_datarow(&group->data[i]);
        fprintf(out, "%s\n", formatted.data);
    }

    fclose(out);
}
