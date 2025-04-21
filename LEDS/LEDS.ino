#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// WiFi y AP
const char* apSSID = "AP-Candela-Maqueta";
const char* apPass = "candelapereira";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
const byte DNS_PORT = 53;

DNSServer dnsServer;
WebServer server(80);

// Definición de cada luz/botón
struct Light {
  const char* id;        // Identificador interno (también usado en el HTML)
  const char* label;     // Texto a mostrar
  const char* section;   // Grupo/Sección: "Planta Baja", "Planta Alta", etc.
  uint8_t     pin;       // GPIO de la ESP32
};

// Aquí modificas este arreglo para añadir/quitar elementos
Light lights[] = {
  {"quincho",     "Quincho",        "Planta Baja", 2},
  {"patio",       "Patio",          "Planta Baja", 4},
  {"comedor",     "Comedor",        "Planta Baja", 5},
  {"entrada",     "Entrada",        "Planta Baja", 18},
  {"garage",      "Garage",         "Planta Baja", 19},
  {"sala",        "Sala de estar",  "Planta Alta", 21},
  {"escalera_pa", "Escalera PA",    "Planta Alta", 22},
  {"hab1",        "Habitación 1",   "Planta Alta", 23},
  {"hab2",        "Habitación 2",   "Planta Alta", 26},
  {"espacio",     "Espacio",        "Planta Alta", 25}
};

const char* title   = "AP-Candela-Maqueta";
const char* h1Text  = "Control de Iluminación";

const size_t numLights   = sizeof(lights) / sizeof(lights[0]);
const char*  sections[]  = { "Planta Baja", "Planta Alta" };
const size_t numSections = sizeof(sections) / sizeof(sections[0]);

// Devuelve el GPIO según el id
uint8_t getPinById(const String& id) {
  for (size_t i = 0; i < numLights; i++) {
    if (id == lights[i].id) return lights[i].pin;
  }
  return 255;
}

void handleRoot() {
  String pageTitle = String(title);
  String pageH1    = String(h1Text);

  // Inicio del HTML
  String html = String(R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>)rawliteral")
    + pageTitle
    + R"rawliteral(</title>
  <style>
    body { font-family: sans-serif; margin: 0; background: #f0f0f0; padding: 1rem; }
    h1 { text-align: center; font-size: 1.6rem; margin-bottom: 1rem; }
    section { background: white; margin-bottom: 1rem; padding: 1rem;
      border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.05); }
    h2 { font-size: 1.2rem; margin-top: 0; }
    .grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(120px, 1fr)); gap: 1rem; }
    .card { background: #fafafa; border: 1px solid #ddd; border-radius: 8px;
      padding: 0.5rem; text-align: center; }
    .switch { position: relative; display: inline-block; width: 50px; height: 28px; margin-top: 0.5rem; }
    .switch input { display: none; }
    .slider { position: absolute; cursor: pointer; top: 0; left: 0;
      right: 0; bottom: 0; background-color: #ccc; transition: 0.4s; border-radius: 34px; }
    .slider:before { position: absolute; content: ""; height: 22px; width: 22px;
      left: 3px; bottom: 3px; background-color: white; transition: 0.4s; border-radius: 50%; }
    input:checked + .slider { background-color: #66bb6a; }
    input:checked + .slider:before { transform: translateX(22px); }
    .footer-buttons { display: flex; justify-content: center; gap: 1rem; margin-top: 1rem; }
    button { background: #2196f3; border: none; color: white; padding: 0.5rem 1rem;
      border-radius: 5px; font-size: 1rem; cursor: pointer; }
    button.off { background: #e53935; }
    a.github-link { color: #0366d6; text-decoration: none; font-weight: bold; }
    a.github-link:hover { text-decoration: underline; }
  </style>
</head>
<body>
  <h1>)rawliteral"
    + pageH1
    + R"rawliteral(</h1>
)rawliteral";

  // Secciones dinámicas
  for (size_t s = 0; s < numSections; s++) {
    html += "<section><h2>" + String(sections[s]) + "</h2><div class=\"grid\">";
    for (size_t i = 0; i < numLights; i++) {
      if (String(lights[i].section) == sections[s]) {
        html += "<div class=\"card\"><div>" + String(lights[i].label) + "</div>"
             + "<label class=\"switch\"><input type=\"checkbox\" id=\"" + String(lights[i].id) + "\">"
             + "<span class=\"slider\"></span></label></div>";
      }
    }
    html += "</div></section>";
  }

  // Botones globales + enlace GitHub
  html += R"rawliteral(
  <div class="footer-buttons">
    <button onclick="toggleAll(1)">Encender Todo</button>
    <button class="off" onclick="toggleAll(0)">Apagar Todo</button>
  </div>
  <div style="text-align:center; margin-top:1rem;">
    <a class="github-link" href="https://github.com/lordbasex/control-de-Iluminacion" target="_blank">
      Código fuente GitHub
    </a>
  </div>

  <script>
    const ids = )rawliteral";

  html += "[";
  for (size_t i = 0; i < numLights; i++) {
    html += "\"" + String(lights[i].id) + "\"";
    if (i + 1 < numLights) html += ",";
  }
  html += "]";
  html += R"rawliteral(;

    ids.forEach(id => {
      const cb = document.getElementById(id);
      cb.addEventListener("change", () => {
        fetch(`/toggle?pin=${id}&state=${cb.checked ? 1 : 0}`);
      });
    });

    function toggleAll(state) {
      ids.forEach(id => {
        const cb = document.getElementById(id);
        cb.checked = state === 1;
        fetch(`/toggle?pin=${id}&state=${state}`);
      });
    }
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleToggle() {
  String pinName = server.arg("pin");
  String state   = server.arg("state");
  uint8_t pin    = getPinById(pinName);
  if (pin == 255) {
    server.send(400, "text/plain", "Pin desconocido");
    return;
  }
  digitalWrite(pin, state == "1" ? HIGH : LOW);
  server.send(200, "text/plain", "OK");
}

void handleCaptive() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleNotFound() {
  String path = server.uri();
  if (path.endsWith("generate_204") || path.endsWith("hotspot-detect.html") || path.endsWith("connecttest.txt")) {
    handleCaptive();
  } else {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializar pines
  for (size_t i = 0; i < numLights; i++) {
    pinMode(lights[i].pin, OUTPUT);
    digitalWrite(lights[i].pin, LOW);
  }

  // Configurar AP y portal cautivo
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID, apPass);
  dnsServer.start(DNS_PORT, "*", apIP);

  // Rutas del servidor
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/generate_204", handleCaptive);
  server.on("/hotspot-detect.html", handleCaptive);
  server.on("/connecttest.txt", handleCaptive);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("AP iniciado. Conéctate a WiFi '" + String(apSSID) + "' y abre http://192.168.4.1");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
