#include "mbed.h"

#define SERVER_IP "172.17.198.60"
#define SERVER_PORT 50000

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
WiFiInterface *wifi;

TCPSocket socket;
Thread sock_thread;
char rxBuf_pc[80];
char txBuf_pc[80];

int index = 0;
volatile int flag ;

void rx_cb (void)
{
  char ch;
  pc.read(&ch, 1);
  pc.write(&ch, 1);
  rxBuf_pc[index++] = ch;
  if (ch == '\r') {
    pc.write("\n", 1);
    rxBuf_pc[--index] = '\0'; //end of string
    index = 0;
    flag = 1;
  }
}

// rx_thread : a thread to receive data from the server
void rx_thread (){
  char* buf = (char*)malloc(200);
  while (true){
    nsapi_size_or_error_t size = socket.recv(buf, 200); // recv data

    if (size <= 0) {
      if (size == NSAPI_ERROR_WOULD_BLOCK) continue;

      sprintf(txBuf_pc , "Error while receiving data from TCP socket (%d)\r\n", size);
      pc.write(txBuf_pc, strlen(txBuf_pc));
      return;
    }
    buf[size] = '\0';

    sprintf(txBuf_pc , "\r\nRX data: (%d) %s \r\n", size, buf);
    pc.write(txBuf_pc , strlen(txBuf_pc));
  }
}

int main()
{
  SocketAddress sockAddr;
  SocketAddress serverAddr(SERVER_IP, SERVER_PORT);

  sprintf(txBuf_pc, "\r\nWifi TCP Client example\r\n");
  pc.write(txBuf_pc, strlen(txBuf_pc));
  pc.attach(rx_cb);

  wifi = WiFiInterface::get_default_instance();
  if(!wifi){
    sprintf(txBuf_pc, "ERROR: No WiFiInterface found.\n");
    pc.write(txBuf_pc, strlen(txBuf_pc));
    while(1){ };
  }

  sprintf(txBuf_pc, "Connecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
  pc.write(txBuf_pc, strlen(txBuf_pc));

  int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
  if(ret!=0){
    sprintf(txBuf_pc, "Connection error!!\r\n");
    pc.write(txBuf_pc, strlen(txBuf_pc));
    return -1;
  }

  sprintf(txBuf_pc, "\r\n");
  pc.write(txBuf_pc, strlen(txBuf_pc));

  sprintf(txBuf_pc, "RSSI: %d\r\n", wifi->get_rssi());
  pc.write(txBuf_pc, strlen(txBuf_pc));

  sprintf(txBuf_pc, "MAC: %s\r\n", wifi->get_mac_address());
  pc.write(txBuf_pc, strlen(txBuf_pc));

  wifi->get_ip_address(&sockAddr);
  sprintf(txBuf_pc, "IP: %s, ", sockAddr.get_ip_address());
  pc.write(txBuf_pc, strlen(txBuf_pc));

  wifi->get_netmask(&sockAddr);
  // pc.printf ("Netmask: %s, ", sockAddr.get_ip_address
  sprintf(txBuf_pc, "Netmask: %s, ", sockAddr.get_ip_address());
  pc.write(txBuf_pc, strlen(txBuf_pc));

  wifi->get_gateway(&sockAddr);
  sprintf(txBuf_pc, "Gateway: %s\r\n", sockAddr.get_ip_address());
  pc.write(txBuf_pc, strlen(txBuf_pc));

  // Open a TCP socket on the network interface, and create a TCP connection to wa Server
  socket.open(wifi);

  int response = socket.connect(serverAddr);
  if(0 != response) {
    sprintf(txBuf_pc, "Error connecting: %d\r\n", response);
    pc.write(txBuf_pc, strlen(txBuf_pc));
    socket.close();
    return 1;
  }

  sock_thread.start(&rx_thread);

  while (true) {
    flag = 0;
    sprintf(txBuf_pc, "Enter characters to send to a server:");
    pc.write(txBuf_pc, strlen(txBuf_pc));
    while (flag != 1) {
    }

    if (!strcmp(rxBuf_pc, "q") || !strcmp(rxBuf_pc, "Q")){
      break;
    } else {
      int len = strlen(rxBuf_pc);
      sprintf(txBuf_pc, "Sent: %s (%d)\r\n", rxBuf_pc, len);
      pc.write(txBuf_pc, strlen(txBuf_pc));
      rxBuf_pc[len++] = '\r';
      rxBuf_pc[len++] = '\n';
      socket.send((const char *)rxBuf_pc, len); // for printing of SocketTest
      ThisThread::sleep_for(500ms); // 1sec
    }
  }

  socket.close();
  wifi->disconnect();

  sock_thread.join();
  sprintf(txBuf_pc, "RX sock_thread joined!!\r\n");
  pc.write(txBuf_pc, strlen(txBuf_pc));
  sprintf(txBuf_pc, "\nDone\r\n");
  pc.write(txBuf_pc, strlen(txBuf_pc));
  while(1) { };
}
