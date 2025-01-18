#include <sapi/embed/php_embed.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void write_response(int client_fd, const char *header)
{
    write(client_fd, header, strlen(header));
}

int main(int argc, char **argv)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {
        AF_INET,
        0x951f,
        0};

    bind(s, &addr, sizeof(addr));
    listen(s, 10);

    while (true)
    {

        int client_fd = accept(s, 0, 0);

        char buffer[256] = {0};
        recv(client_fd, buffer, 256, 0);

        write_response(client_fd, "HTTP/1.1 200 OK\n");
        write_response(client_fd, "Content-Length: 25\n");
        write_response(client_fd, "Content-Type: text/html\n\n");
        write_response(client_fd, "<h1>Hello, World!</h1>");

        close(client_fd);
    }

    close(s);

    // char *f = buffer + 5;
    // *strchr(f, ' ') = 0;
}

int execute_php(int argc, char **argv)
{
    PHP_EMBED_START_BLOCK(argc, argv)

    zend_file_handle file_handle;
    zend_stream_init_filename(&file_handle, "example.php");

    if (php_execute_script(&file_handle) == FAILURE)
    {
        php_printf("Failed to execute PHP script.\n");
    }

    PHP_EMBED_END_BLOCK()
}