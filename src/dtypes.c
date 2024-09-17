/* Data types used in this application.

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

#include "dtypes.h"

/// Create a string by copying from
/// passed data pointer
String string_create(const char* data, int length) {
    if (data == NULL) {
        perror("Null pointer provided as argument.");
        abort();
    }
    if (length <= 0) {
        perror("Length cannot be less than 1.");
        abort();
    }

    // Create a string struct instance
    String string;
    string.data = (char *)calloc(length+1, sizeof(char));
    string.length = length;
    
    // Copy data
    memcpy(string.data, data, length);

    string.data[length] = '\0';
    
    return string;
}

/// Copy a String object
String string_copy(const String* string) {
    if (string == NULL) {
        perror("Null pointer provided as argument.");
        abort();
    }
    String newstr = string_create(string->data, string->length);
    return newstr;
}

/// Destroy a string object
void string_destroy(String string) {
    if (string.data == NULL) return;
    free(string.data);
}

/// Check if two strings are equal
bool string_equal(const String* str1, const String* str2) {
    if (str1==NULL || str2==NULL) {
        fprintf(stderr, "Null pointer provided.\n");
        exit(1);
    }
    return strcmp(str1->data, str2->data) == 0;
}


/// Print a String object
void string_print(const String* string) {
    if (string == NULL) return;
    printf("%s", string->data);
}

/// Print a DataRow object
void datarow_print(const DataRow* datarow) {
    if (datarow == NULL) return;
    printf("location: ");
    string_print(datarow->location);
    printf("temperature: %g\n", datarow->temperature);
}

/// Destroy a DataRow object
void datarow_destroy(DataRow datarow) {
    string_destroy(*(datarow.location));
    free(datarow.location);
}

/// Create a new DataRowGroup object
DataRowGroup datarowgroup_create(size_t num_rows) {
    if (num_rows == 0) {
        perror("Error: num_rows cannot be zero.");
        abort();
    }
    DataRowGroup res;
    res.data = (DataRow*)calloc(num_rows, sizeof(DataRow));
    res.num_rows = num_rows;

    return res;
};

/// Print a DataRowGroup object
void datarowgroup_print(const DataRowGroup* group) {
    if (group == NULL) return;
    for (size_t i = 0; i < group->num_rows; ++i)
        datarow_print(&group->data[i]);
}

/// Destroy a DataRowGroup object
void datarowgroup_destroy(DataRowGroup* datarowgroup) {
    if (datarowgroup == NULL) return;
    int ctr = (int)(datarowgroup->num_rows - 1);
    while (ctr >= 0) {
        datarow_destroy(datarowgroup->data[ctr]);
        ctr--;
    }
    free(datarowgroup->data);
}
