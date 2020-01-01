import paho.mqtt.client as mqtt

import time

import json
import random



client=mqtt.Client()

#client.connect("localhost",1883,60)

client.connect("192.168.4.1", 1883,60)



count=0
drop=0
test = ""
for i in range(256):
	test += 'h'
	


while(1):

	count=count+1
	msg="this is a test, this is a test: count "+str(count)

	print(msg+", drop: "+str(drop))

	#client.publish("topic/test",msg);
	if (random.random() < 0.15):	
		drop=drop+1
#		count+= 1
		continue
	

	dict={"PUB_Board_ID":"rocky", "Message_ID":str(count) , "Message_Content": test}

	j=json.dumps(dict)
	print(j)

	if(count == 1):
		client.publish("/team18/rocky",j, retain = True);
	
	client.publish("/team18/rocky",j, retain = True);

	print(" "+time.asctime(time.localtime(time.time() ) ) )

	time.sleep(0.1)

	#client.disconnect();