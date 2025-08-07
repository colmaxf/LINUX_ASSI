# Working with IPC Sockets

This project demonstrates sending and receiving a simple string using four different types of sockets:
*   IPv4 Stream Socket (TCP)
*   IPv4 Datagram Socket (UDP)
*   Unix Stream Socket
*   Unix Datagram Socket

## Socket Characteristics Comparison

| Characteristic | IPv4 Stream (TCP) | IPv4 Datagram (UDP) | Unix Stream | Unix Datagram |
| :--- | :--- | :--- | :--- | :--- |
| **Domain** | `AF_INET` | `AF_INET` | `AF_UNIX` | `AF_UNIX` |
| **Type** | `SOCK_STREAM` | `SOCK_DGRAM` | `SOCK_STREAM` | `SOCK_DGRAM` |
| **Connection** | Connection-oriented (`connect`, `accept`) | Connectionless | Connection-oriented (`connect`, `accept`) | Connectionless |
| **Reliability** | High (Reliable, ordered) | Low (Unreliable, unordered) | High (Reliable, ordered) | High (Reliable, but datagram-based) |
| **Address** | IP & Port (`sockaddr_in`) | IP & Port (`sockaddr_in`) | File path (`sockaddr_un`) | File path (`sockaddr_un`) |
| **Send/Receive** | `send`/`recv` or `write`/`read` | `sendto`/`recvfrom` | `send`/`recv` or `write`/`read` | `sendto`/`recvfrom` |
| **Scope** | Between machines on a network | Between machines on a network | On the same machine | On the same machine |
| **Cleanup** | `close()` | `close()` | `close()`, `unlink()` | `close()`, `unlink()` |
