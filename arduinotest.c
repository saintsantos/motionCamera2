#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                //error_message ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
              //  error_message ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                //error_message ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        //if (tcsetattr (fd, TCSANOW, &tty) != 0)
                //error_message ("error %d setting term attributes", errno);
}

int main() {
  char *portname = "/dev/ttyACM2";
  int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0)
  {
          //error_message ("error %d opening %s: %s", errno, portname, strerror (errno));
          return -1;
  }

  set_interface_attribs (fd, B9600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  set_blocking (fd, 0);                // set no blocking

  int x_dummy = 400;
  int y_dummy = 400;
  while(1)  {

    ostringstream x_stream;
  	x_stream << x_dummy;
  	ostringstream y_stream;
  	y_stream << y_dummy;

  	string x_str = x_stream.str();
  	string y_str = y_stream.str();

    write(fd, "x", 1);
    usleep(100000);
    write (fd, x_str.c_str(), x_str.length());
    write(fd, "y", 1);
    usleep(100000);
    write(fd, y_str.c_str(), y_str.length());

    x_dummy += 1;
    y_dummy += 1;

    if (x_dummy > 600) {
      return -1;
    }
  }         // send 7 character greeting

  usleep ((1 + 25) * 100);             // sleep enough to transmit the 7 plus
                                       // receive 25:  approx 100 uS per char transmit
  char buf [100];
  int n = read (fd, buf, sizeof buf);  // read up to 100 characters if ready to read
}
