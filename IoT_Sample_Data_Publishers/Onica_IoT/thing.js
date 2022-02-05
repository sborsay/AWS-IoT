const awsIot = require('aws-iot-device-sdk')

const host = process.argv[2]
const stage = process.argv[3]
const clientId = process.argv[4]
const topic = `iotsample/${stage}/${clientId}/data`

start()

function start(profile) {
  console.log(`Host: ${host} | ClientId: ${clientId} | Topic: ${topic}`)

  const device = awsIot.device({
    host: host,                  //leave this as it is here: 'host', the endpoint is aquired through the command line 
    keyPath: 'private.pem.key',  //Make sure the entire file extentions here match yours 
    certPath: 'cert.pem.crt',
    caPath: 'ca.pem',
    clientId: clientId,
    will: {
      topic: topic,
      payload: JSON.stringify({
        thingId: clientId,
        disconnected: true
      })
    }
  });

  device
    .on('connect', function() {
      console.log('connected');
    })
    .on('close', function() {
      console.log('closed');
    })
    .on('reconnect', function() {
      console.log('reconnecting');
    })
    .on('message', function(topic, payload) {
      console.log('message', topic, payload.toString());
    });


    setInterval(function() {
	                                                    //Use math.floor() unless you want to deal with decimal percision
      let temperature = Math.floor(Math.random() * (110));  //Obviously Fahrenheit
      let humidity = Math.floor(Math.random() * (100));     //The humidity index can never go below zero or exceed 100

      device.publish(topic, JSON.stringify({
        thingId: clientId,
        time: Date.now(),
        temperature: temperature,
        humidity: humidity
      }));
    }, 5000);
}

