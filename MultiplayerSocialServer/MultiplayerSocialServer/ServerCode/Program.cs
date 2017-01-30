using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace MultiplayerSocialServer
{
	/*
	Server TODO:
	UDP
	Generate UniqueID (per session) - multiple logons or guest users
	Protobuff
	Rooms?
	Friends?
	Highscore
	Optimise
	*/

    // Reorganise? Split into multiple?
	public enum Messages{REG, LOGIN, LOGOUT, 
        JOINROOM, CREATEROOM, LEAVEROOM, GETROOMS, REFRESHROOMS, 
        GETPLAYERS, READY, SETTINGS, POSITIONS, DIRECTION, FIRE,
        SENDTO, SENDALL};

	public class StateObject
	{
		// Client socket.
		public Socket WorkSocket = null;
		// Size of recieve buffer.
		public const int BufferSize = 2048;
		// Receive buffer.
		public byte[] Buffer = new byte[BufferSize];
		// Receive data string
		public StringBuilder SB = new StringBuilder();
	}

	class MainClass
	{
		public static int BasePort = 27550;
		//public static int UDPBasePort = 27600;

		public static int Main(string[] args)
		{
            Thread t_BroadcastThread = new Thread(new ThreadStart(BroadCast));
            t_BroadcastThread.Name = "BroadCasting Thread";
            t_BroadcastThread.Start();

			TCPAsynchronousListener.StartListening(BasePort, 50);
			return 0;
		}

        public static void BroadCast()
        {
            bool EndBroadcast = false;

            UdpClient l_Listener = new UdpClient(MainClass.BasePort);
            IPEndPoint l_EndPoint = new IPEndPoint(IPAddress.Any, MainClass.BasePort);

            while(!EndBroadcast)
            {
                Console.WriteLine("Waiting for broadcast");

                // Block and wait for a broadcast
                byte[] l_bytes = l_Listener.Receive(ref l_EndPoint);

                Console.WriteLine("Received broadcast from {0}", l_EndPoint.ToString());

                byte[] l_ReturnMessage = Encoding.ASCII.GetBytes("Hello There");
                l_Listener.Send(l_ReturnMessage, l_ReturnMessage.Length, l_EndPoint);
            }
        }
	}


}
