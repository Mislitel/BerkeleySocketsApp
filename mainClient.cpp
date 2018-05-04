#include "TcpClient.h"
#include "UdpClient.h"

// Test system
void test() {

    UdpClient udp;
    TcpClient tcp;
    udp.connectTo(DEFAULT_SERVER_IP, DEFAULT_UDP_PORT);
    tcp.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT);

    const char *test_strings[] = {
            "hello", "123456", "", "a3b5c7d8", "I'm the Fire!", "0", "999999999999999911111111111111188888888"
    };

    for(auto s: test_strings) {
        std::cout << "udp:" << std::endl << "c: " << s << std::endl;
        udp.singleSendReceive(s);
        std::cout << "tcp:" << std::endl << "c: " << s << std::endl;
        tcp.singleSendReceive(s);
    }

    udp.singleSendReceive(COMMAND_QUIT);
    tcp.singleSendReceive(COMMAND_QUIT);
    std::cout << "Success!" << std::endl;
}

void very_long_message_test() {
    //Client udp, tcp;
    UdpClient udp;
    TcpClient tcp;
    udp.connectTo(DEFAULT_SERVER_IP, DEFAULT_UDP_PORT);
    tcp.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT);

    char very_long_message[MAX_MESSAGE_SIZE];
    for(auto i = 0; i < MAX_MESSAGE_SIZE; i++)
        very_long_message[i] = '1';

    std::cout << "Very long message test:" << std::endl;
    std::cout << "udp:" << std::endl;
    udp.singleSendReceive(very_long_message);
    std::cout << "tcp:" << std::endl;
    tcp.singleSendReceive(very_long_message);

    udp.singleSendReceive(COMMAND_QUIT);
    tcp.singleSendReceive(COMMAND_QUIT);
    std::cout << "Success!" << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            TcpClient client;
            client.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT);
            client.cycleSendReceive();
            return 0;
        }
        if (argc == 2) {
            if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
                std::cout << "Usage:" << std::endl;
                std::cout << " client [option]" << std::endl;
                std::cout << " client [option] <ip> <port>" << std::endl;
                std::cout << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << " -h, --help\tdisplay this help and exit" << std::endl;
                std::cout << " -t, --tcp\tuse TCP for data transfer" << std::endl;
                std::cout << " -T, --test\trun test mode (default ip and port only) - need server in both-mode" << std::endl;
                std::cout << " -u, --udp\tuse UDP for data transfer" << std::endl;
                std::cout << " -v, --vlmt\trun test with very long message (default ip and port only) - need server in both-mode" << std::endl;
                return 0;
            }
            if (strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
                UdpClient client;
                client.connectTo(DEFAULT_SERVER_IP, DEFAULT_UDP_PORT);
                client.cycleSendReceive();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
                TcpClient client;
                client.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT);
                client.cycleSendReceive();
                return 0;
            }
            if (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-T") == 0) {
                test();
                return 0;
            }
            if (strcmp(argv[1], "--vlmt") == 0 || strcmp(argv[1], "-v") == 0) {
                very_long_message_test();
                return 0;
            }
        }
        if (argc == 3) {
            TcpClient client;
            client.connectTo(argv[1], atoi(argv[2]));
            client.cycleSendReceive();
            return 0;
        }
        if (argc == 4) {
            if (strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
                UdpClient client;
                client.connectTo(argv[2], atoi(argv[3]));
                client.cycleSendReceive();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
                TcpClient client;
                client.connectTo(argv[2], atoi(argv[3]));
                client.cycleSendReceive();
                return 0;
            }
        }
        std::cout << "Wrong parameters. Type 'client --help' for help" << std::endl;
        return 0;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}