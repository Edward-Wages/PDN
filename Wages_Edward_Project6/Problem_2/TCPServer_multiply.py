from socket import *

#Please implement both the client program and the server program in both TCP and UDP. The
#client program will prompt the user to enter a list of comma-separated numbers, send the server
#this list of numbers, receive the product back, and print it out. The server program will receive a
#list of number, perform the multiplication, and send the product or an error message back.

welcomeSocket = socket(AF_INET, SOCK_STREAM)

serverIP = '127.0.0.1'
serverPort = 12345
serverAddress = (serverIP, serverPort)
welcomeSocket.bind(serverAddress)

welcomeSocket.listen(1)
print("The server is ready to receive")

#Start the operation
while True:
    connectionSocket, clientAddress = welcomeSocket.accept()
    clientIP, clientPort = clientAddress

    #recieve a byte from the socket
    try:
        messageBytes = connectionSocket.recv(1024)
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


    #Send the result back to the client
    connectionSocket.send(response.encode("utf-8"))

    connectionSocket.close()