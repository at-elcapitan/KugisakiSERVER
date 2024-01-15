#include "types.h"

struct Request *create_request(int socket, int size) {
  struct Request *req = (struct Request *)malloc(sizeof(struct Request));

  req->request_file = (char *)malloc(sizeof(char) * size);
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
