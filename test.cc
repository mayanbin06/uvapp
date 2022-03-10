#include <iostream>

#include <uv.h>

int main(int argc, char* argv[]) {
    uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    std::cout << "Now quitting." << std::endl;
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}
