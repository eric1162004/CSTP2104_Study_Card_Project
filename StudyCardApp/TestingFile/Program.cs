using System;
using fileLib;
using System.Collections.Generic;

namespace TestingFile
{
    class Program
    {
        static void Main(string[] args)
        {
            FileManager fm = new FileManager();
            float f = fm.getRadius(0);
            Console.WriteLine(f);

            /*            List<string> files = fm.GetListsOfFiles();

                        string[] itemToSave = new string[2] { "a:abc", "b:cef" };
                        fm.WriteToFile("anotherfile.txt", itemToSave);

                        foreach (string filepath in files)
                        {
                            List<string[]> items = fm.GetItems(filepath);
                            Console.WriteLine("Filename: " + filepath);
                            foreach (string[] x in items)
                            {
                                Console.WriteLine("key: " + x[0]);
                                Console.WriteLine("value: " + x[1]);
                            }
                        }

                        fm.DeleteFile("a.txt");*/

        }
    }
}
