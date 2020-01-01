import paho.mqtt.client as mqtt

import time

import json
import random



client=mqtt.Client()

#client.connect("localhost",1883,60)

client.connect("192.168.4.1", 1883,60)



msg="Picked Up."

dict={"Message_ID": 1, "angle": 10, "distance": 1, "type": 0}

j=json.dumps(dict)
print(j)

client.publish("/team18/sensor",j, retain = False);

print(" "+time.asctime(time.localtime(time.time() ) ) )

time.sleep(0.1)

#client.disconnect();