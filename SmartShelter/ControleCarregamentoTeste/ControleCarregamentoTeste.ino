const int SinalReleBase = 12;
const int SinalReleApm = 11;
const int SensorBase = A0;
const int SensorBateria = A1;
#define DELTAANALOGV 0.1913580247
#define MAXBATERIA 736.4007782
#define MAXBASE 796.1089494
#define AMOSTRAS 12

void setup() {
  Serial.begin(9600);
  pinMode(SinalReleBase,OUTPUT);
  pinMode(SinalReleApm,OUTPUT);
  pinMode(SensorBase,INPUT);
  pinMode(SensorBateria,INPUT);
}

float LePortaAnalogica(uint8_t portaAnalogica) {
  float total = 0;  
  for (int i = 0; i < AMOSTRAS; i++) {
    total += analogRead(portaAnalogica);
    delay(5);
  }
  return total/(int)AMOSTRAS;
}

void AcionarRele(uint8_t portaDigital){
  digitalWrite(portaDigital,HIGH);
  delay(500);
}

void DesligarRele(uint8_t portaDigital){
  digitalWrite(portaDigital,LOW);
  delay(500);
}

void AcionarRelesPreCarregamento(){
  AcionarRele(SinalReleApm);
  AcionarRele(SinalReleBase);  
}

void AcionarRelesPosCarregamento(){
  DesligarRele(SinalReleBase); 
  DesligarRele(SinalReleApm); 
}

bool VerificaLeituraAnalogica(uint8_t portaAnalogica, float constanteAnalogica){
  float leituraAnalogica = LePortaAnalogica(portaAnalogica);
  Serial.print("\n\rConstante analógica é: ");
  Serial.print(leituraAnalogica);
  Serial.print("\n\rA tensão é: ");
  Serial.print(leituraAnalogica/1023);
  Serial.println ("V");
  if(leituraAnalogica >= (constanteAnalogica - (float)DELTAANALOGV) || leituraAnalogica <= (constanteAnalogica + (float)DELTAANALOGV))
	return true;
  return false;
}

bool VerificaTensaoDaBase(){
  if (VerificaLeituraAnalogica(SinalReleBase,(float)MAXBASE))
    return true;
  return false;
}

bool AcompanharCarregamento(){
  if (VerificaLeituraAnalogica(SinalReleApm,(float)MAXBATERIA))
  {
    Serial.print("Bateria carregada!");
	  AcionarRelesPosCarregamento();
	  return true;
  }
  Serial.print("Bateria não está carregada!");
  return false;
}

void CarregarBateria(){
  bool carregou = false;
  AcionarRelesPreCarregamento();
  while(carregou = false)
  {
	carregou = AcompanharCarregamento();
  }
}

void loop() {
  if(VerificaTensaoDaBase())
  {
	  Serial.print("Há tensão na base!");
	  CarregarBateria();
	  delay(300000);
  }
  Serial.print("Não há tensão na base!");
  delay(1000);
}
