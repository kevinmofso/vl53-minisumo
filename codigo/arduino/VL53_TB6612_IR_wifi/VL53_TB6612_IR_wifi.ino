#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "vl.h"
#include "TB6612.h"
#include "IR_Remote.h"
#include "QTR1A.h"

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
uint16_t lecturas_mm[NUM_SENSORS];
TB6612 motores(7,6,10,20,5,21);

// Interfaz para iPhone (Safari)
const char* html_interface = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<style>
  body{background:#0a0f14;color:white;font-family:sans-serif;margin:0;overflow:hidden;}
  canvas{display:block;background:#0a0f14;}
  #btn{position:absolute;top:10px;right:10px;padding:12px;background:#00c8ff;color:white;border:none;border-radius:5px;font-weight:bold;z-index:10;}
</style></head><body>
<button id="btn" onclick="mostrarG=!mostrarG">GRÁFICO</button>
<canvas id="c"></canvas>
<script>
  let socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  let sensores = [0,0,0,0,0], historial = [[],[],[],[],[]], mostrarG = true;
  const canvas = document.getElementById('c'), ctx = canvas.getContext('2d');
  socket.onmessage = (e) => {
    let d = e.data.split(',');
    if(d[0]=='D') {
      for(let i=0; i<5; i++) {
        let v = parseFloat(d[i+1]); sensores[i] = v;
        historial[i].push(v > 1200 ? 0 : v);
        if(historial[i].length > 60) historial[i].shift();
      }
    }
  };
  function draw() {
    canvas.width = window.innerWidth; canvas.height = window.innerHeight;
    ctx.clearRect(0,0,canvas.width,canvas.height);
    let cx = mostrarG ? canvas.width*0.3 : canvas.width/2, cy = canvas.height/3;
    ctx.fillStyle="#333"; ctx.fillRect(cx-30, cy-40, 60, 80); // Robot
    sensores.forEach((v, i) => {
      let a = (Math.PI/4)*i - Math.PI, len = v>1200?0:v/4;
      ctx.strokeStyle = i==0?'cyan':i==1?'orange':i==2?'magenta':i==3?'green':'violet';
      ctx.lineWidth=4; ctx.beginPath(); ctx.moveTo(cx,cy);
      ctx.lineTo(cx+Math.cos(a)*len, cy+Math.sin(a)*len); ctx.stroke();
    });
    if(mostrarG) {
      let gx = canvas.width*0.55, gy = 80, gw = canvas.width*0.4, gh = 300;
      ctx.strokeStyle="rgba(255,255,255,0.2)"; ctx.strokeRect(gx,gy,gw,gh);
      historial.forEach((h,s) => {
        ctx.strokeStyle = s==0?'cyan':s==1?'orange':s==2?'magenta':s==3?'green':'violet';
        ctx.lineWidth=2; ctx.beginPath();
        h.forEach((v,i) => {
          let x = gx+(i*gw/60), y = (gy+gh)-(v*gh/1200);
          if(i==0) ctx.moveTo(x,y); else ctx.lineTo(x,y);
        });
        ctx.stroke();
      });
    }
    requestAnimationFrame(draw);
  }
  draw();
</script></body></html>
)rawliteral";

void setup() {
  Serial.begin(115200); // Esto va al USB CDC (Processing)
  init_vl();
  init_sensores_piso();
  motores.init_pines();
  IR_Init();

  WiFi.softAP("MiniSumo_Radar", "12345678");
  server.on("/", []() { server.send(200, "text/html", html_interface); });
  server.begin();
  webSocket.begin();
}

void loop() {
  server.handleClient();
  webSocket.loop();
  IR_ProcessSignals();

  leerTodosLosSensores(lecturas_mm);
  int q_izq, q_der;
  leer_piso(q_izq, q_der);

  String trama = "D";
  for (int i = 0; i < NUM_SENSORS; i++) trama += "," + String(lecturas_mm[i]);
  trama += "," + String(q_izq) + "," + String(q_der);

  Serial.println(trama); // Telemetría por cable
  webSocket.broadcastTXT(trama); // Telemetría por Wi-Fi
  
  delay(20);
}