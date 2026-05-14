#include <stdio.h> // for console printing
#include <stdlib.h> // exiting program 
#include <string.h> // string ops 
#include <unistd.h> // for read,write and sleep operations 
#include <fcntl.h> // for file control ops (read/write)
#include <errno.h> // error number specification 
#include <termios.h> // linux api 
#include <sys/select.h> // for receiving data without errors 

#define BAUDRATE B115200  // bits per second travelling through TX/RX
#define BUFFER_SIZE 256  // buffer size 

// Configure UART interface

int configure_uart(int fd)
{
    struct termios tty;

    // Read current UART setting
    
    if (tcgetattr(fd, &tty) != 0) // set the attributes of tty for our fd 
    {
        perror("tcgetattr");
        return -1;
    }

    // Set baud rate 
    
    cfsetispeed(&tty, BAUDRATE); // set the speed 
    cfsetospeed(&tty, BAUDRATE);

    /*
     * UART configuration:
     * 8 data bits
     * No parity
     * 1 stop bit
     */
    tty.c_cflag &= ~PARENB;     // No parity 
    tty.c_cflag &= ~CSTOPB;     // 1 stop bit 
    tty.c_cflag &= ~CSIZE;      // clear bits 
    tty.c_cflag |= CS8;         // 8 bits 

    // Enable receiver
    tty.c_cflag |= (CLOCAL | CREAD);

    // Disable hardware flow control 
    tty.c_cflag &= ~CRTSCTS;

    // raw iput mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Raw output 
    tty.c_oflag &= ~OPOST;

    // disable software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Non-blocking read settings
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;

    // applying setting
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int fd;
    const char *device;
    const char *tx_message =
        "UART Test message from Ubuntu to Device!\n";

    char rx_buffer[BUFFER_SIZE]; // to receive data

    if (argc != 2)  // args!=2 must be handled for error
    {
        fprintf(stderr,
                "Usage: %s <UART_DEVICE>\n",
                argv[0]);

        fprintf(stderr,
                "Example: %s /dev/ttyUSB0\n",
                argv[0]);

        return EXIT_FAILURE;
    }

    device = argv[1];

    // opens UART device for read,write
    
    fd = open(device, O_RDWR | O_NOCTTY);

    if (fd < 0)     // invalid FD handling
    {
        fprintf(stderr,
                "Failed to open %s\n",
                device);

        if (errno == ENOENT)
            fprintf(stderr,
                    "Error: Device not found\n");

        else if (errno == EACCES)
            fprintf(stderr,
                    "Error: Permission denied\n");

        else
            perror("open");

        return EXIT_FAILURE;
    }

    printf("UART device %s opened successfully\n",
           device);

    // device relaxation time becuase it was booting .
    sleep(2);

    // configure UART
    if (configure_uart(fd) != 0)
    {
        close(fd);
        return EXIT_FAILURE;
    }

    printf("UART configured successfully\n");

    // flush old UART data 
    tcflush(fd, TCIOFLUSH);

    // send messgae 
    ssize_t bytes_written =
        write(fd,
              tx_message,
              strlen(tx_message));

    if (bytes_written < 0)
    {
        perror("write");

        close(fd);
        return EXIT_FAILURE;
    }

    printf("Transmitted: %s", tx_message);


    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds); // clears out for new incoming data
    FD_SET(fd, &readfds);

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    printf("Waiting for incoming data...\n");

    int ret =
        select(fd + 1,
               &readfds,
               NULL,
               NULL,
               &timeout);
    
    // Handling invalid ret .
    if (ret < 0)
    {
        perror("select");

        close(fd);
        return EXIT_FAILURE;
    }
    else if (ret == 0)
    {
        printf("Timeout: No data received "
               "within 5 seconds\n");
    }
    else
    {
        if (FD_ISSET(fd, &readfds)) // final check for valid case
        {
           
            usleep(200000);  // time to receive complete msg (microsec)

            memset(rx_buffer,
                   0,
                   sizeof(rx_buffer));

            ssize_t bytes_read =
                read(fd,
                     rx_buffer,
                     sizeof(rx_buffer) - 1);

            if (bytes_read < 0)
            {
                perror("read");

                close(fd);
                return EXIT_FAILURE;
            }

            printf("Received (%ld bytes): %s\n",
                   bytes_read,
                   rx_buffer);
        }
    }

    close(fd);

    printf("UART device closed\n");

    return EXIT_SUCCESS;
}

/*
 
 *  - Opens UART device (/dev/ttyUSB0) in my case 
 *  - Configures UART settings
 *  - Sends test message
 *  - Receives reply using select() timeout
 *
 * build:
 *      gcc -Wall -Wextra -o uart_demo uart_termios.c
 *
 * run:
 *      ./uart_demo /dev/ttyUSB0
 */

 // Before all running C snippet , upload the INO code to the device and then run the C code in terminal.