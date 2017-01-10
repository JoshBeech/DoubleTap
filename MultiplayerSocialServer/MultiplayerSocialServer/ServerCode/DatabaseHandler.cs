using System;
using System.Data;
using Mono.Data.Sqlite;

namespace MultiplayerSocialServer
{
    public class DatabaseHandler
    {
        private static string m_ConnectionString = "Data Source=../../Database/DoubleTap.db; Version=3";

        public static bool AddUser(string p_Username, string p_Password)
        {
            SqliteConnection l_NewConnection = new SqliteConnection(m_ConnectionString);
            l_NewConnection.Open();

            Console.WriteLine("Checking if user exists");

            if(ConfirmUser(p_Username, p_Password))
            {
                Console.WriteLine("User Exists");
                return false;
            }

            string l_InsertQuery = "INSERT into Users (Username, PasswordHash) values ('"+ p_Username + "', '" + p_Password + "')";
            SqliteCommand l_InsertCommand = new SqliteCommand(l_InsertQuery, l_NewConnection);
            l_InsertCommand.ExecuteNonQuery();

            return true;
        }

        public static bool ConfirmUser(string p_Username, string p_Password)
        {
            SqliteConnection l_NewConnection = new SqliteConnection(m_ConnectionString);
            l_NewConnection.Open();

            string l_SelectQuery = "SELECT * FROM Users WHERE Username = '" + p_Username + "'";
            SqliteCommand l_SelectCommand = new SqliteCommand(l_SelectQuery, l_NewConnection);
            SqliteDataReader l_Reader = l_SelectCommand.ExecuteReader();

            while (l_Reader.Read())
            {
                if (p_Password.Equals((string)l_Reader["PasswordHash"]))
                {
                    l_NewConnection.Close();
                    return true;
                }
            }

            l_NewConnection.Close();
            return false;
        }

        //Single queries
    }
}

