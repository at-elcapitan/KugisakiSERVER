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
#include "types.h"

struct Request *create_request(int socket, int size) {
  struct Request *req = (struct Request *)malloc(sizeof(struct Request));

  if (req == NULL) {
    log_error("Unable to allocate memory: %s", strerror(errno));
  }

  req->request_file = (char *)malloc(sizeof(char) * size);

  if (req->request_file == NULL) {
    log_error("Unable to allocate memory: %s", strerror(errno));
  }

  req->socket = socket;

  return req;
};

void delete_reqest(struct Request *req) {
  free(req->request_file);
  free(req);
};

void delete_response(struct Response *resp) {
  free(resp->response_content);
  free(resp->response_header);
  free(resp);
}
