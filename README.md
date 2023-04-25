# Bethernet_NP
In this repository you will find a application and a service wich can communicate with each other.

## What is it?
This project is a ZMQ service, which is a calculator that exists out of a service and client. These two will work with communication over Benthernet.

The diagram below will explain more how the communication works.

## Diagram
![Diagram](C:\Users\natha\Desktop\S2\Netwerk_Programming\1_Taken\FlowCharts\ZMQ.png)

Both the service and client communicate over ZMQ with the Benthernet broker.

# How it works
When the service and client are both started the client will ask for the formula you want to calculate. giev in the desired formula and press "Enter". When "Enter" is pressed it will ask to press any other key to validate it's the right formula. After pressing any other key, the client will push the formula to the broker on the right topic where the service is subscribed on. The broker recieves the string. The service is subscribed on the topic where the client sends to to the broker and now sees that there is a message. The broker pushes the recieved string to the topic where the service is subscribed on. The service now reads the string and extracts the correct part, the formula in this case, from the string. Know it knows the formula which the user wants to be calculated. The service now takes this formula and calculates it with the C++ code designed to calculate formulas and returns the answer. the service now knows what the answer is of the fomrula and pushes it to the broker on the topic where the client is subscribed on. The client sees that there is an answer on his subscribed topic and the broker now pushes the answer to the client. the client now needs to also extract the answer from the recieved string and prints it in the terminal.

## Server
The service is subscribed on the topic "Nathan>service?>" and will push to the broker on "Nathan>service!>".

## Client
The client is subscribed on the topic "Nathan>service!>" and will push to the broker on "Nathan>service?>".



alles samen in benthernet

Broker van Bart;

Client;

Service;

client vraagt formule, geef formule in en stuurt formule en topic naar benthernet, 1+1 is topic, server is sub op topic en haalt 1+1 uit bericht, doet fomrule en stuurt 2 naar broker met topic ! met uitkomst 2, broker stuurt topic! met 2 in naar client

client connect met broker op ip en poort
client socket subt op topic - push topic en socket en sub topic en socket (socket = file op pc)
client subt op topic (naam in code) 

programma start; client sub op ?; service sub op !; client vraagt formule en ik geef in; client pushed string naar broker op topic waar de service op gesubt is; broker   