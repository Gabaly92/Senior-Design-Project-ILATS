# Senior-Design-Project-ILATS (Infrared Light Asset Tracking System)
A prototype of a system that tracks medical devices in hospitals using infrared light and sends to a    
database on a server which device is in which room using Ethernet.

1) Programmed the Ethernet module which updates the servers with the latest devices locations.
   The Ethernet module uses the TCP/IP protocol to send the devices latest locations to an online server
2) Programmed the server that updates the room assignments database with the latest devices locations
   The Server Receives the devices latest locations within a link and then uses PHP code to store the    
   locations within a .CSV file which represents the devices locations database
3) Programmed the battery life checker which computes the battery life of the battery on the moving  object and shows the    
   current status of the battery using an RGB LED
