load('api_config.js');
load('api_gpio.js');
load('api_mqtt.js');
load('api_net.js');
load('api_sys.js');
load('api_timer.js');
load('api_math.js'); //for Math.random

let pubTopic = 'myTopic/getPayload';
let subTopic = 'myTopic/sendPayload';
let Button0 = 0;  //Cfg.get('pins.button')


GPIO.set_button_handler(Button0, GPIO.PULL_UP, GPIO.INT_EDGE_NEG, 200, function() {
  let message = JSON.stringify({ 'Temperature': Math.random() * 100,
                                 'Humidity':    Math.random() * 50,   
								                 'Time' :       Timer.fmt('%H:%M:%S', Timer.now()),
								                 'Device_ID' :  Cfg.get('device.id')
								 });
  let ok = MQTT.pub(pubTopic, message, 1);
  print('Published:', ok);
 
  print('The Time is:', Timer.fmt('%H:%M:%S', Timer.now()));
}, null);


MQTT.sub(subTopic, function(conn,subTopic, message2) {
   print('Got message:', message2);
}, null);