
var Thread = Java.type("java.lang.Thread");

var topic = "hhh"
var waitTime = 2000;
var iterations = 10;


var temp= Math.random()*130;
var humid = Math.random()*100;
var ts = Date.now();

var IoT_Payload = { 
  "temperature" :  temp, 
  "humidity"    :  humid, 
  "timestamps"  :  ts
}

var payload = JSON.stringify(IoT_Payload)
out(payload)
//out(sammy)
//var IoT_Payload = JSON.parse(sammy.data);
//out(IoT_Payload)

function execute(action) {
    out("Test Script: " + action.getName());
    for (var i = 0; i < 10; i++) {
        switchON(payload);
        Thread.sleep(500);
        switchOFF();
        Thread.sleep(500);
    }
    action.setExitCode(0);
    action.setResultText("done.");
    out("Test Script: Done");
    return action;
}

function switchON(myPayload) {
    out("fountain ON");
    mqttManager.publish("home/garden/fountain", "ON");
	//var IoT_Payload = JSON.parse(sammy);
	mqttManager.publish("home/garden/fountain", myPayload);
}

function switchOFF() {
    out("fountain OFF");
    mqttManager.publish("home/garden/fountain", "OFF");
}

function out(message){
     output.print(message);
}
