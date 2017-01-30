using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace MultiplayerSocialServer
{
    public class Client
    {
        public Socket ClientSocket = null;
        // Size of recieve buffer.
        public const int BufferSize = 2048;
        // Receive buffer.
        public byte[] Buffer = new byte[BufferSize];

        public UdpClient UDPClient;
        public IPEndPoint EndPoint;

        public Player User = new Player();

        public Client(Socket p_Socket)
        {
            ClientSocket = p_Socket;
        }

        public void AssignUDP(UdpClient p_Client, IPEndPoint p_EndPoint)
        {
            UDPClient = p_Client;
            EndPoint = p_EndPoint;
        }
    }
}

