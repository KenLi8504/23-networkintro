#include "socket_networking.h"


/*=========================
  server_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  //use getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, "9845", hints, &results);  //Server sets node to NULL

  //create socket
  int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(listen_socket, results->ai_addr, results->ai_addrlen);
  listen(listen_socket,10);

  free(hints);
  freeaddrinfo(results);
  return listen_socket;
}

/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int listen_socket) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
  return client_socket;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake() {
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("127.0.0.1", "9845", hints, &results);  //Server sets node to NULL

  int connect_socket = socket(results->ai_family,results->ai_socktype,results->ai_protocol);
  connect(connect_socket, results->ai_addr, results->ai_addrlen);

  // Exiting Function
  free(hints);
  freeaddrinfo(results);
  return connect_socket;
}
