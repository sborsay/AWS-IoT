load('api_aws.js');
load('api_azure.js');
load('api_config.js');
load('api_dash.js');
load('api_events.js');
load('api_gcp.js');
load('api_gpio.js');
load('api_mqtt.js');
load('api_shadow.js');
load('api_timer.js');
load('api_sys.js');
load('api_watson.js');
load('api_math.js'); //for Math.random


let btn = Cfg.get('board.btn1.pin');              // Built-in button GPIO
let led = Cfg.get('board.led1.pin');              // Built-in LED GPIO number
let onhi = Cfg.get('board.led1.active_high');     // LED on when high?
let state = {on: false, btnCount: 0, uptime: 0};  // Device state
let online = false;                               // Connected to the cloud?

let setLED = function(on) {
  let level = onhi ? on : !on;
  GPIO.write(led, level);
  print('LED on ->', on);
};

GPIO.set_mode(led, GPIO.MODE_OUTPUT);
setLED(state.on);

let reportState = function() {
  Shadow.update(0, state);
};

// Update state every second, and report to cloud if online
Timer.set(1000, Timer.REPEAT, function() {
  state.uptime = Sys.uptime();
  state.ram_free = Sys.free_ram();
  print('online:', online, JSON.stringify(state));
  if (online) reportState();
}, null);

// Set up Shadow handler to synchronise device state with the shadow state
Shadow.addHandler(function(event, obj) {
  if (event === 'UPDATE_DELTA') {
    print('GOT DELTA:', JSON.stringify(obj));
    for (let key in obj) {  // Iterate over all keys in delta
      if (key === 'on') {   // We know about the 'on' key. Handle it!
        state.on = obj.on;  // Synchronise the state
        setLED(state.on);   // according to the delta
      } else if (key === 'reboot') {
        state.reboot = obj.reboot;      // Reboot button clicked: that
        Timer.set(750, 0, function() {  // incremented 'reboot' counter
          Sys.reboot();                 // Sync and schedule a reboot
        }, null);
      }
    }
    reportState();  // Report our new state, hopefully clearing delta
  }
});

if (btn >= 0) {
  let btnCount = 0;
  let btnPull, btnEdge;
  if (Cfg.get('board.btn1.pull_up') ? GPIO.PULL_UP : GPIO.PULL_DOWN) {
    btnPull = GPIO.PULL_UP;
    btnEdge = GPIO.INT_EDGE_NEG;
  } else {
    btnPull = GPIO.PULL_DOWN;
    btnEdge = GPIO.INT_EDGE_POS;
  }
  GPIO.set_button_handler(btn, btnPull, btnEdge, 20, function() {
    state.btnCount++;
	
    let message = JSON.stringify({ 'Temperature': Math.random() * 100,
                                 'Humidity':      Math.random() * 50,   
								                 'Time' :       Timer.fmt('%H:%M:%S', Timer.now()),
								                 'Device_ID' :  Cfg.get('device.id')
								 });
	
	
	
    let sendMQTT = true;
    if (Azure.isConnected()) {
      print('== Sending Azure D2C message:', message);
      Azure.sendD2CMsg('', message);
      sendMQTT = false;
    }
    if (GCP.isConnected()) {
      print('== Sending GCP event:', message);
      GCP.sendEvent(message);
      sendMQTT = false;
    }
    if (Watson.isConnected()) {
      print('== Sending Watson event:', message);
      Watson.sendEventJSON('ev', {d: state});
      sendMQTT = false;
    }
    if (Dash.isConnected()) {
      print('== Click!');
      // TODO: Maybe do something else?
      sendMQTT = false;
    }
    // AWS is handled as plain MQTT since it allows arbitrary topics.
    if (AWS.isConnected() || (MQTT.isConnected() && sendMQTT)) {
      let topic = 'devices/' + Cfg.get('device.id') + '/events';
      print('== Publishing to ' + topic + ':', message);
      MQTT.pub(topic, message, 0 /* QoS */);
    } else if (sendMQTT) {
      print('== Not connected!');
    }
  }, null);
}

Event.on(Event.CLOUD_CONNECTED, function() {
  online = true;
  Shadow.update(0, {ram_total: Sys.total_ram()});
}, null);

Event.on(Event.CLOUD_DISCONNECTED, function() {
  online = false;
}, null);
