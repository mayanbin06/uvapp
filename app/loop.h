#ifndef APP_LOOP_H
#define APP_LOOP_H

#include <memory>
#include <iostream>

#include "uv.h"

namespace app {

class UvLoopDeleter {
 public:
  void operator()(uv_loop_t* loop) {
    std::cout << "closed == " << loop << std::endl;
    uv_loop_close(loop);
  }
};

// Singleton for each thread.
class Loop {
 private:
  // forbiden move constructor.
  Loop(Loop&&) = delete;
  // forbiden copy constructor.
  Loop(const Loop&) = delete;
  // forbiden assignment constructor.
  Loop& operator=(const Loop&) = delete;
  // forbiden move assignment constructor.
  Loop& operator=(Loop&&) = delete;
 public:
  // constructor function.
  Loop();
  // Get the loop of current thread, return nullptr if not initialize.
  static Loop* GetCurrentLoop();
  virtual ~Loop();
  virtual void Run();
  virtual void DispatchMessages();
 private:
  std::unique_ptr<uv_loop_t, UvLoopDeleter> loop_;
};

} // namespace app

#endif
