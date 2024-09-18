/* Functions for formatting data types as strings.

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

#include "format.h"

/// Format a DataRow in the form of "foo;123.44"
String format_datarow(const DataRow* row) {
    // Format the data into the desired format
    char formatted[BUFSIZE];
    if (snprintf(formatted, sizeof(formatted), "%s;%.2f\n", row->location->data, row->temperature) < 0) {
        perror("Error formatting data row.");
        abort();
    }
    return string_create(formatted, strlen(formatted));
}
