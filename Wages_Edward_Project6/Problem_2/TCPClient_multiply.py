from socket import *

#Please implement both the client program and the server program in both TCP and UDP. The
#client program will prompt the user to enter a list of comma-separated numbers, send the server
#this list of numbers, receive the product back, and print it out. The server program will receive a
#list of number, perform the multiplication, and send the product or an error message back.

#In the client programs, please specify the IP address of the server program to be “127.0.0.1” so 
#that they can be tested in your local computer.

clientSocket = socket(AF_INET, SOCK_STREAM)
serverIP = '127.0.0.1'
serverPort = 12345

serverAddress = (serverIP, serverPort)
clientSocket.connect(serverAddress)
print("Client has successfully connected to the server")

message = input("Please enter a list of comma-separated numbers: ")

clientSocket.send(message.encode("utf-8"))

#At this point the server is performing the multiplication and sending the result back.

modifiedMessageBytes = clientSocket.recv(1024)
modifiedMessage = modifiedMessageBytes.decode("utf-8")
print("The product is:", modifiedMessage)

clientSocket.close()