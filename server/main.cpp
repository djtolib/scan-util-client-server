#include <iostream>
#include "server.h"
int main() {
    server my_server;
    try {
        my_server.run();
    }
    catch (runtime_error &err)
    {
        cout<<"Server running is failed. "<< err.what() <<endl;
    }
    return 0;
}
