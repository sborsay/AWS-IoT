Create these default roles if this option is not showing up in your data pipleline creation screen

![image](https://user-images.githubusercontent.com/16296900/165592009-9c63bf58-e370-4858-8e52-d0d98e2c67fc.png)



Create Role one: call it somethign like: **myDataPipelineDefaultRole**

attach the policy called **AWSDataPipelineRole** (listed above) to the myDataPipelineDefaultRole role.
(_Default policy for the DataPipelineDefaultRole for Data Pipeline service role_)

Next


Create Role one: call it somethign like: **myDataPipelineDefaultResourceRole**

attach the policy called **AmazonEC2RoleforDataPipelineRole** (listed above) to this role.
(_Default policy for the Amazon EC2 Role for Data Pipeline service role_)

