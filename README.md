# TLS_Server_and_client_using_openssl
Demonstration of TLS client and server using openssl. 

# How to make a TLS client connection using OpenSSL.
One way to make a TLS client connection with OpenSSL is to use its SSL layer on top of a TCP connection made using the Berkeley sockets API.  The TCP connection to the server is made in the usual way.  For the TLS connection, an SSL context must be created first.  This manages the certificates, and sets the TLS algorithm to use.  For a TLS client connection, a certificate from a certificate authority is required.  If the right certificate is known, it can be loaded directly into the context.  In many cases, the SSL context is linked to a certificate store on the system and OpenSSL will find the right certificate automatically.  Once the SSL context is setup, a TCP socket can be created along with an SSL session.  The SSL session is linked to the SSL context and the socket.  Most interaction with the TLS connection will go through the SSL session.

After the TCP socket is connected, the next step is the TLS handshake.  OpenSSL handles all of the details of the handshake.  The client application only needs to poll the handshake status until either an error or success is returned.  If the handshake succeeds, the SSL session can be used to do encrypted reads and writes to the server.  To do this, the client calls read and write functions on the SSL session that take plain data as input and return plain data as output.  The SSL read and write functions handle the details of encryption and decryption.  When the client is finished with the connection, the TCP socket can be closed and the SSL session freed.

# How to make a TLS server connection using OpenSSL.
One way to make a TLS server connection with OpenSSL is to use its SSL layer on top of a TCP connection made using the Berkeley sockets API.  Creating the TCP server connection is done in the usual way.  For the TLS connection, the first requirement is to create an SSL context that tells which TLS version to use, and manages certificates.  A TLS server requires a server certificate and a key, both of which are loaded into the SSL context.  When a new TCP connection is received, an SSL session can be created using the SSL context.  The SSL session is associated with the socket for the new connection.  After this association is made, most of the interaction with the socket should go through the SSL session.

Once the TCP socket is connected, the next step is the TLS handshake.  OpenSSL handles all of the details of the handshake.  The server application only needs to poll the handshake status until either an error or success is returned.  If the handshake succeeds, the SSL session can then be used to do encrypted reads and writes on the socket.  OpenSSL handles the details of encryption and decryption, so the server application exchanges clear-text data with the SSL session read and write functions.  When the server is finished with the connection, the TCP socket can be closed and the SSL session freed.

# Make sure you have OpenSSL installed on your system and compile the programs with the following command:
*   gcc server.c -o server -lssl -lcrypto
*   gcc client.c -o client -lssl -lcrypto

# Certificates:
We need a server certificate (server.crt) and a private key (server.key) in PEM format.

# Generate a self-signed server certificate (server.crt) and private key (server.key) in PEM format using OpenSSL:
1.  Generate a private key: Use the OpenSSL command-line tool to generate a private key. The following command will generate a 2048-bit RSA private key and save it in a file called server.key:
*       openssl genpkey -algorithm RSA -out server.key -aes256
    This command will prompt you to enter a passphrase to protect the private key. Make sure to remember this passphrase as you'll need it later.
2.  This command will prompt you to enter a passphrase to protect the private key. Make sure to remember this passphrase as you'll need it later.
*       openssl req -new -key server.key -out server.csr
    Follow the prompts to enter the necessary information, such as the Common Name (CN) for the server. The CN should be the fully qualified domain name (FQDN) of your server.
3.  Generate a self-signed certificate: Use the private key and CSR to generate a self-signed certificate. The following command will generate a self-signed certificate that is valid for 365 days and save it in a file called server.crt:
*       openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
    The certificate is now signed with the private key, and you have a self-signed certificate (server.crt) and private key (server.key) in PEM format.
4.  Protect the private key: Since the private key is sensitive information, it's recommended to protect it with proper file permissions. You can set the appropriate permissions with the following command:
*       chmod 400 server.key
    This ensures that only the owner of the file has read and write permissions for the private key.
5.  Now we can use the server.crt and server.key files in our SSL/TLS server application. Remember to update the paths to these files in our server code accordingly.
