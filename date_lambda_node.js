// Handler called by IoT Analytics
exports.handler = function handler(event, context, callback) {
    
//add timesteamp to incoming data and name it "ServersideTimestam"
        event[0].ServersideTimestamp = Date.now();
        
        // Return the data        
        callback(null, event);
 //   });
};
