from socket import *

#Please implement both the client program and the server program in both TCP and UDP. The
#client program will prompt the user to enter a list of comma-separated numbers, send the server
#this list of numbers, receive the product back, and print it out. The server program will receive a
#list of number, perform the multiplication, and send the product or an error message back.

#In the client programs, please specify the IP address of the server program to be “127.0.0.1” so 
#that they can be tested in your local computer.


serverSocket = socket(AF_INET, SOCK_DGRAM)

serverIP = '127.0.0.1'
serverPort = 12345

serverAddress = (serverIP, serverPort)
serverSocket.bind(serverAddress)
print("Server has successfully been connected")

#Start the operation
while True:
    #Recieving byte string - 1024 is the length of the message
    messageBytes, clientAddress = serverSocket.recvfrom(2048)
    clientIP, clientPort = clientAddress

    #recieve a byte from the socket
    try:
        message = messageBytes.decode("utf-8")

        nums = [n.strip() for n in message.split(',') if n.strip()]

        if not nums:
            response = "Invalid input"
        else:
            result = 1
            for num in nums:
                result *= int(num)
            response = str(result)

    except ValueError:
        response = "Invalid input"
    except Exception as e:
        response = "Invalid input"


    #Send result back
    serverSocket.sendto(response.encode("utf-8"), clientAddress)