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
        public int NumberOfPlayers;
        public int MaxPlayers = 8;
        public string Availablity;
        public bool InGame = false;
		public int RoomID;
        public Dictionary<string, Client> Clients = new Dictionary<string, Client>();
		public int UDPPort; 
        public GameManager RoomManager = new GameManager();

        public Room(string p_RoomName, int p_RoomID, int p_UDPPort, string p_Availablity)
		{
			name = p_RoomName;
			RoomID = p_RoomID;
			UDPPort = p_UDPPort;
            Availablity = p_Availablity;
		}

        public void StartUDPListener(Room p_Room)
        {
            UDPAsynchronousListener.StartListening((UDPPort + RoomID), p_Room, Clients);
        }

        public void AddClient(string p_ClientName, Client p_Client)
		{
            
			Clients.Add(p_ClientName, p_Client);
            NumberOfPlayers++;
            if (NumberOfPlayers <= 4)
                p_Client.User.Team = "Team" + NumberOfPlayers;
            else
                p_Client.User.Team = "Team" + (NumberOfPlayers - 4);

			// Add the client to the UDP connection/broadcast?
            //UDPAsynchronousListener.StartListening((UDPPort + RoomID), this, Clients);
		}

		public void RemoveClient(string p_ClientName)
		{
			Clients.Remove(p_ClientName);
            NumberOfPlayers--;
			// Remove the client from the UDP connection/broadcast?
		}

        public string GetAllPlayerInfo()
        {
            string l_PlayerInfo = "";
            foreach(KeyValuePair<string, Client> l_Player in Clients)
            {
                // Possibly inlcude ready status
                l_PlayerInfo += l_Player.Key + "\t" + l_Player.Value.User.Team + "\t" + 
                    l_Player.Value.User.ReadyStatus + ":";
            }
            return l_PlayerInfo;
        }

        public void UpdatePlayerStatus(string p_ClientName, string p_NewStatus)
        {
            Clients[p_ClientName].User.ReadyStatus = p_NewStatus;

            AllPlayersReady();
        }

        public void AllPlayersReady()
        {
            int l_PlayersReady = 0;
            foreach(Client l_Client in Clients.Values)
            {
                if (l_Client.User.ReadyStatus == "Ready")
                    l_PlayersReady++;
            }

            if(l_PlayersReady == Clients.Count)
            {
                InGame = true;

                foreach (Client l_Client in Clients.Values)
                {
                    if (NumberOfPlayers <= 4)
                        l_Client.User.SetPosition(RoomManager.GetPlayerSpawnXPosition(NumberOfPlayers), 
                            RoomManager.GetPLayerSpawnYPositions(NumberOfPlayers));
                    
                    else
                        l_Client.User.SetPosition(RoomManager.GetPlayerSpawnXPosition(NumberOfPlayers - 4), 
                            RoomManager.GetPLayerSpawnYPositions(NumberOfPlayers - 4));
                    

                    TCPAsynchronousListener.SendSingleTCPMessage(l_Client, "12:Self:" +
                        l_Client.User.Xposition + "," + l_Client.User.Yposition + ":");
                }

                TCPAsynchronousListener.SendGroupTCPMessage(name, "11:Start:");
            }
                
        }

        public void UpdatePlayerPositions(Client p_TriggerClient)
        {
            string l_Message = "12:";
            foreach(KeyValuePair<string, Client> l_Pair in Clients)
            {
                if(l_Pair.Value == p_TriggerClient)
                    l_Message += "Self:" + l_Pair.Value.User.Xposition + "," + l_Pair.Value.User.Yposition + ":";
                else
                    l_Message += l_Pair.Key + ":" + l_Pair.Value.User.Xposition + "," + l_Pair.Value.User.Yposition + ":";

                UDPAsynchronousListener.SendMessage(l_Pair.Value, l_Message);
            }

        }
	}
}

