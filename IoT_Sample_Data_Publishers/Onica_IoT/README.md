This publisher is taken from: https://github.com/onicagroup/sample-iot-thing <br/>
Updated, modified, and simplified by Stephen Borsay for test use.<br/>
Requires NPM to be installed on your computer:  https://docs.npmjs.com/downloading-and-installing-node-js-and-npm <br/>



A really simple IoT "thing" for connecting to AWS IoT.

Get an IoT certificate & key from AWS IoT Core and place them root directory, named cert.pem and private.pem.<br/>
CA certificate (x.509 root) is provided for you as it is fungible.<br/>
All files should be in the same folder on the same level.<br/>


Run '***npm install***' to download dependencies<br/>

Run this simulator with:<br/>

npm start HOST STAGE CLIENTID<br/>

Where HOST is the MQTT endpoint for an AWS account, STAGE is the name of an environment (dev, prod, etc), and CLIENTID is a unique identifier for this "thing".
(example: **npm start a32qaa131oyees-ats.iot.us-east-1.amazonaws.com aaa bbb**

See messages published to the IoT topic **iotsample/STAGE/CLIENTID/data** in the AWS IoT Core Console.<br/>

Need help with your IoT project? Check out IoTanium @ http://www.onica.com/iotanium <br/>

----------------------
changes from original thing.js

<sup>
<br/>
A) Full file names of Private and Client added, original truncated file extentions throw an error<br/>
B) Random function fixed and simplified.  Old version caused 'sticky' numbers<br/>
C) Removed package-lock.json as files is regenerated  with 'npm init' so redundent<br/>
</sup>

