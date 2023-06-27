#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

#define RTSP_SERVER_ADD "10.10.22.4"
#define RTSP_PORT 8554

int main (int argc, char *argv[])
{
    int s, len, result;
    struct sockaddr_in srv_addr;
    char buf[64];
    char tmpBuff[50];
    SSL_CTX *ctx;
    SSL *ssl;

    /* Create a TLS client context with a CA certificate */
    ctx = SSL_CTX_new(TLS_client_method());
    SSL_CTX_use_certificate_file(ctx, "ca.crt", SSL_FILETYPE_PEM);

    /* Set the address and port of the server to connect to */
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = 9001;
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

    /* Create a socket and SSL session */
    s = socket(AF_INET, SOCK_STREAM, 0);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, s);

    /* Try to connect */
    result = connect(s, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (result == 0) {
        /* Run the OpenSSL handshake */
        result = SSL_connect(ssl);

        /* Exchange some data if the connection succeeded */
        if (result == 1) {
            sprintf(buf, "Hello from the client");
            SSL_write(ssl, buf, strlen(buf) + 1);
            len = SSL_read(ssl, buf, sizeof(buf));
            printf("Received message from server: %s\n", buf);
            printf("Run VLC Media Player\n");
            //Run VLC Media Player
            sprintf(tmpBuff,"vlc rtsp://%s:%d/test",RTSP_SERVER_ADD,RTSP_PORT);
            system(tmpBuff);
            
        }
    }
    printf("Close VLC Media Player\n");
    system("killall -9 vlc");
    /* Done */
    close(s);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;
}
