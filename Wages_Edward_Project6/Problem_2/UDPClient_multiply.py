from socket import *

#Please implement both the client program and the server program in both TCP and UDP. The
#client program will prompt the user to enter a list of comma-separated numbers, send the server
#this list of numbers, receive the product back, and print it out. The server program will receive a
#list of number, perform the multiplication, and send the product or an error message back.

#In the client programs, please specify the IP address of the server program to be “127.0.0.1” so 
#that they can be tested in your local computer.


#set up socket
clientSocket = socket(AF_INET, SOCK_DGRAM)

clientSocket.bind(('127.0.0.1', 10000))

message = input('Input a list of comma-separated numbers:')

messageBytes = message.encode("utf-8")

#Send the message through client socket
serverIP = '127.0.0.1'
serverPort = 12345
serverAddress = (serverIP, serverPort)

clientSocket.sendto(messageBytes, serverAddress)

#Recive the resulting value
modifiedMessageBytes, serverAddress = clientSocket.recvfrom(2048)
modifiedMessage = modifiedMessageBytes.decode("utf-8")
serverIP, serverPort = serverAddress
print(f"Multiplication Result from {serverIP}:{serverPort} = {modifiedMessage}")

clientSocket.close()