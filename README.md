# Socket Sample Test
## Table of Contents
1. [Test Summary](README.md#test-summary)
2. [Running Instructions](README.md#test-summary)
3. [Implementation](README.md#test-summary)
4. [Comments](README.md#test-summary)
5. [Contact](README.md#test-summary)
## Test Summary
[Back to Table of Contents](README.md#table-of-contents)
### Requirements:
1) Program must be written with C++ and must compile using g++
2) The application must be capable of running on x86_64 Linux
3) Source, makefiles, and documentation are expected

### Task Description:
1. Create a simple OO server program. The server shoudl accept client connections, receive XML data on that connection, display the contents of the XML message, and send a response back to the client. The server should have the following characteristics:  
     a. Take the address and port information as commandline parameters that default to 127.0.0.1 and port 5000  
     b. Open a socket and listen for incoming data  
     c. Receive data and determine if it is valid XML  
        - If it is XML, pass to a work queue that "processes" the command  
        - Invalid XML should result in a display of "Unknown Command"  
     d. Parse the command and display it to the console with any data rows  
     e. Send a response to the originating socket  
2. The client simply intiates a connection, sends an XML packet, and displays the server's response. The client could be a simple shell script using netcat or a small program to send an XML message (from a text file) to the socket server monitor.  
### Example:  
A sample valid XML message will be in the following format:  
(Request)
```
<?xml version = '1.0' encoding = 'UTF-8'?>
<request>
  <command>Print</command>
  <data>
     <row type="name">Mr. Joe Chase</row>
     <row type="address">123 Anywhere Lane</row>
  </data>
</request>
```
(Response)  
```
<?xml version = '1.0' encoding = 'UTF-8'?>
<response>
  <command>Print</command>
  <status>Complete</status>
  <date>1970-01-01 00:00:00</date>
</response>
```
## Running Instructions
[Back to Table of Contents](README.md#table-of-contents)
### Compile  
*requires C++11 or better*  
To compile run `make`.
### Usage
Once compiled the executable will run as follows:
```
./main -i 127.0.0.1 -p 5000
```
Either or both the IP address and port number can be omitted which will result in the default value being used.
### Client  
*cat and netcat are required for testing*  

The repo includes a shell script that will connect to the default IP and Port then send the included test.xml file.
To use the shell script run as follows:
```
bash client.sh
```
To change any of the defaults just run as follows:
```
cat FILE.xml | netcat IPADDRESS PORT
```
## Implementation
[Back to Table of Contents](README.md#table-of-contents)
### Expected Input
The server requires valid XML as follows:
```
<?xml version = '1.0' encoding = 'UTF-8'?>
<request>
  <command>Print</command>
  <data>
     <row type="name">Mr. Joe Chase</row>
     <row type="address">123 Anywhere Lane</row>
  </data>
</request>
```
There must be a value between the command tags. If the command tags are missing or there is no value in the command tags the server will display and return `Unknown Command`.  
There must be data tags present, but they are not required to have any values between them. If the data tags are missing the server will display and return the error `Data tag missing`
### Expected Output
When the server starts it will display a message that it has started and the full server address:
`Starting Server at 127.0.0.1 5000`
When an XML message is received, the command and data from the message will be displayed
```
Command: Print
Data:
    address: 123 Anywhere Lane
    name: Mr. Joe Chase

```
as well as a response XML message sent to the client
```
<?xml version = '1.0' encoding = 'UTF-8'?>
<response>
  <command>Print</command>
  <status>Complete</status>
  <date>2019-07-30 11:11:11</date>
</response>
```
### Details
At this point the server will sit and wait for another client to connect. Current server architecture will support multiple clients sequentially.
To shutdown the server the key command of CTRL-C will close the sockets and display the shutdown message, `Shutting down server`.
## Comments
[Back to Table of Contents](README.md#table-of-contents)  

Next task would be to add threading which would allow multiple simultaneous clients to be processed.
## Contact
[Back to Table of Contents](README.md#table-of-contents)  

[matt@fostermatt.com](mailto:matt@fostermatt.com)

















