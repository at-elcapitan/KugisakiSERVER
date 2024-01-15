/*
 The GPLv3 License (GPLv3)

 Copyright (c) 2024 Vladislav 'ElCapitan' Nazarov (AT PROJECT)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef kugisaki_TYPES
#define kugisaki_TYPES

#include <stdlib.h>

struct Request {
  int socket;

  char request_type[5];
  char *request_file;
};

struct Response {
  char *response_header;
  char *response_content;
};

struct Request *create_request(int socket, int size);
void delete_reqest(struct Request *req);

#endif
