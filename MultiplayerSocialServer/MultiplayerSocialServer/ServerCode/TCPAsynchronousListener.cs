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
		public static Dictionary<string, Socket> UnassignedClients = new Dictionary<string, Socket>();
		public static Dictionary<string, Room> Rooms = new Dictionary<string, Room>();

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

			StateObject state = new StateObject();
			state.WorkSocket = handler;

			// Tell the client what rooms are available, if none ask them if they want to create a room?

			handler.BeginReceive(state.Buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallBack), state);
		}

		public static void ReadCallBack(IAsyncResult ar)
		{
			String content = String.Empty;

			// Retrieve the stat object and the handler socket
			// from the asynchronous state object
			StateObject state = (StateObject) ar.AsyncState;
			Socket handler = state.WorkSocket;

			// Read data from the client socket
			int BytesRead = handler.EndReceive(ar);

			if(BytesRead > 0)
			{
				// There might be more data, so store the data received so far.
				//state.SB.Append(Encoding.ASCII.GetString(state.Buffer, 0, BytesRead));

				content = Encoding.ASCII.GetString(state.Buffer, 0, BytesRead);
				char[] delimiter = new char[] {':'};
				string[] MessageParts = content.Split(delimiter);

				// Look for end of message?

				Console.WriteLine("Message recieved: {0}\n", content);

				// Filter the message
				Messages MessageType;
				if(Enum.TryParse<Messages>(MessageParts[0].ToUpper(), out MessageType))
				{
					Console.WriteLine(MessageType);
					switch(MessageType)
					{
					case Messages.REG:
						if(!UnassignedClients.ContainsKey(MessageParts[1]))
						{
							UnassignedClients.Add(MessageParts[1], handler);
						}
						else
						{
							SendTo(handler, "User already exists");
						}							
						break;
					case Messages.UNREG:
						UnassignedClients.Remove(MessageParts[1]);
						break;
					case Messages.CREATEROOM:
						if(!Rooms.ContainsKey(MessageParts[1]))
						{
							Room NewRoom = new Room(MessageParts [1], Rooms.Count, (BasePort + PortOffset + Rooms.Count));
							Rooms.Add(NewRoom.name, NewRoom);
						}
						else
							SendTo(handler, "Room already exists");
						break;
					case Messages.JOINROOM:
						// Check if room is full - what is max size?
						if(!Rooms.ContainsKey(MessageParts[1]))
							SendTo(handler, "Room does not exist");

						UnassignedClients.Remove(MessageParts[2]);
						Rooms[MessageParts[1]].AddClient(MessageParts[2], handler);						
						break;
					case Messages.LEAVEROOM:
						Rooms[MessageParts[1]].Clients.Remove(MessageParts[2]);
						UnassignedClients.Add(MessageParts[2], handler);
						break;
                        case Messages.SENDTO:
                        if (!UnassignedClients.ContainsKey(MessageParts[1]))
                            SendTo(handler, "Could not find user: " + MessageParts[1] + "\n");
                        else
                            SendTo(UnassignedClients[MessageParts[1]], MessageParts[2]);
						break;
					case Messages.SENDALL:
						if (!MessageParts [1].Contains ("Room")) 
							foreach (KeyValuePair<string, Socket> pair in UnassignedClients)
								SendTo (UnassignedClients [pair.Key], MessageParts [2]);						
						else
							foreach(KeyValuePair<string, Socket> pair in Rooms[MessageParts[1]].Clients)
								SendTo(Rooms[MessageParts[1]].Clients[pair.Key], MessageParts[2]);
						break;
					default:
						SendTo(handler, "Invalid Message\n");
						break;
					}
				}
				else
				{
					SendTo(handler, "Invalid Message\n");
				}
			}

			// Start Receiving again whether data has been sent or not
			handler.BeginReceive(state.Buffer, 0, StateObject.BufferSize, 0, 
				new AsyncCallback(ReadCallBack), state);

		}

		private static void SendTo(Socket handler, String data)
		{
			// Convert the string data to byte data using ASCII encoding
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
	}
}

