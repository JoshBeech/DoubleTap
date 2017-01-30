using System;

namespace MultiplayerSocialServer
{
    public class MessageHandler
    {
        public MessageHandler()
        {
        }

        public static void FilterTCPMessage(string[] p_Message, Client p_Sender)
        {
            string l_ReturnMessage = "";

            Messages MessageType;
            if(Enum.TryParse<Messages>(p_Message[0].ToUpper(), out MessageType))
            {
                Console.WriteLine(MessageType);
                switch(MessageType)
                {
                    case Messages.REG:
                        if (DatabaseHandler.AddUser(p_Message[1], p_Message[2]))
                        {
                            TCPAsynchronousListener.AddUser(p_Message[1], p_Sender);
                            l_ReturnMessage = "1:Success:";
                            TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                            Console.WriteLine("User Accepted");
                        }
                        break;
                    case Messages.LOGIN:
                        //DB check
                        if (DatabaseHandler.ConfirmUser(p_Message[1], p_Message[2]))
                        {
                            if (!TCPAsynchronousListener.CheckForUser(p_Message[1]))
                            {
                                TCPAsynchronousListener.AddUser(p_Message[1], p_Sender);
                                Console.WriteLine("User Accepted");
                                l_ReturnMessage = "1:Success:";

                            }
                            else
                            {
                                l_ReturnMessage = "1:Failed:User Already Logged In:";
                            }
                        }
                        else
                        {
                            l_ReturnMessage = "1:Failed:User Not Found:";    
                        }
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        break;
                    case Messages.LOGOUT:
                        l_ReturnMessage = "2:Success:" + p_Message[1];
                        TCPAsynchronousListener.RemoveUser(p_Message[1]);
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        break;
                    case Messages.CREATEROOM:
                        if (!TCPAsynchronousListener.CheckForRoom(p_Message[1]))
                        {
                            TCPAsynchronousListener.CreateRoom(p_Message[1], p_Message[2], p_Message[3], p_Sender);
                            l_ReturnMessage = "4:Success:" + p_Message[1] + ":";
                        }
                        else
                            l_ReturnMessage = "4:Fail:Room already exists";
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "7:Bind:" +
                            TCPAsynchronousListener.Rooms[p_Message[1]].UDPPort + ":");
                        TCPAsynchronousListener.Rooms[p_Message[1]].StartUDPListener(TCPAsynchronousListener.Rooms[p_Message[1]]);
                        break;
                    case Messages.JOINROOM:
                        // Check if room is full - what is max size?
                        if (!TCPAsynchronousListener.CheckForRoom(p_Message[1]))
                            l_ReturnMessage = "4:Fail:Room does not exist:";
                        else
                        {
                            TCPAsynchronousListener.JoinRoom(p_Message[1], p_Message[2], p_Sender);
                            l_ReturnMessage = "4:Success:" + p_Message[1] + ":";
                        }   
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        l_ReturnMessage = "8:Lobby:" + TCPAsynchronousListener.GetAllPlayerInfoInRoom(p_Message[1]);
                        TCPAsynchronousListener.SendGroupTCPMessage(p_Message[1], l_ReturnMessage);
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "7:Bind:" + 
                            TCPAsynchronousListener.Rooms[p_Message[1]].UDPPort + ":");
                        TCPAsynchronousListener.Rooms[p_Message[1]].StartUDPListener(TCPAsynchronousListener.Rooms[p_Message[1]]);
                        break;
                    case Messages.LEAVEROOM:
                        TCPAsynchronousListener.LeaveRoom(p_Message[1], p_Message[2], p_Sender);
                        l_ReturnMessage = "9:Success:";
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        l_ReturnMessage = "8:Lobby:" + TCPAsynchronousListener.GetAllPlayerInfoInRoom(p_Message[1]);
                        TCPAsynchronousListener.SendGroupTCPMessage(p_Message[1], l_ReturnMessage);
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "7:UnBind:");
                        break;
                    case Messages.GETROOMS:
                        l_ReturnMessage = "5:" + TCPAsynchronousListener.GetRooms();
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        break;
                    case Messages.REFRESHROOMS:
                        l_ReturnMessage = "6:" + TCPAsynchronousListener.GetRooms();
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        break;
                    case Messages.GETPLAYERS:
                        // May need updating
                        l_ReturnMessage = "8:Lobby:" + TCPAsynchronousListener.GetAllPlayerInfoInRoom(p_Message[1]);
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, l_ReturnMessage);
                        break;
                    case Messages.READY:
                        l_ReturnMessage = "8:Lobby:" + TCPAsynchronousListener.UpdatePlayerInfoInRoom(
                            p_Message[1], p_Message[2], p_Message[3]);

                        TCPAsynchronousListener.SendGroupTCPMessage(p_Message[1], l_ReturnMessage);
                        break;
                    case Messages.SETTINGS:
                        if(p_Message[1] == "MapSize")
                        {
                            TCPAsynchronousListener.Rooms[p_Message[2]].RoomManager.SetMapSize(
                                Int32.Parse(p_Message[3]), Int32.Parse(p_Message[4]));
                        }
                        break;
                    case Messages.FIRE:
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "11:Fire");
                        break;
                    case Messages.SENDTO:
                        if (TCPAsynchronousListener.CheckForUser(p_Message[1]))
                            TCPAsynchronousListener.SendSingleTCPMessage (TCPAsynchronousListener.UnassignedClients[p_Message[1]],
                                p_Message[2]);
                        else
                            TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "Could not find" + p_Message[1]);
                        break;
                    case Messages.SENDALL:
                        // Needs reworking
                        if (p_Message[1] == "")
                            TCPAsynchronousListener.SendGroupTCPMessage("", p_Message[2]);
                        else
                            TCPAsynchronousListener.SendGroupTCPMessage(p_Message[1], p_Message[2]);
                        break;
                    default:
                        TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "0:Invalid Message");
                        break;
                }
            }
            else
            {
                TCPAsynchronousListener.SendSingleTCPMessage(p_Sender, "0:Invalid");
            }            
        }

        public static void FilterUDPMessage(string[] p_Message, Client p_Sender, Room p_Room)
        {
            Messages MessageType;
            if (Enum.TryParse<Messages>(p_Message[0].ToUpper(), out MessageType))
            {
                switch(MessageType)
                {
                    case Messages.POSITIONS:
                        p_Sender.User.SetPosition(Int32.Parse(p_Message[2]), Int32.Parse(p_Message[3]));
                        p_Room.UpdatePlayerPositions(p_Sender);
                        break;
                    case Messages.DIRECTION:
                        p_Sender.User.MovementDirection = Int32.Parse(p_Message[1]);
                        break;
                }
            }
        }

    }
}

