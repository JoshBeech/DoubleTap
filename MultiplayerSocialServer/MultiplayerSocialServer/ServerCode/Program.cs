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

	public enum Messages{REG, UNREG, LOGIN, LOGOUT, JOINROOM, CREATEROOM, LEAVEROOM, SENDTO, SENDALL};

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
			TCPAsynchronousListener.StartListening(BasePort, 50);
			return 0;
		}
	}
}
