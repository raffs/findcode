/*
 * findcode helps you locate lines of code inside your code base
 * Copyright (C) 2020  Rafael Oliveira Silva <rafaeloliveira.cs@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "findcode.h"

int
process_file(char *filepath, size_t filesize)
{
    int fd;
    int cur_line = 1, curbuf_start = 0, curbuf_end = 0;
    char *buffer;
    struct cblock_t *cblock;
    struct cqueue_t *cqueue;

    fd = open(filepath, O_RDONLY);
    if (!fd) {
        fprintf(stderr, "Error when open file: %s\n", filepath);
        return 1;
    }

    buffer = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buffer == MAP_FAILED) {
        close(fd);
        fprintf(stderr, "Error when open file: %s\n", filepath);
        return 1;
    }

    cqueue = init_cqueue();

    /* process each line */
    for (int i = 0; i < filesize; i++) {
        if (buffer[i] == '\n') {
            /*
             * Keep track on which line we are at, so we
             * we can return accurate line position
             */
            cur_line += 1;
        }
        else if (buffer[i] == '{') {
            cblock = init_cblock();
            cblock->name = substr_lastline(buffer, curbuf_start, curbuf_end);
            cblock->start_column = i;
            cblock->start_line = cur_line;
            cqueue_push(cqueue, cblock);

            curbuf_start = i + 1;
        }
        else if (buffer[i] == '}') {
            cblock = cqueue_pop(cqueue);

            if (cblock != NULL) {
                cblock->end_line = cur_line;
                cblock->end_column = i;

                print_block(cblock, filepath, 0);
                free_cblock(cblock);
            }
        }

        /* keeps tracking of which position of the buffer we are */
        curbuf_end += 1;
    }

    free_cqueue(cqueue);
    munmap(buffer, filesize);
    close(fd);

    return 0;
}

int
main(const int argc, char *argv[])
{
    struct stat file_sb;

    if (argc < 2) {
        fprintf(stderr, "Usage: findcode <filepath> <filepath> ...\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) { /* for each file */

        if (lstat(argv[i], &file_sb) == -1) {
            fprintf(stderr, "no able to stat() file %s\n", argv[i]);
            continue;
        }

        if (!(file_sb.st_mode & S_IFREG)) {
            fprintf(stderr, "file '%s' not a regular file\n", argv[i]);
            continue;
        }

        process_file(argv[i], (size_t)file_sb.st_size);
    }

    return 0;
}
