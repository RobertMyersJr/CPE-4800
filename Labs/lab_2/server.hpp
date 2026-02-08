#pragma once
#include <string>

class Server {
    public:
        Server();

        /**
         * @brief Start receiving messages. The program control is transferred to this function and will not return.
         */
        void start_receiving();
    private:
        void send_message(std::string message);
        int sock_;
        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_;
};
