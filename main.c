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

#include "src/dtypes.h"
#include "src/io_utils.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <PATH TO DATA FILE>\n", argv[0]);
        return 1;
    }

    FILE* datafile = fopen(argv[1], "r");

    // Exit if file could not be opened
    if (datafile == NULL) {
        fprintf(stderr, "Could not read %s.\n", argv[1]);
        return 2;
    }
    
    // Parse raw data
    DataRowGroup parsed_data = parse_raw_data(datafile);
    
    // Random sample with replacement from parsed data

    // Release all buffers
    datarowgroup_destroy(&parsed_data);
    
    fclose(datafile);
}
