from flask import Flask, request, jsonify

app = Flask(__name__)

clock_config = {
    "ntp_server": "",
    "tz_index": 15,
    "tz_value": "",
    "time": "00:00:00",
    "brightness": 5,
    "ssid": "wtf",
    "ip": "127.0.0.1",
    "hostname": "kurwa",
    "ntp": True 
}

@app.get("/config")
def send():
    print("Sending: ")
    print(clock_config)
    return(clock_config)

@app.post("/config")
def get():
    print("Recieve: ")
    print(request.json)
    #TODO: Get data into clock_config
    return clock_config

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
