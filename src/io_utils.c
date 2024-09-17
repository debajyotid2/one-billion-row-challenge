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
#include <yatpool.h>

typedef struct {
    const String* lines;
    size_t start_lineno;
    size_t end_lineno;
    size_t* offset_ptr;
} _GetOffsetArg;

typedef struct {
    char* mapped_file;
    const String* lines;
    size_t start_lineno;
    size_t end_lineno;
    size_t offset;
} _WriteToFileArg;

void _getoffsetarg_init(_GetOffsetArg** arg, 
                       const String* lines, 
                       size_t start_lineno,
                       size_t end_lineno,
                       size_t* offset_ptr) {
    if (arg==NULL) return;
    if (lines==NULL) return;
    *arg = (_GetOffsetArg*)malloc(sizeof(_GetOffsetArg));
    (*arg)->lines = lines;
    (*arg)->start_lineno = start_lineno;
    (*arg)->end_lineno = end_lineno;
    (*arg)->offset_ptr = offset_ptr;
}

void _getoffsetarg_destroy(void* arg) {
    if (arg==NULL) return;
    _GetOffsetArg* _arg = (_GetOffsetArg*)arg;
    free(_arg);
}

void _writetofilearg_init(_WriteToFileArg** arg,
                         char* mapped,
                         const String* lines,
                         size_t start_lineno,
                         size_t end_lineno,
                         size_t offset) {
    if (arg==NULL || mapped==NULL || lines==NULL) return;
    *arg = (_WriteToFileArg*)malloc(sizeof(_WriteToFileArg));
    (*arg)->mapped_file = mapped;
    (*arg)->lines = lines;
    (*arg)->start_lineno = start_lineno;
    (*arg)->end_lineno = end_lineno;
    (*arg)->offset = offset;
}

void _writetofilearg_destroy(void* arg) {
    if (arg==NULL) return;
    _WriteToFileArg* _arg = (_WriteToFileArg*)arg;
    free(_arg);
}

/// Function for threadpool to calculate the offset required for a group of lines
void* _get_offset(void* arg) {
    _GetOffsetArg* offsetarg = (_GetOffsetArg*)arg;
    *(offsetarg->offset_ptr) = 0;
    for (size_t i = offsetarg->start_lineno; i < offsetarg->end_lineno; ++i)
        *(offsetarg->offset_ptr) += offsetarg->lines[i].length;
    return NULL;
}

/// Function for threadpool to write a chunk of lines into file
void* _write_to_file(void* arg) {
    _WriteToFileArg* currarg = (_WriteToFileArg*)arg;

    size_t running_total_bytes = 0;
    currarg->mapped_file += currarg->offset;

    for (size_t i = currarg->start_lineno; i < currarg->end_lineno; ++i) {
        memcpy(currarg->mapped_file+running_total_bytes, currarg->lines[i].data, currarg->lines[i].length);
        running_total_bytes += currarg->lines[i].length;
    }
    return NULL;
}

/// Parse a single row of data from a given input array
/// The data is expected to be present in the format of 
/// fooo;124.0
DataRow parse_single_row(const char* row) {
    assert(row != NULL);
    char* tmp = (char *)row;
    int ctr = 0;
    DataRow row_data;
    row_data.location = (String*)malloc(sizeof(String));
    
    // Parse location
    while(*tmp != '\n') {
        if (*tmp == ';') {
            *(row_data.location) = string_create(row, ctr);
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

/// Write a DataRowGroup to a txt file
void write_datarowgroup_serial(const String* data, size_t num_rows, const char* outfile) {
    if (data == NULL) {
        perror("Error: Data pointer provided is null.");
        abort();
    }
   
    if (outfile == NULL) {
        perror("Error: Outfile name provided is null.");
        abort();
    }

    if (num_rows==0) {
        perror("Error: num_rows provided must be non-zero.");
        abort();
    }

    // Open outfile for writing
    FILE* out = fopen(outfile, "w");
    if (out == NULL) {
        perror("Error: Could not open output file to write.");
        abort();
    }
    
    // Format datarows and write to outfile
    for (size_t i = 0; i < num_rows; ++i) {
        fprintf(out, "%s\n", data[i].data);
    }

    fclose(out);
}

/// Write a DataRowGroup to a txt file using multithreading
void write_datarowgroup_threaded(const String* data, const char* outfile, size_t num_lines, size_t num_threads) {
    if (data == NULL) {
        perror("Error: Data pointer provided is null.");
        abort();
    }
   
    if (outfile == NULL) {
        perror("Error: Outfile name provided is null.");
        abort();
    }
    if (num_threads==0) {
        perror("Error: num_threads cannot be zero.");
        abort();
    }
    if (num_lines==0) {
        perror("Error: num_lines cannot be zero.");
        abort();
    }

    YATPool* pool;

    size_t fac = 8 * num_threads;
    size_t num_tasks = num_lines / fac;
    num_tasks = num_lines % fac == 0 ? num_tasks: num_tasks + 1;
    
    yatpool_init(&pool, num_threads, num_tasks);

    size_t* offsets = (size_t*)calloc(num_tasks, sizeof(size_t));
    memset(offsets, 0, num_tasks * sizeof(size_t));
    
    for (size_t i=0; i<num_tasks; ++i) {
        Task* task;
        _GetOffsetArg* arg;
        size_t start_lineno = fac * i;
        size_t end_lineno = fac * (i + 1);
        end_lineno = end_lineno > num_lines ? num_lines: end_lineno;
        _getoffsetarg_init(&arg, data, start_lineno, end_lineno, &offsets[i]);

        task_init(&task, &_get_offset, arg, &_getoffsetarg_destroy);
        yatpool_put(pool, task);
    }

    yatpool_wait(pool);
    yatpool_destroy(pool);

    for (size_t i=1; i<num_tasks; ++i)
        offsets[i] += offsets[i-1];
 
    // Open outfile for writing
    int fd = open(outfile, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd==-1) {
        perror("Error: could not open file for writing.");
        abort();
    }
   
    // Set file size
    size_t file_size = offsets[num_tasks-1];
    if (ftruncate(fd, file_size)==-1) {
        perror("Error: could not truncate file to specific length.");
        close(fd);
        return;
    }

    char* file_buf = (char*)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_buf==MAP_FAILED) {
        perror("Error: mmap error.");
        close(fd);
        return;
    }

    yatpool_init(&pool, num_threads, num_tasks);

    for(size_t i=0; i<num_tasks; ++i) {
        Task* task;
        _WriteToFileArg* arg;
        size_t start_lineno = fac * i;
        size_t end_lineno = fac * (i + 1);
        end_lineno = end_lineno > num_lines ? num_lines: end_lineno;
        size_t offset = (i == 0) ? 0: offsets[i-1];

        _writetofilearg_init(&arg, file_buf, data, start_lineno, end_lineno, offset);

        task_init(&task, &_write_to_file, arg, &_writetofilearg_destroy);
        yatpool_put(pool, task);
    }

    yatpool_wait(pool);
    yatpool_destroy(pool);

    munmap(file_buf, file_size);
    close(fd);

    free(offsets);
}
