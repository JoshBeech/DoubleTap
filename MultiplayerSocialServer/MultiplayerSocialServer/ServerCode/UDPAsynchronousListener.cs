using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace MultiplayerSocialServer
{
    public class UDPState
    {
        public IPEndPoint EndPoint;
        public UdpClient client;

        public UDPState(IPEndPoint p_EndPoint, UdpClient p_Client)
        {
            EndPoint = p_EndPoint;
            client = p_Client;
        }
    }

    public class UDPAsynchronousListener
    {
        public static int port;

        public static void StartListening(int p_Port)
        {   
            port = p_Port;

            // Begin Receive
            ReceiveMessages();
        }

        public static void ReceiveCallBack(IAsyncResult ar)
        {
            UdpClient client = (UdpClient)((UDPState)(ar.AsyncState)).client;
            IPEndPoint EndPoint = (IPEndPoint)((UDPState)(ar.AsyncState)).EndPoint;

            Byte[] BytesReceived = client.EndReceive(ar, ref EndPoint);
            string StringReceived = Encoding.ASCII.GetString((BytesReceived));

            Console.WriteLine("Received: {0}", StringReceived);

            // Do the thing (while loop?)
            SendMessage(EndPoint, StringReceived);
        }

        public static void ReceiveMessages()
        {
            IPEndPoint l_EndPoint = new IPEndPoint(IPAddress.Any, port);
            UDPState l_state = new UDPState(l_EndPoint, new UdpClient(l_EndPoint));

            l_state.client.BeginReceive(new AsyncCallback(ReceiveCallBack), l_state);
        }

        public static void SendCallBack(IAsyncResult ar)
        {
            UdpClient client = (UdpClient)ar.AsyncState;

            Console.WriteLine("number of bytes sent: {0}", client
                .EndSend(ar));
        }

        public static void SendMessage(IPEndPoint p_EndPoint, string message)
        {
            UdpClient client = new UdpClient();

            // Was string and port
            client.Connect(p_EndPoint);
            Byte[] SendBytes = Encoding.ASCII.GetBytes(message);

            // send message, destination defined by Connect() call
            client.BeginSend(SendBytes, SendBytes.Length, new AsyncCallback(SendCallBack), client);

            // the while loop thing? talk to julien?
        }
    }
}

