void healthCheck()
{
  // check if time was retrieved and the mqtt connection
   if(mqttEnabled) {
      if (!MQTT_Client.connected())  mqttConnect();
   }
   // check if we need to get time
   if(!timeRetrieved) getTijd();
}
