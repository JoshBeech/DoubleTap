using System;
using System.Collections.Generic;
using System.Numerics;

namespace MultiplayerSocialServer
{
    public class GameManager
    {
        public bool IsGameInProgress = false;
        public int MapXMax;
        public int MapYMax;


        // List of Players
        // List of other entities, e.g bases, bullets, collectable
        // Start Positions

        private List<int> PlayerSpawnXPositions = new List<int>();
        private List<int> PlayerSpawnYPositions = new List<int>();


        public GameManager()
        {

        }

        public void StartGame()
        {
            IsGameInProgress = true;
        }

        public void SetMapSize(int p_Xmax, int p_YMax)
        {
            MapXMax = p_Xmax;
            MapYMax = p_YMax;

            SetPlayerSpawnPositions();
        }

        public void SetPlayerSpawnPositions()
        {
            PlayerSpawnXPositions.Add(10);
            PlayerSpawnYPositions.Add(10);

            PlayerSpawnXPositions.Add(MapXMax - 10);
            PlayerSpawnYPositions.Add(MapYMax - 10);

            PlayerSpawnXPositions.Add(MapXMax - 10);
            PlayerSpawnYPositions.Add(10);

            PlayerSpawnXPositions.Add(10);
            PlayerSpawnYPositions.Add(MapYMax - 10);
        }

        public int GetPlayerSpawnXPosition(int p_Team)
        {
            return PlayerSpawnXPositions[p_Team - 1];
        }

        public int GetPLayerSpawnYPositions(int p_Team)
        {
            return PlayerSpawnYPositions[p_Team - 1];
        }
    }
}

