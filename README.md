

#How To

Need to create these endpoints:

## /get-data.json

```json
{
    "info":[
        {
            "name": "Power",
            "state": "On",
            "buttons": [
                {
                    "name": "Shut Down",
                    "action": "/shutdown"
                },
                {
                    "name": "Reboot",
                    "action": "/reboot"
                }
            ]
        },
        {
            "name": "State 2",
            "state": "Not Working",
            "buttons": [
                {
                    "name": "Thing 1",
                    "action": "/action1-url"
                }
            ]
        }
    ]
}
```

Using something like this (assuming you use https://github.com/bblanchon/ArduinoJson):

```c
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& state;
JsonArray& buttons;
JsonObject& button;

JsonObject& root = jsonBuffer.createObject();
JsonArray& info = root.createNestedArray("info");


// State 1 Setup
state = info.createNestedObject();
buttons = state.createNestedArray("buttons");

// The name, ex: Temperature
state["name"] = "Power";

// It's current state: ex: 55F
state["state"] = "On";

// Shutdown Button
button = buttons.createNestedObject();
button["name"] = "Shutdown";
button["action"] = "/shutdown";

// Reboot Button
button = buttons.createNestedObject();
button["name"] = "Reboot";
button["action"] = "/reboot";



root.printTo(your-output-here);

```