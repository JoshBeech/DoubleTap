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

        public static Room l_Room;
        public static Dictionary<string, Client> RoomClients = new Dictionary<string, Client>();

        public static void StartListening(int p_Port, Room p_Room, Dictionary<string, Client> Clients)
        {   
            port = p_Port;
            l_Room = p_Room;
            RoomClients = Clients;
            // Begin Receive
            ReceiveMessages();
        }

        public static void ReceiveMessages()
        {
            foreach (Client l_Client in RoomClients.Values)
            {
                l_Client.EndPoint = new IPEndPoint(IPAddress.Any, port);
                l_Client.UDPClient = new UdpClient(l_Client.EndPoint);

                l_Client.UDPClient.BeginReceive(new AsyncCallback(ReceiveCallBack), l_Client);
            }
        }

        public static void ReceiveCallBack(IAsyncResult ar)
        {
            Client l_Client = (Client)ar.AsyncState;
            //UdpClient client = (UdpClient)((UDPState)(ar.AsyncState)).client;
            //IPEndPoint EndPoint = (IPEndPoint)((UDPState)(ar.AsyncState)).EndPoint;
            Console.WriteLine("Receiving UDP");
            Byte[] l_BytesReceived = l_Client.UDPClient.EndReceive(ar, ref l_Client.EndPoint);

            if (l_BytesReceived.Length > 0)
            {
                string l_StringReceived = Encoding.ASCII.GetString((l_BytesReceived));

                char[] delimiter = new char[] { ':' };
                string[] MessageParts = l_StringReceived.Split(delimiter);

                Console.WriteLine("Message recieved: {0}\n", l_StringReceived);

                MessageHandler.FilterUDPMessage(MessageParts, l_Client, l_Room);
            }
            //SendMessage(l_Client.EndPoint, StringReceived);

            l_Client.UDPClient.BeginReceive(new AsyncCallback(ReceiveCallBack), l_Client);
        }

        public static void SendMessage(Client p_Client, string message)
        {
            // Was string and port
            p_Client.UDPClient.Connect(p_Client.EndPoint);
            Byte[] SendBytes = Encoding.ASCII.GetBytes(message);

            // send message, destination defined by Connect() call
            p_Client.UDPClient.BeginSend(SendBytes, SendBytes.Length, new AsyncCallback(SendCallBack), p_Client.UDPClient);

            // the while loop thing? talk to julien?
        }

        public static void SendCallBack(IAsyncResult ar)
        {
            UdpClient client = (UdpClient)ar.AsyncState;

            Console.WriteLine("number of bytes sent: {0}", client.EndSend(ar));
        }
    }
}

