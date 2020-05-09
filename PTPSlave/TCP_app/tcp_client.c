#include "tcp_client.h"
#include "lwip.h"
#include "lwip/api.h"
#include "socket.h"
#include "lwip/dns.h"
//#include "lwip/prot/dns.h"

void DNS_found_cb(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    uint32_t* ip = (uint32_t*)ipaddr;
    printf("DNS found: IP %d:%d:%d:%d\n",((*ip)&0xff),(((*ip)>>8)&0xff),(((*ip)>>16)&0xff),(((*ip)>>24)&0xff));
}

void network_init(struct netconn *conn)
{
    int sock = -1;
    conn = netconn_new(NETCONN_TCP);
    if (conn == NULL){
        printf("Set new connection failed...\r\n");
        return;
    }
    sock = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock < 0){
        printf("Get socket failed!... Try again...\r\n");
        vTaskDelay(1000);
    }
    conn->socket = sock;
}

int network_connect(struct netconn *conn, char *hostname, int port)
{
    int err = 0;
    struct sockaddr_in sAddr;
    ip_addr_t addr;
    printf("Socket is:%d\r\n",conn->socket);
    err = dns_gethostbyname(hostname, &addr, DNS_found_cb, NULL);
    if(err != 0){
        printf("DNS failed to get host by name : %d \n",err);
        return -1;
    }
    //addr.addr = htonl(addr.addr);
    sAddr.sin_addr.s_addr = *((in_addr_t*)(&addr));
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);

    //netconn_bind(conn,IP_ADDR_ANY,port); 
    err = lwip_connect(conn->socket, (struct sockaddr*)(&sAddr), sizeof(struct sockaddr));
    if(err != 0){
        printf("LwIP connect failed :%d \n", err);
        return -2;
    }
    return 0;

}

void tcp_client_task(void const * argument)
{
//    struct netbuf *buf;
//    void *data;
//    u16_t len;
    err_t err;
    struct netconn conn;

    LWIP_UNUSED_ARG(argument);

    char ip4[] = "192.168.1.233";
    uint16_t port = 30001;

    printf("Connecting to server: %s :%d ...\r\n",ip4,port);
    network_init(&conn);

CONNECT:
    err = network_connect(&conn, ip4, port);
    if (err != ERR_OK){
        printf("Connect to server failed:%d \tReconnecting...\r\n",err);
//        printf("Socket is:%d\r\n",conn->socket);
        vTaskDelay(3000);
        goto CONNECT;
    }
    printf("Connect to server successed!\r\n");

    //netconn_bind(conn,IP_ADDR_ANY,port); 
    while(1)
    {
        vTaskDelay(2000);
    }
}
