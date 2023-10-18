# Automatic-water-level

making new projects based on nodeMCU :

EQUIPMENTS : 
requirement: 
1) 3x nodeMCU  (1 into server 2 others will be client)
2) making socket programming
3) sensors : (water level measurement x2)
4) relay : for turning on the motor automatically.

WORKFLOW:
intialize 1 server 2 client.(two client will be having water level sensor attached.)
automatic water level measurement.
one is on the ground level one is on the 2nd floor.

Client 1:
ground water level should be more than 50% initially. (client)
while(1)
{
IF 
ground water level would be more than 20% then only 
send ON signal to server 
start the motor 
Else
already off signal
}

Client 2: 
2nd floor water level it will check 
while(1)
{
IF
water level is below 10% then
send signal to Server.
ELse
check server data which will be water level of ground tank. 
}

Server : 
It should have data from both the client.
client 1 will send its data of water level in percentage.
client 2 will send its data of water level in percentage.

it will be connected to Relay to Turn On Motor.
send all data to website or some local IP.
tank water levels.
}




   
