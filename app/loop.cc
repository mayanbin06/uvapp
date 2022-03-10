#include "loop.h"

#include "base/threading/thread_local.h"

namespace app {
static base::ThreadLocalOwnedPointer<Loop> pointer_;
// static
Loop* Loop::GetCurrentLoop() {
  return pointer_.Get();
}

Loop::Loop(): loop_(uv_default_loop()) {
  // bind to current thread.
  pointer_.Set(std::unique_ptr<Loop>(this));
  std::cout << "construct loop == " << this << std::endl;
  std::cout << "construct get loop == " << pointer_.Get() << std::endl;
}

Loop::~Loop() {
  std::cout << "destruct loop == " << this << std::endl;
}

void Loop::Run() {
  uv_run(loop_.get(), UV_RUN_DEFAULT);
}

void Loop::DispatchMessages() {
}

} // namespace app
