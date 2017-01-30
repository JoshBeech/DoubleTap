using System;
using System.Numerics;

namespace MultiplayerSocialServer
{
    public class Player
    {
        public int Xposition;
        public int Yposition;

        public string ReadyStatus = "Not Ready";
  
        public string Team;

        // 1 == Left, 2 == Right, 3 == Up, 4 == Down
        public int MovementDirection;

        public Player()
        {
        }

        public void SetPosition(int x, int y)
        {
            Xposition = x;
            Yposition = y;
        }
    }
}

