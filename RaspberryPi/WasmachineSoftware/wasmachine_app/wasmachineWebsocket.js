var list = "<ul>"
var ws
var naam

function WebSocketConnect() {
  naam = document.location.hostname;
  if ("WebSocket" in window) {
     //ws = new WebSocket("ws://0.0.0.0:8080");
     ws = new WebSocket("ws://"+document.location.hostname+":8080");
     ws.onopen = function(evt) { onOpen(evt) };
     ws.onclose = function(evt) { onClose(evt) };
     ws.onmessage = function(evt) { onMessage(evt) };
     ws.onerror = function(evt) { onError(evt) };
  }
  else  {
     alert("WebSocket NOT supported by your Browser!");
  }
}


function sendMessage() {
	ws.send(JSON.stringify(wasmachineDatapack));
}

function sendWebStatus() {
	ws.send(wasmachineWebStatus);
}

function closeConnection() {
	console.log("connection closed");
	ws.send("NEW_CONNECTION " + naam + " gaat weg.");
	ws.close();
}

function onOpen (evt) {
	console.log("connection open");
	ws.send("NEW_CONNECTION " + naam + " heeft zich aangesloten bij het gesprek");
 }

function onMessage (evt){
	console.log(evt.data);
	var status = evt ;
	document.getElementById("status").innerHTML = status;
 }

function onClose (evt){
	console.log("connection closed");
 }

 function onError (evt){
 	console.log("websocket error" + evt.data);
  }

  function append(message) {
  }
