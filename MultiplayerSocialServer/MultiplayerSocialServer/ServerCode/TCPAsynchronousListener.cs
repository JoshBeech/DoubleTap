using System;
using System.IO;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace MultiplayerSocialServer
{
	public class TCPAsynchronousListener
	{
		public static int BasePort;
		public static int PortOffset;
		public static ManualResetEvent AllDone = new ManualResetEvent(false);
        public static Dictionary<string, Client> UnassignedClients = new Dictionary<string, Client>();
		public static Dictionary<string, Room> Rooms = new Dictionary<string, Room>();
        //public static DatabaseHandler Database = new DatabaseHandler();

		public static void StartListening(int p_TCPPort, int p_PortOffset)
		{
			BasePort = p_TCPPort;
			PortOffset = p_PortOffset;

			byte[] bytes = new byte[2048];

			// Establish the local endpoint for the socket.
			IPEndPoint LocalEndPoint = new IPEndPoint(IPAddress.Any, BasePort);

			// Create a TCP/IP socket.
			Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			// Bind the socket to the local endpoint
			// listen for incoming connections
			try
			{
				listener.Bind(LocalEndPoint);
				listener.Listen(100);

				while(true)
				{
					AllDone.Reset();

					//Start an asynchroous socket to listen for connections
					Console.WriteLine("Waiting for a connection...");
					listener.BeginAccept(new AsyncCallback(AcceptCallBack), listener);

					// Wait until a connection is made before continuing.
					AllDone.WaitOne();
				}
			}
			catch(Exception e)
			{
				Console.WriteLine(e.ToString());
			}

			Console.WriteLine("\nPress ENTER to continue...");
			Console.Read();
		}

		public static void AcceptCallBack(IAsyncResult ar)
		{
			// Get the socket that handles the clinet request
			Socket listener = (Socket)ar.AsyncState;
			Socket handler = listener.EndAccept(ar);

			// Signal the main thread to continue
			AllDone.Set();

            Client NewClient = new Client(handler);
            NewClient.ClientSocket = handler;

            handler.BeginReceive(NewClient.Buffer, 0, Client.BufferSize, 0, new AsyncCallback(ReadCallBack), NewClient);
		}

		public static void ReadCallBack(IAsyncResult ar)
		{
			String content = String.Empty;

			// Retrieve the stat object and the handler socket
			// from the asynchronous state object
            Client l_Client = (Client) ar.AsyncState;
            Socket handler = l_Client.ClientSocket;

			// Read data from the client socket
			int BytesRead = handler.EndReceive(ar);

			if(BytesRead > 0)
			{
				// There might be more data, so store the data received so far.
				//state.SB.Append(Encoding.ASCII.GetString(state.Buffer, 0, BytesRead));

                content = Encoding.ASCII.GetString(l_Client.Buffer, 0, BytesRead);
				char[] delimiter = new char[] {':'};
				string[] MessageParts = content.Split(delimiter);

				Console.WriteLine("Message recieved: {0}\n", content);

                MessageHandler.FilterTCPMessage(MessageParts, l_Client);
			}
             
			// Start Receiving again whether data has been sent or not
            handler.BeginReceive(l_Client.Buffer, 0, Client.BufferSize, 0, 
                new AsyncCallback(ReadCallBack), l_Client);

		}

		private static void SendTo(Socket handler, String data)
		{
			// Convert the string data to byte data using ASCII encoding
            Console.WriteLine("Sending: " + data);
			byte[] ByteData = Encoding.ASCII.GetBytes(data);

			// Begin sending the data to the remote device.
			handler.BeginSend(ByteData, 0, ByteData.Length, 0, new AsyncCallback(SendCallBack), handler);
		}

		private static void SendCallBack(IAsyncResult ar)
		{
			try
			{
				// Retrieve the socket from the state object.
				Socket handler = (Socket)ar.AsyncState;

				// Complete sending the data to the remote device.
				int BytesSent = handler.EndSend(ar);
				Console.WriteLine("Sent Message - contained {0} bytes.\n", BytesSent);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}

        public static bool CheckForUser(string p_ClientName)
        {
            if (UnassignedClients.ContainsKey(p_ClientName))
                return true;
            else
                return false;
        }

        public static void AddUser(string p_ClientName, Client p_Client)
        {
            UnassignedClients.Add(p_ClientName, p_Client);
        }

        public static void RemoveUser(string p_ClientName)
        {
            UnassignedClients.Remove(p_ClientName);
        }

        public static bool CheckForRoom(string p_RoomName)
        {
            if (Rooms.ContainsKey(p_RoomName))
                return true;
            else
                return false;
        }

        public static void CreateRoom(string p_RoomName, string p_RoomAvailablity, string p_ClientName, Client p_User)
        {
            Room NewRoom = new Room(p_RoomName, Rooms.Count, (BasePort + PortOffset + Rooms.Count), p_RoomAvailablity);
            Rooms.Add(NewRoom.name, NewRoom);
            JoinRoom(NewRoom.name, p_ClientName, p_User);
        }

        public static void JoinRoom(string p_RoomName, string p_ClientName, Client p_User)
        {
            UnassignedClients.Remove(p_ClientName);
            Rooms[p_RoomName].AddClient(p_ClientName, p_User);
        }

        public static void LeaveRoom(string p_RoomName, string p_ClientName, Client p_User)
        {
            Rooms[p_RoomName].RemoveClient(p_ClientName);
            UnassignedClients.Add(p_ClientName, p_User);
        }

        public static string GetRooms()
        {
            string l_RoomList = "";
            foreach(KeyValuePair<string, Room> l_Room in Rooms)
            {
                l_RoomList += l_Room.Value.name + "\t" + l_Room.Value.NumberOfPlayers + "/" +
                    l_Room.Value.MaxPlayers + "\t" + l_Room.Value.Availablity + ":";
            }
            return l_RoomList;
        }

        public static string GetAllPlayerInfoInRoom(string p_RoomName)
        {
            return Rooms[p_RoomName].GetAllPlayerInfo();
        }

        public static string UpdatePlayerInfoInRoom(string p_RoomName, string p_ClientName, string p_ReadyStatus)
        {
            Rooms[p_RoomName].UpdatePlayerStatus(p_ClientName, p_ReadyStatus);
            return GetAllPlayerInfoInRoom(p_RoomName);
        }

        public static void SendSingleTCPMessage(Client p_Client, string p_Message)
        {
            SendTo(p_Client.ClientSocket, p_Message);
        }

        public static void SendGroupTCPMessage(string p_Room, string p_Message)
        {
            if (p_Room.Equals(""))
                foreach (Client l_Client in UnassignedClients.Values)
                    SendTo(l_Client.ClientSocket, p_Message);
            else
                foreach (Client l_Client in Rooms[p_Room].Clients.Values)
                    SendTo(l_Client.ClientSocket, p_Message);
        }
	}
}

