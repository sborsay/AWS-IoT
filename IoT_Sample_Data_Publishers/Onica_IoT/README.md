This publisher is taken from: https://github.com/onicagroup/sample-iot-thing <br/>
Updated and simplified by Stephen Borsay for test use.<br/>



A Really simple IoT "thing" for connecting to AWS IoT.

Get an IoT certificate & key from AWS IoT Core and place them root directory, named cert.pem and private.pem.
CA certificate (x.509 root) is provided as it is fungible
all files should be in the same folder on the same level

Run '***npm install***' to download dependencies

Run this simulator with:

npm start HOST STAGE CLIENTID

Where HOST is the MQTT endpoint for an AWS account, STAGE is the name of an environment (dev, prod, etc), and CLIENTID is a unique identifier for this "thing".
(example: **npm start a32qaa131oyees-ats.iot.us-east-1.amazonaws.com aaa bbb**

See messages published to the IoT topic **iotsample/STAGE/CLIENTID/data** in the AWS IoT Core Console.

Need help with your IoT project? Check out IoTanium @ http://www.onica.com/iotanium

----------------------
changes from Original
______________________
<sup>
<br/>
A) Full file names of Private and Client added, original truncated file extentions throw an error<br/>
B) Random function fixed and simplified.  Old version caused 'sticky' numbers<br/>
C) Removed package-lock.json as files is regenerated  with 'npm init' so redundent<br/>
</sup>

