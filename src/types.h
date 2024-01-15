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
