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

		public Room(string RoomName)
		{
			name = RoomName;
		}
	}
}

