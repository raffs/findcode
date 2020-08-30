/**
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
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "findcode.h"

int
main(int argc, char* argv[])
{
    int rcode = 0;
    struct stat sb;

    if (argc < 2) {
        fprintf(stderr, "Error, please specify a <path>\n");
        fprintf(stderr, "Usage: %s <path> \n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {

        if (lstat(argv[i], &sb) == -1) {
            fprintf(stderr, "not found %s\n", argv[i]);
            return 2;
        }

        if (sb.st_mode & S_IFREG)
            parse_file(argv[i]);
        else {
            fprintf(stderr, "file '%s' is not regular file \n", argv[i]);
            rcode = 1;
        }
    }

    return rcode;
}
