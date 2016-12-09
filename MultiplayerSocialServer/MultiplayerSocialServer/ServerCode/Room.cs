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
		public int RoomID;
		public Dictionary<string, Socket> Clients = new Dictionary<string, Socket>();
		public int UDPPort; 

		public Room(string p_RoomName, int p_RoomID, int p_UDPPort)
		{
			name = p_RoomName;
			RoomID = p_RoomID;
			UDPPort = p_UDPPort;

			UDPAsynchronousListener.StartListening((UDPPort + RoomID));
		}

		public void AddClient(string p_ClientName, Socket p_ClientSocket)
		{
			Clients.Add(p_ClientName, p_ClientSocket);
			// Add the client to the UDP connection/broadcast?
		}

		public void RemoveClient(string p_ClientName)
		{
			Clients.Remove(p_ClientName);
			// Remove the client from the UDP connection/broadcast?
		}
	}
}

