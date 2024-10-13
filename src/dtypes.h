/* Data types used in this application

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

#ifndef _DTYPES_H_
#define _DTYPES_H_

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define MAX_STRINGSIZE 1024

typedef struct String {
    char* data;
    size_t length;
} String;

typedef struct {
    char data[MAX_STRINGSIZE];
    size_t length;
} SAString;

typedef struct DataRow {
    String* location;
    double temperature;
} DataRow;

/// Stack allocated data row
typedef struct {
    SAString location;
    double temperature;
} SADataRow;

// Struct to hold a group of DataRows
typedef struct DataRowGroup {
    DataRow* data;
    size_t num_rows;
} DataRowGroup;

String string_create(const char* data, int length);
String string_copy(const String* string);
bool string_equal(const String* str1, const String* str2);
void string_destroy(String string);
void string_print(const String* string);
SADataRow sa_datarow_create(const char* data, int length);
void sa_datarow_print(const SADataRow* datarow);
void datarow_print(const DataRow* datarow);
void datarow_destroy(DataRow datarow);
DataRowGroup datarowgroup_create(size_t num_rows);
void datarowgroup_print(const DataRowGroup* group);
void datarowgroup_destroy(DataRowGroup* datarowgroup);

#endif // _DTYPES_H_
