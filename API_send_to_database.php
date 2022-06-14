/ Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String body = "diastolic=";
  body += diastolic;
  body += "&systolic=";
  body += systolic;
  body += "&hr=";
  body += hr;

  String post = String("POST /api.php/bp HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" +
               "Content-Type: text/plain\r\n" +
               "Content-Length: " + body.length() + "\r\n\r\n" + // must be twice
               body + "\r\n" +
               "Connection: close";

  Serial.println("Full request:");
  Serial.println(post);
  
  client.print(post);
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}