#ifndef _SERVER_H
#define _SERVER_H

#include "uv.h"

namespace base {

class PeerState {
public:
  uint64_t number_;
  uv_tcp_t* client_;
  char buffer_[1024];
  int buffer_end_;
};

class Server {
public:
  enum STATE {START, STOP, RUNING, ERROR};

  Server(int port): port_(port), backlog_(64) {}
  virtual ~Server();
  virtual int Start();
protected:
  // uv callback functions.
  static void OnPeerConnected(uv_stream_t*, int);
  static void OnAllocBuffer(uv_handle_t*, size_t, uv_buf_t*);
  static void OnPeerRead(uv_stream_t*, ssize_t, const uv_buf_t*);
  static void OnClientClosed(uv_handle_t*);
private:
  uv_tcp_t uv_tcp_server_;
  uv_loop_t* loop_;
  int32_t port_;
  struct sockaddr_in addr_;
  int32_t backlog_;
};

} // namespace base

#endif
