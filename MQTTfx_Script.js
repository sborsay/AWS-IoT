var Thread = Java.type("java.lang.Thread");

var topic = "hhh"
var waitTime = 2000;
var iterations = 10;

function execute(action) {
    out("Test Script: " + action.getName());
    for (var i = 0; i < 10; i++) {
        sendPayload();
        Thread.sleep(1000);
    }
    action.setExitCode(0);
    action.setResultText("done.");
    out("Test Script: Done");
    return action;
}

function sendPayload() {
		
	var temp = Math.round(Math.random()*130);
	var humid = Math.round(Math.random()*100);
	var ts = Date.now();
	
	var IoT_Payload = { 
  "temperature" :  temp, 
  "humidity"    :  humid, 
  "timestamps"  :  ts
}

var payload = JSON.stringify(IoT_Payload)

    out("payload sent");
    mqttManager.publish("home/garden/fountain", payload);
}


function out(message){
     output.print(message);
}
