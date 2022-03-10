#include "server.h"
#include <string>
#include <functional>

namespace internal {

// c++ 14.
//template<typename T, typename Ret, typename ... Args>
//auto TraitsClassMethod(const T *t, Ret(T::*handler)(Args...))
//{
//    return [&](Args... args) {
//        return (t->*handler)(args);
//    };
//}
//
//
//  Apple* apple = new Apple;
//  // 封装回调函数
//  auto func = TraitsClassMethod(apple, &Apple::test);
//  func(...);

}

namespace base {

Server::~Server() {
  uv_loop_close(uv_default_loop());
}

void Server::OnAllocBuffer(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
  // 内存管理模型
  void* mem = malloc(suggested_size);
  if (mem == nullptr) {
      buf->len = 0;
      return;
  }
  buf->base = static_cast<decltype(buf->base)>(mem);
  buf->len = suggested_size;
}

void Server::OnPeerRead(uv_stream_t* client, ssize_t read, const uv_buf_t* buf) {
  return;
}

void Server::OnClientClosed(uv_handle_t* handle) {
  uv_tcp_t* client = reinterpret_cast<uv_tcp_t*>(handle);
  if (client->data != nullptr) {
      PeerState* state = reinterpret_cast<PeerState*>(client->data);
      delete state;
  }
  free(client);
}

void Server::OnPeerConnected(uv_stream_t* server, int status) {
  if (status < 0) {
    return;
  }

  uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  int rc;
  if ((rc = uv_tcp_init(uv_default_loop(), client)) < 0) {
    return;
  }
  client->data = nullptr;

  if (uv_accept(server, (uv_stream_t*)client) == 0) {
    struct sockaddr_storage peername;
    int namelen = sizeof(peername);
    if ((rc = uv_tcp_getpeername(client,
        (struct sockaddr*)&peername,
        &namelen)) < 0) {
      return;
    }
    //report_peer_connected((const struct sockaddr_in*)&peername, namelen);

    // Initialize the peer state for a new client.
    PeerState* peerstate = new PeerState;
    peerstate->buffer_end_= 0;
    client->data = static_cast<decltype(client->data)>(peerstate);

    // Start reading on the peer socket.
    if ((rc = uv_read_start((uv_stream_t*)client, &Server::OnAllocBuffer,
                            &Server::OnPeerRead)) < 0) {
      return;
    }
  } else {
    //uv_close_cb cb = std::bind(std::mem_fn(&Server::OnClientClosed), this, std::placeholders::_1);
    uv_close((uv_handle_t*)client, &Server::OnClientClosed);
  }
}

int Server::Start() {
  int rc = 0;
  rc = uv_tcp_init(uv_default_loop(), &uv_tcp_server_);
  if (rc < 0) {
    return -1;
  }

  rc = uv_ip4_addr("0.0.0.0", port_, &addr_);
  if (rc < 0) {
    return -1;
  }

  rc = uv_tcp_bind(&uv_tcp_server_, &addr_, 0);
  if (rc < 0) {
    return -1;
  }

  rc = uv_listen((uv_stream_t*)&uv_tcp_server_, backlog_, &Server::OnPeerConnected);
  if (rc < 0) {
    return -1;
  }
  // start the loop.
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
} 
} // namespace base
