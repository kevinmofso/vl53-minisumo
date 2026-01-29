import processing.serial.*;

Serial myPort;
float[][] historial; 
int maxPuntos = 150; 
float[] sensores = new float[0];
String estrategia = "ESPERANDO...";
String ultimoComando = "NINGUNO";

// --- NUEVA VARIABLE PARA EL BOTÓN ---
boolean mostrarGrafico = true; 

void setup() {
  size(1200, 800);
  historial = new float[5][maxPuntos]; 
  for (int s = 0; s < 5; s++) {
    for (int i = 0; i < maxPuntos; i++) historial[s][i] = 0;
  }
  try {
    String portName = Serial.list()[1]; 
    myPort = new Serial(this, portName, 115200);
    myPort.bufferUntil('\n');
  } catch (Exception e) {
    println("Error Serial.");
  }
}

void draw() {
  background(10, 15, 20);
  
  drawStatusPanel();
  drawRadar();
  drawBoton(); // Dibujamos el botoncito
  
  // Solo dibujamos el gráfico si la variable es verdadera
  if (mostrarGrafico) {
    drawGraph(); 
  }
  
  drawFloorPanel();
}

// --- FUNCIÓN PARA EL BOTÓN INTERACTIVO ---
void drawBoton() {
  int bx = 820; // Alineado con el gráfico
  int by = 110;
  int bw = 120;
  int bh = 30;
  
  // Cambia de color si está activo o no
  if (mostrarGrafico) fill(0, 200, 255, 200); 
  else fill(60, 60, 60, 200);
  
  stroke(255, 100);
  rectMode(CORNER);
  rect(bx, by, bw, bh, 5);
  
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(12);
  String txt = mostrarGrafico ? "OCULTAR GRAFICO" : "VER GRAFICO";
  text(txt, bx + bw/2, by + bh/2);
}

// --- DETECTAR CLIC DEL MOUSE ---
void mousePressed() {
  // Coordenadas del botón
  if (mouseX >= 820 && mouseX <= 820 + 120 && mouseY >= 110 && mouseY <= 110 + 30) {
    mostrarGrafico = !mostrarGrafico; // Cambia el estado (ON/OFF)
  }
}

void drawRadar() {
  pushMatrix();
  // Si el gráfico está oculto, centramos más el radar
  float posX = mostrarGrafico ? width/2 - 200 : width/2;
  translate(posX, height/2); 
  
  rectMode(CENTER);
  fill(40); noStroke();
  rect(-75, 10, 30, 70, 5); rect(75, 10, 30, 70, 5);
  fill(195, 176, 145); stroke(255, 50);
  rect(0, 0, 130, 150, 8); 

  if (sensores.length >= 5) {
    for (int i = 0; i < 5; i++) {
      float angulo = TWO_PI - (PI / 4) * i;
      if (sensores[i] < 1200) {
        float d = map(sensores[i], 0, 1200, 0, 250); 
        pushMatrix();
        rotate(angulo);
        stroke(getSensorColor(i), 180);
        strokeWeight(3);
        line(65, 0, 65 + d, 0);
        popMatrix();
      }
    }
  }
  popMatrix();
}

void drawGraph() {
  int gx = 820; 
  int gy = 150; 
  int gw = 320; 
  int gh = 500; 

  fill(15, 25, 35);
  stroke(0, 200, 255, 100);
  rectMode(CORNER);
  rect(gx, gy, gw, gh, 5);
  
  textAlign(RIGHT, CENTER);
  textSize(12);
  for(int i=0; i<=4; i++) {
    int valorMm = 1200 - (i * 300);
    float yPos = gy + (gh/4.0)*i;
    stroke(255, 20);
    line(gx, yPos, gx + gw, yPos); 
    fill(255, 150);
    text(valorMm + " mm", gx - 10, yPos);
  }

  noFill(); 
  for (int s = 0; s < 5; s++) {
    noFill(); 
    stroke(getSensorColor(s));
    strokeWeight(2.5);
    beginShape(); 
    for (int i = 0; i < maxPuntos; i++) {
      float vx = gx + (i * (float)gw / (maxPuntos - 1));
      float val = historial[s][i];
      if (val >= 1200 || val <= 0) val = 0; 
      float vy = map(val, 0, 1200, gy + gh, gy);
      vertex(vx, vy);
    }
    endShape(); 
    
    fill(getSensorColor(s));
    textAlign(LEFT, CENTER);
    float lastVal = historial[s][maxPuntos-1];
    float labelY = map((lastVal >= 1200 || lastVal <= 0) ? 0 : lastVal, 0, 1200, gy + gh, gy);
    text("S" + s + ": " + (lastVal >= 1200 ? "---" : int(lastVal)), gx + gw + 10, labelY);
  }
  strokeWeight(1);
}

color getSensorColor(int i) {
  color[] colors = {
    color(0, 255, 255), color(255, 200, 0), color(255, 0, 255), 
    color(0, 255, 100), color(200, 100, 255)
  };
  return colors[i % colors.length];
}

void drawStatusPanel() {
  fill(20, 40, 60, 220); stroke(0, 200, 255);
  rectMode(CORNER); rect(20, 20, 250, 90, 5);
  fill(0, 200, 255); textSize(12); textAlign(LEFT);
  text("MODO / ESTRATEGIA", 35, 45);
  fill(255); textSize(20); text(estrategia, 35, 80);

  fill(20, 40, 60, 220); stroke(200, 200, 0);
  rect(290, 20, 250, 90, 5);
  fill(200, 200, 0); textSize(12); text("COMANDO IR / ÁNGULO", 305, 45);
  fill(255); textSize(18); text(ultimoComando, 305, 80);
}

void drawFloorPanel() {
  if (sensores.length >= 2) {
    float vI = sensores[sensores.length-2];
    float vD = sensores[sensores.length-1];
    drawGroundSensor(width/2 - 300, height - 80, "QTR IZQ", vI);
    drawGroundSensor(width/2, height - 80, "QTR DER", vD);
  }
}

void drawGroundSensor(float x, float y, String label, float val) {
  fill(255, 150); textSize(13); text(label + ": " + int(val), x, y - 10);
  fill(30); noStroke(); rect(x, y, 200, 25, 4);
  fill(val < 1000 ? color(255, 150, 0) : color(0, 255, 150));
  rect(x, y, map(val, 4000, 0, 0, 200), 25, 4);
}

void serialEvent(Serial myPort) {
  try {
    String inString = myPort.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString);
      if (inString.startsWith("D,")) {
        String[] list = split(inString.substring(2), ',');
        sensores = new float[list.length];
        for (int i = 0; i < list.length; i++) {
          sensores[i] = float(list[i]);
          if (i < 5) {
            for (int j = 0; j < maxPuntos-1; j++) historial[i][j] = historial[i][j+1];
            historial[i][maxPuntos-1] = sensores[i];
          }
        }
      } 
      else if (inString.startsWith("E,")) estrategia = inString.substring(2);
      else if (inString.startsWith("C,")) ultimoComando = inString.substring(2);
    }
  } catch (Exception e) {}
}
