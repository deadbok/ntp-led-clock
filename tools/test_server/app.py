from flask import Flask, request, jsonify
import time

app = Flask(__name__)

clock_config = {
    "ntp_server": "pool.ntp.org",
    "tz_index": 15,
    "tz_value": "",
    "time": "01:02:03",
    "brightness": 5,
    "ssid": "wtf",
    "ip": "127.0.0.1",
    "hostname": "kurwa",
    "ntp": True 
}

ssids = {
    "test","PrettyFlyForAWiFi", "YouSee12A7"
}

@app.get("/config")
def config_get():
    print("Sending config: ")
    print(clock_config)
    return(clock_config)

@app.post("/config")
def config_post():
    print("Recieving config: ")
    print(request.data)
    clock_config = request.data
    return clock_config

@app.get("/ssids")
def ssid_get():
    # Emulate wifi scan delay
    time.sleep(5)
    print("Sending SSIDs: ")
    print(ssids)
    return(ssids)

@app.post("/passwd")
def passwd_post():
    print("Recieving password: ")
    print(request.data)
    return(request.data)

#@app.get("/brightness")
#def brightness_get():
#    return {
#        "brightness": str(brightness)
#    }

#@app.post("/brightness")
#def brightness_post():
#    brightness = request.form["brightness"]
#    print("Brightness set to " + brightness)
#    return None

#@app.get("/ntp_state")
#def ntp_get():
#    return {
#        "ntp_state": str(ntp_state)
#    }

#@app.post("/ntp_state")
#def ntp_post():
#    ntp_state = request.form["ntp"]
#    print("NTP set to " + ntp_state)
#    return None

#@app.get("/ip")
#def ip_get():
#    return {
#        "ip": "127.0.0.1"
#    }
