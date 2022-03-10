#include "app/loop.h"
#include <thread>

int main(int argc, char* argv[]) {

    std::thread t([]()->void {
        new app::Loop;
        app::Loop* loop = app::Loop::GetCurrentLoop();
        std::cout << "get loop of thread = " << loop << std::endl;
    });
    //app::Loop* loop = app::Loop::GetCurrentLoop();
    //loop->Run();
    t.join();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}