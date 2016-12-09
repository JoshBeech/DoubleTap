using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace MultiplayerSocialServer
{
	public class Room
	{
		public string name;
		public Dictionary<string, Socket> Clients = new Dictionary<string, Socket>();
		public int UdpPort; 
		public UdpClient UdpListener;
		public IPEndPoint RoomUdpEndPoint;

		public Room(string p_RoomName, int p_UdpPort)
		{
			name = p_RoomName;
			UdpPort = p_UdpPort;
			UdpListener = new UdpClient(UdpPort);
			RoomUdpEndPoint = new IPEndPoint (IPAddress.Any, UdpPort);
		}


	}
}

