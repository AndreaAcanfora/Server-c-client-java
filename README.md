# Server-c-client-java
c++ Server and Java client with socket

Make a chat using the socket server in c ++ and java client.
Part client:
I created two a text area for messages received one for messages sent and a text field where you can enter the text to send.
I enter the user IP address and port of the server, this information I will need to create the socket, and then use the chat
I also take the input the user's name, this data is used especially when using the java server that handles multiple users and then you can
distinguish by name. Septum title with your name and the current day.
Once taken the input data if correct and if the server is running is enabled text field.
In case of previous chats will be taken by two rows chat.txt and chat2.txt each one for its text area.
The client will have to work before receiving two messages from the server once connected
the first is to inform you that it is connected and this will be the string "START_NAME" In fact you can enter the name only after the client has received
this message. The second message that will receive '"NameAccepted" this message is used to inform the client that the server has accepted the name.
Subsequent messages that will receive the client must be in a format spefico:
must contain "Message" before the message if they are messages written by java client
instead must contain "messages" before the message if they are messages received from someone else.
The client automatically plunder only the post a message or messages.
Handled the event sent from the keyboard to send a message and add the sending time of the message also before I post them on file.

The server also acts as a C ++ client and has an interface similar to that of the client java
The server is started automatically with the start of the program. Uses port 12345 and is able to communicate with only one client at a time.
Wait to connect a client and when that happens enables the buttons to disconnect and to send messages.
Unlike the C ++ client does not save messages to a file. Just connects sends two messages to the client java first "START_NAME" and the second
"NameAccepted" that serve to make it clear to the client that it is ready. The first message that is receiving full is the name of the client, but unlike message
that will receive later this not write it on the text box. A label on the bottom left says the name of the computer connected.
A function returns the time and will be added in front of all the messages that are sent, also before each message adds "messages",
This serves to make it clear to the java client that the message and 'was sent by the client c ++.
As in the java client there are two text boxes, one on the left for messages sent and the right one for messages received.
(Only works on windows).
