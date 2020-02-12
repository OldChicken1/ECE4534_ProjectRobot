import paho.mqtt.client as mqtt
import time
import json

# feel free to change out print
topic = [("/team18/jie_", 0), ("/team18/rocky", 0), ("/TriRover/status", 1)]

trackBlock = -1
trackColor = 0
trackDistance = 0
trackAngle = 0
trackDirect = 0
curObjectNum = 0
msgNum = 0
curType = 0
state = 0
isArm1Sent = 0
prevState = 0
def on_connect(client, userdata, flags, rc):
    # print("connected with result code"+str(rc) )
    # print("subscribing on "+str(topic)  )
    client.subscribe(topic)  # <----subscribing topie


def on_message(client, userdata, msg):
    msg = json.loads(msg.payload.decode())
    # print(msg)
    global curObjectNum
    global msgNum
    global trackBlock
    global trackColor
    global trackDistance
    global trackAngle
    global trackDirect
    global curType
    global state
    global isArm1Sent
    global prevState
    # if msg['PUB_Board_ID'] is "jie__":
    #     curObjectNum = int(msg['Object_Detected'])



    if state == 0:
        if prevState == 6:
            if 'inStill' in msg:
                prevState = 0

        else:
            print("state 0")
            # if msg['']
            print(msg)
            curObjectNum = int(msg['Object_Detected'])
            if curObjectNum == 0:
                print("zero")
                trackBlock = -1
                trackColor = 0
                trackDistance = 0
                trackAngle = 1
                curType = 2
            else:
                trackBlock = int(msg['TrackingIndex0'])
                # print(trackBlock)
                trackColor = int(msg['Color0'])
                # print(trackColor)
                trackAngle = int(msg['Angle0'])
                # print(trackAngle)
                trackDistance = int(msg['IR_Distance'])
                # print(trackDistance)
                curType = 0
                state = 1
            roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
            print(roverMsg)
            roverMsg_send = json.dumps(roverMsg)
            client.publish("/team18/sensor", roverMsg_send, retain=False)
            msgNum += 1
    if state == 1:
        curObjectNum = int(msg['Object_Detected'])
        if curObjectNum == 0:
            curType = 4
        else:
            isTrackFound = False
            for i in range(curObjectNum):
                print('TrackingIndex' + str(i))
                # print(i)
                if trackBlock is not int(msg['TrackingIndex' + str(i)]):
                    print("trackblock: ", trackBlock)
                    print("trackingIndex: ", msg['TrackingIndex' + str(i)])
                    continue
                isTrackFound = True
                trackColor = int(msg['Color' + str(i)])
                print('Color' + str(i))
                trackAngle = int(msg['Angle' + str(i)])
                print('Angle' + str(i))
                trackDistance = int(msg['IR_Distance'])
                print(trackDistance)
                trackDirect = int(msg['Direction' + str(i)])
            if isTrackFound:
                if trackAngle >= -1 and trackAngle <= 1:
                    if trackDistance <= 11 and curType == 1:
                        state = 2
                        curType = 3
                        print("state changed to arm grabbing")
                        sensorMsg = {"PUB_Board_ID": "navig", "Message_ID": 1, "Message_Content": "Picked Up."}
                        sensorMsg_send = json.dumps(sensorMsg)
                        client.publish("/team18/navig", sensorMsg_send, retain=False)
                    else:
                        curType = 1
                else:
                    curType = 0
            else:
                # trackBlock = -1
                # trackColor = 0
                # trackDistance = 0
                # trackAngle = 0
                # curType = 2
                trackBlock = int(msg['TrackingIndex0'])
                # print(trackBlock)
                trackColor = int(msg['Color0'])
                # print(trackColor)
                trackAngle = int(msg['Angle0'])
                # print(trackAngle)
                trackDistance = int(msg['IR_Distance'])
                # print(trackDistance)
                curType = 0

        roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
        print(roverMsg)
        roverMsg_send = json.dumps(roverMsg)
        client.publish("/team18/sensor", roverMsg_send, retain=False)
        msgNum += 1

    if state == 2:
        print(msg['PUB_Board_ID'])
        if msg['PUB_Board_ID'] == "rocky":
            print(msg['Message_Content'])
            print("whats up")
            if (msg['Message_Content']) is "1":
                state = 3
                print("state changed to 2")

                # trackBlock = -1
                # smallRoverMsg = {"PUB_Board_ID": "Navig", "target_color": trackColor}
                # smallRoverMsg_send = json.dumps(smallRoverMsg)
                # client.publish("/team18/target", smallRoverMsg_send, retain=False)
        else:
            armMsg = {"PUB_Board_ID": "jacob", "Message_Content": "1"}
            roverMsg_send = json.dumps(roverMsg)
            armMsg_send = json.dumps(armMsg)
            client.publish("/team18/sensor", roverMsg_send, retain=False)
            if not isArm1Sent:
                client.publish("/team18/jacob", armMsg_send, retain=False)
                isArm1Sent = 1
            msgNum += 1
        trackBlock = -1
        curType = 2
        # trackColor = 0
        trackDistance = 0
        trackAngle = 1
        trackDirect = 0
        curObjectNum = 0

    if state == 3:
        curObjectNum = int(msg['Object_Detected'])
        if curObjectNum == 0:
            print("zero")
            trackBlock = -1
            # trackColor = 0
            trackDistance = 0
            trackAngle = 1
            curType = 2

        else:
            trackBlock = int(msg['TrackingIndex0'])
            print(trackBlock)
            # trackColor = int(msg['Color0'])
            print(trackColor)
            trackAngle = int(msg['Angle0'])
            print(trackAngle)
            trackDistance = int(msg['IR_Distance'])
            print(trackDistance)
            curType = 0
            state = 4
        roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
        print(roverMsg)
        roverMsg_send = json.dumps(roverMsg)
        client.publish("/team18/sensor", roverMsg_send, retain=False)
        msgNum += 1

    if state == 4:
        curObjectNum = int(msg['Object_Detected'])
        if curObjectNum == 0:
            curType = 4
        else:
            isTrackFound = False
            for i in range(curObjectNum):
                print('TrackingIndex' + str(i))
                # print(i)
                if trackBlock is not int(msg['TrackingIndex' + str(i)]):
                    print("trackblock: ", trackBlock)
                    print("trackingIndex: ", msg['TrackingIndex' + str(i)])
                    continue
                isTrackFound = True
                # trackColor = int(msg['Color' + str(i)])
                print('Color' + str(i))
                trackAngle = int(msg['Angle' + str(i)])
                print('Angle' + str(i))
                trackDistance = int(msg['IR_Distance'])
                print(trackDistance)
                trackDirect = int(msg['Direction' + str(i)])
            if isTrackFound:
                if trackAngle >= -1 and trackAngle <= 1:
                    if trackDistance <= 11 and curType == 1:
                        state = 5
                        curType = 3
                        print("state changed to arm grabbing")
                        sensorMsg = {"PUB_Board_ID": "navig", "Message_ID": 1, "Message_Content": "Dropped Up."}
                        sensorMsg_send = json.dumps(sensorMsg)
                        client.publish("/team18/navig", sensorMsg_send, retain=False)
                    else:
                        curType = 1
                else:
                    curType = 0
            else:
                # trackBlock = -1
                # trackColor = 0
                # trackDistance = 0
                # trackAngle = 0
                # curType = 2
                trackBlock = int(msg['TrackingIndex0'])
                # print(trackBlock)
                # trackColor = int(msg['Color0'])
                # print(trackColor)
                trackAngle = int(msg['Angle0'])
                # print(trackAngle)
                trackDistance = int(msg['IR_Distance'])
                # print(trackDistance)
                curType = 0

        roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
        print(roverMsg)
        roverMsg_send = json.dumps(roverMsg)
        client.publish("/team18/sensor", roverMsg_send, retain=False)
        msgNum += 1

    if state == 5:
        roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
        print(roverMsg)
        print(state)
        print(msg['PUB_Board_ID'])
