#include <Main.hpp>




String cpiId = "";


// const char* host = "espitag";
// const char* ssid = "ESP32-iTag";
// const char* password = "PASSWORD";

WebServer server(80);

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String pageLogin = 
"<form>"
"<h1>ESP32 LOGIN</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>" + style;
 
/* Server Index Page */
String pageIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;


unsigned long lastSend = 0;
int accessSend = 1;
int notConnecting = 1;
char buf[40];



bool auth{false};



bool getAuth()
{
  return auth;
}

bool setAuth(bool state)
{
  auth = state;
  return true;
}

bool isAuth()
{
  return getAuth();
}


void getCpuID()
{
    uint64_t chipid=ESP.getEfuseMac();
    cpiId = String((uint16_t)(chipid>>32)) + String((uint32_t)chipid);

    //Serial.print("ESP32 Chip ID = " + ssidAP + "-");
    //Serial.println(cpiId);

    //Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));
    //Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.   
}



void setupServer()
{
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", pageLogin);
    });
    
    server.on("/index", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", pageIndex);
    });
    
    // /*handling uploading firmware file */
    // server.on("/update", HTTP_POST, []() {
    //   server.sendHeader("Connection", "close");
    //   server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    //   ESP.restart();
    // }, []() {
    //   HTTPUpload& upload = server.upload();
    //   if (upload.status == UPLOAD_FILE_START) {
    //     //Serial.printf("Update: %s\n", upload.filename.c_str());
    //     if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
    //       Update.printError(Serial);
    //     }
    //   } else if (upload.status == UPLOAD_FILE_WRITE) {
    //     /* flashing firmware to ESP*/
    //     if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
    //       Update.printError(Serial);
    //     }
    //   } else if (upload.status == UPLOAD_FILE_END) {
    //     if (Update.end(true)) { //true to set the size to the current progress
    //       //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    //     } else {
    //       Update.printError(Serial);
    //     }
    //   }
    // });

    server.begin();
}


void setup(void) 
{
  Serial.begin(115200);
  Serial.setDebugOutput(0);

  getCpuID();

    
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // SPIFFS.format();

  // IPAddress apIP(10, 10, 10, 10);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  // WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // WiFi.softAP((ssidAP + "-" + cpiId).c_str(), passwordAP.c_str());
  WiFi.softAP(WLAN_AP_SSID, WLAN_AP_PASSWORD);


  setupServer();

  // Connect to WiFi network
  //WiFi.begin(ssid, password);
  //Serial.println("");

  //int cntConnect = 0;
  
  // Wait for connection
  //while (WiFi.status() != WL_CONNECTED && cntConnect < 5) {
   // delay(500);
    //Serial.print(".");
   // cntConnect++;
  //}
 // 
  //Serial.println("");
  //Serial.println("");
  //Serial.println("");

  //if(cntConnect == 5) notConnecting = 0;

  //if(notConnecting == 1)
  //{
    //Serial.println("");
    //Serial.print("Connected to ");
    //Serial.println(ssid);
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());

    /*use mdns for host name resolution*/
   // if (!MDNS.begin(host)) { //http://esp32.local
      //Serial.println("Error setting up MDNS responder!");
    //  while (1) {
    //    delay(1000);
     // }
    //}
    //Serial.println("mDNS responder started");
    /*return index page which is stored in serverIndex */

  //}
  
}

void loop(void)
{
  if(notConnecting == 1)
  {
    server.handleClient();
  }
  
     
  if(millis() > lastSend + 1000)
  {
      accessSend = 1;
      lastSend = millis();
  }

  delay(1);

}
