
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// https://github.com/bblanchon/ArduinoJson
#include <ArduinoJson.h>
 
const char* ssid     = "YourRouterID";
const char* password = "YourRouterPassword";
 
ESP8266WebServer server(80);
 
 
void handle_root() {

  // This is there html.min.cpp goes
  server.send(200, "text/plain", '<!DOCTYPE html><html lang="en" ng-app="app"><head>    <meta charset="utf-8">    <meta http-equiv="X-UA-Compatible" content="IE=edge">    <meta name="viewport" content="width=device-width, initial-scale=1">    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->    <meta name="description" content="">    <meta name="author" content="">    <link rel="icon" href="../../favicon.ico">    <title>Starter Template for Bootstrap</title>    <!-- Latest compiled and minified CSS -->    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" integrity="sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7" crossorigin="anonymous">    <!-- Optional theme -->    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap-theme.min.css" integrity="sha384-fLW2N01lMqjakBkx3l/M9EahuwpSfeNvV63J5ezn3uZzapT0u7EYsXMjQV+0En5r" crossorigin="anonymous">    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.5/angular.min.js"></script>    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->    <!--[if lt IE 9]>      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>    <![endif]-->    <style type="text/css">        body {            margin-top: 60px;        }        a{            cursor:pointer;        }    </style>    <script type="text/javascript">        var app = angular.module(\'app\', []);        app.controller("Main", function ($scope, $http, $interval) {                        $scope.data = {                info:[]            };                        $scope.loading = false;            $scope.do = function (url) {                $http({                    method: \'GET\',                    url: url                }).then(function successCallback(response) {                    $scope.update();                }, function errorCallback(response) {                    //alert(response);                });            };            $scope.update = function () {                if($scope.loading) return;                $scope.loading = true;                $http({                    method: \'GET\',                    url: "/get-data.json"                }).then(function successCallback(response) {                    console.log(response);                    $scope.data = response.data;                    $scope.loading = false;                }, function errorCallback(response) {                    alert(response);                });                            };                        $interval($scope.update, 1000);        });    </script></head><body ng-controller="Main">    <nav class="navbar navbar-inverse navbar-fixed-top">        <div class="container">            <div class="navbar-header">                <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">                    <span class="sr-only">Toggle navigation</span>                    <span class="icon-bar"></span>                    <span class="icon-bar"></span>                    <span class="icon-bar"></span>                </button>                <a class="navbar-brand" href="#">ESP Control</a>            </div>            <div id="navbar" class="collapse navbar-collapse">                <ul class="nav navbar-nav">                    <li class="active"><a href="#">Control</a></li>                    <li><a ng-click="do(\'/reboot\')">Reboot</a></li>                                    </ul>            </div>            <!--/.nav-collapse -->        </div>    </nav>    <div class="container">                <form>            <div class="form-group" ng-repeat="info in data.info">                <label ng-bind="info.name"></label>                <p class="help-block" ng-bind="info.state"></p>                                <div class="btn-group" role="group">                    <button type="button" class="btn btn-default" ng-repeat="button in info.buttons" ng-click="do(button.action)" ng-bind="button.name"></button>                </div>            </div>        </form>                <pre ng-bind="data|json"></pre>                    </div>    <!-- /.container --></body></html>');
  delay(100);
}
 
void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  
 
   /**
    * Connecting to WiFi and getting the ball rolling
    */

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   

  /**
   * Setting up our user interface
   */


  // Setup data
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& state;
  JsonArray& buttons;
  JsonObject& button;
  char buffer[1024];

  JsonObject& root = jsonBuffer.createObject();
  JsonArray& info = root.createNestedObject("info");


  // Power Setup
  state = info.createNestedObject("power");
  buttons = state.createNestedArray("buttons");

  // The name, ex: Temperature
  state["name"] = "Power";

  // Shutdown Button
  button = buttons.createNestedObject();
  button["name"] = "Shutdown";
  button["action"] = "/shutdown";

  // Reboot Button
  button = buttons.createNestedObject();
  button["name"] = "Reboot";
  button["action"] = "/reboot";

  
  // Temperature Setup
  state = info.createNestedObject("temp");
  buttons = state.createNestedArray("buttons");

  // The name, ex: Temperature
  state["name"] = "Temperature";






  /**
   * Our URL Endpoints
   */

  server.on("/", handle_root);
  
  // This tells the clients what's up every second
  server.on("/get-data.json", [](){ 

    // Get and update State of Power
    info["power"]["state"] = "On";

    // Get and update State of Temperature
    info["temp"]["state"] = getTemperature();

    // Generate the JSON
    root.printTo(buffer, sizeof(buffer));

    // Send JSON to client
    server.send(200, "text/json", buffer);
  });


 
  // When someone clicks the "Shutdown button"
  server.on("/shutdown", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    
    // Do Something
    
    server.send(200, "text/json", "{success:true}");
  });


  // When someone clicks the "Reboot Button"
  server.on("/reboot", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    
    // Do Something
    
    server.send(200, "text/json", "{success:true}");
  });




  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
} 
char* getTemperature(void){

  // Actually get the temperature here
  return "55F";

}
