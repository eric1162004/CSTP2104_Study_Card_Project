using System.Collections.Generic;
using System.IO;

namespace fileLib
{
    public class FileManager
    {
        public float getRadius(int frameNum)
        {
            return 40.0f;
        }

        public float getX(int frameNum)
        {
            return 20.0f;
        }

        public List<string> GetListsOfFiles()
        {
            List<string> files = new List<string>();
            string dirs = Directory.GetCurrentDirectory();
            string[] fileEntries = Directory.GetFileSystemEntries(dirs, "*.txt");
            foreach (string fileName in fileEntries)
                files.Add(fileName);

            return files;
        }

        public void WriteToFile(string fileName, string[] items)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(fileName, FileMode.Create)))
            {
                foreach (string s in items)
                    writer.Write(s);
            }
        }

        public List<string[]> GetItems(string fullPath)
        {
            List<string[]> items = new List<string[]>();

            if (File.Exists(fullPath))
            {
                using (BinaryReader reader = new BinaryReader(File.Open(fullPath, FileMode.Open)))
                {
                    while (reader.PeekChar() != -1)
                    {
                        string[] pairs = reader.ReadString().Split(':');
                        items.Add(pairs);
                    }
                }
            }
            return items;
        }

        public void DeleteFile(string fileName)
        {
            string dir = Directory.GetCurrentDirectory();
            string fullPath = dir + "/" + fileName;
            if (File.Exists(fullPath))
                File.Delete(fullPath);
        }

    }
}