#        print(msg['Message_Content'])
        if msg['PUB_Board_ID'] == "rocky":
            print("same board ID")
            if msg['Message_Content'] == "2":
                print("change to State 0")
                state = 6
                smallRoverMsg = {"PUB_Board_ID": "Navig", "target_color": trackColor, "ready": "1"}
                smallRoverMsg_send = json.dumps(smallRoverMsg)
                client.publish("/team18/target", smallRoverMsg_send, retain=False)
                trackBlock = -1
                curType = 5
                trackColor = 0
                trackDistance = 0
                trackAngle = 1
                trackDirect = 0
                curObjectNum = 0
        else:
            armMsg = {"PUB_Board_ID": "jacob", "Message_Content": "2"}
            roverMsg_send = json.dumps(roverMsg)
            armMsg_send = json.dumps(armMsg)
            client.publish("/team18/sensor", roverMsg_send, retain=False)
            client.publish("/team18/jacob", armMsg_send, retain=False)
            msgNum += 1
            isArm1Sent = 0

    if state == 6:
        roverMsg = {"Message_ID": msgNum, "angle": trackAngle, "distance": trackDistance, "type": curType}
        roverMsg_send = json.dumps(roverMsg)
        client.publish("/team18/sensor", roverMsg_send, retain=False)
        state = 0
        prevState = 6

client = mqtt.Client()
# client.connect("localhost",1883,60)
client.connect("192.168.4.1", 1883, 60)  # <--this ip address of broker
client.on_connect = on_connect
client.on_message = on_message
client.loop_forever()