#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>


using namespace std;

//Hashes that contain passwords
const string PASS_HASH[10] = {
     "567cf4c0b536d13d88f5a1794da17864935c3e4f", 
     "f21e77142fdc5b2075bfa10dcd1cad930af3a1e5", 
     "f32799f284d777cf10838e63dfc8e67b7a6cfff1", 
     "e5564df8bdf471ec15e8b9c586cf52f6c0b6aec6", 
     "3957c1afc1bf2c683f6dc65477a5ef0f9307fbfd", 
     "8bda99642e781243b1a1b15571797fc98312f22c", 
     "b56a4f72f0c4031a0737346eb80e669c6d6b3cd4", 
     "54c15eedd7615d3d92afc152722f231ceb76366d", 
     "b1f9f946d0738e1512da5c37a70c0bacb0f1cce9", 
     "9e3ec2133ca26eb0b04e07c71604de509d434bcc" };

int main()
{
     //Iterates through passwords and attempts to crack each one
     for (int ii = 0; ii < 10; ii++)
     {
          map<string, list > rainbowTable;

          string key;
          string value;
          string temp;
          ifstream table("table.txt");
          if (table.is_open())
          {
               while (!table.eof())
               {
                    table >> temp;
                    table >> value;
                    table >> temp;
                    table >> key;
                    table >> temp;

                    rainbowTable[key].push_back(value);
               }
          }
          table.close();

          string test;

          //Sets up marker for finding password
          bool done = false;

          do
          {
               //If end reduce is found
               bool found = false;

               //Beginning reduce
               test = reduce(PASS_HASH[ii], 5);

               //Hash reduce, and if found evaluate
               do
               {
                    //Searches rainbow table for reduced string
                    map<string, list >::iterator itor;
                    itor = rainbowTable.find(test);

                    //If the reduced string is there, evaluate the hash
                    if (itor != rainbowTable.end())
                    {
                         //End reduce has been found
                         found = true;

                         //Loop though possible starting strings
                         for (list::iterator itor2 = (itor->second).begin(); itor2 != (itor->second).end(); ++itor2)
                         {
                              //Val is the reduced string, test is the hash
                              string val = *itor2;
                              test = hash(val);

                              //Set up marker for moving on to next starting string
                              bool next = false;
                              do
                              {
                                   //If hash is the password, success
                                   if (test == PASS_HASH[ii])
                                   {
                                        done = true;
                                        cout << test << ": " << val << endl;                                    }                                    //Else, if it is equal to the end reduce, try next starting reduce                                    else if (val == itor->first)
                                   {
                                        next = true;
                                   }

                                   //Else, reduce and hash again
                                   else
                                   {
                                        val = reduce(test, 5);
                                        test = hash(val);
                                   }
                              //while not at end reduce or password, keep reduce hashing
                              } while (done != true && next != true);

                              //if a password has been found, break 
                              if (done)
                                   break;
                         }
                         if (!done)
                              rainbowTable.erase(itor);
                    }

                    //If the string is not found, hash reduce it and try again
                    else if (!found)
                    {
                         test = hash(test);
                         test = reduce(test, 5);
                    }

               } while (!found);
         } while (!done);
     }
}