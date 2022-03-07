load('api_config.js');
load('api_gpio.js');
load('api_mqtt.js');
load('api_net.js');
load('api_sys.js');
load('api_timer.js');
load('api_math.js'); //for Math.random

let pubTopic = 'iot/outTopic';
let subTopic = 'iot/inTopic';
let Button0 = 0;  //Cfg.get('pins.button')


Timer.set(2000 /* milliseconds */, Timer.REPEAT, function() {
  let message = JSON.stringify({ 'temperature': Math.random() * 130,
                                 'humidity':    Math.random() * 100,   
								                 'timestamps':  Timer.fmt('%H:%M:%S', Timer.now()),
								                 'device_ID' :  Cfg.get('device.id')
								 });
  let ok = MQTT.pub(pubTopic, message, 1);
  print('Published:', ok);
 
  print('The Time is:', Timer.fmt('%H:%M:%S', Timer.now()));
}, null);


MQTT.sub(subTopic, function(conn,subTopic, message2) {
   print('Got message:', message2);
}, null);
