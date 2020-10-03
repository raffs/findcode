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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "findcode.h"

void
help_usage()
{
    fprintf(stdout, "Usage: findcode [OPTIONS] <filepath> <filepath> ...\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "OPTIONS:\n");
    fprintf(stdout, "    --help             show this message\n");
    fprintf(stdout, "    -n|--line-numbers  show line numbers\n");
    fprintf(stdout, "\n");

    exit(1);
}

/*
 * Returns a cmd_options struct with the command line options and
 * default values set.
 * Caller are responsible to free the memory space after usage.
 */
struct cmd_options*
parse_opts(const int argc, char *argv[])
{
    int option;
    int option_index = 0;
    struct cmd_options *opts;

    static struct option cmd_options[] = {
        {"line-numbers", no_argument, 0, 'n'},
        {"help",         no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    opts = (struct cmd_options *) malloc(sizeof(struct cmd_options));
    if (!opts)
        die("OutOfMemory: could not allocate memory for parse_options()");
    memset(opts, 0, sizeof(struct cmd_options));

    while (1) {
        option = getopt_long(argc, argv, "n", cmd_options, &option_index);

        if (option == -1)
            break;

        if (option == 'n')
            opts->line_numbers = 1;

        if (option == '?' || option == 'h') {
            free(opts);
            help_usage();
        }
    }

    return opts;
}

int
process_file(char *filepath, size_t filesize, struct cmd_options *cmd_opts)
{
    FILE *fp;
    char *buffer;
    int n_read;
    int flags = 0;
    int cur_line = 1, curbuf_start = 0, curbuf_end = 0;
    struct cblock_t *cblock;
    struct cqueue_t *cqueue;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error when open file: %s\n", filepath);
        return 1;
    }

    cqueue = init_cqueue();

    buffer = (char *) malloc(sizeof(char) * (filesize + 1));
    n_read = fread(buffer, sizeof(char), filesize, fp);

    if (n_read == 0 || n_read != filesize) {
        free_cqueue(cqueue);
        fclose(fp);
        free(buffer);
        return -1;
    }

    /*
     * setup the flags that will be passed to the helper functions
     */
    if (cmd_opts->line_numbers)
        flags |= CBLOCK_LINE_NUMBERS;

    /* process each line */
    for (int i = 0; i < n_read; i++) {

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

                print_block(cblock, filepath, flags);
                free_cblock(cblock);
            }
        }

        /* keeps tracking of which position of the buffer we are */
        curbuf_end += 1;
    }

    free_cqueue(cqueue);
    fclose(fp);
    free(buffer);

    return 0;
}

int
main(const int argc, char *argv[])
{
    char* file;
    struct stat file_sb;
    struct cmd_options *cmd_opts;

    if (argc < 2)
        help_usage();

    cmd_opts = parse_opts(argc, argv);

    while (optind < argc) {
        file = argv[optind++];

        if (lstat(file, &file_sb) == -1) {
            fprintf(stderr, "no able to stat() file %s\n", file);
            continue;
        }

        if (!(file_sb.st_mode & S_IFREG)) {
            fprintf(stderr, "file '%s' not a regular file\n", file);
            continue;
        }

        process_file(file, (size_t)file_sb.st_size, cmd_opts);
    }

    free(cmd_opts);
    return 0;
}
