import paho.mqtt.client as mqtt

import time

import json
import random



client=mqtt.Client()

#client.connect("localhost",1883,60)

client.connect("192.168.4.1", 1883,60)



msg="Picked Up."

dict={"PUB_Board_ID": "Navig", "target_color": 1}

j=json.dumps(dict)
print(j)

client.publish("/team18/target",j, retain = False);

print(" "+time.asctime(time.localtime(time.time() ) ) )

time.sleep(0.1)

#client.disconnect();