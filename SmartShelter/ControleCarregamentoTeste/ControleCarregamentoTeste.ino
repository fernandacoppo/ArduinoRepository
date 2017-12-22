const int SinalReleBase = 12;
const int SinalReleApm = 11;
const int SensorBase = A0;
const int SensorBateria = A1;
#define aRef 5
#define DELTAANALOGV 0.1913580247
#define MAXBATERIA 736.4007782
#define MAXBASE 746.3521401
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
  return total/(float)AMOSTRAS;
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
  Serial.print("\n\rConstante analÃ³gica Ã©: ");
  Serial.print(leituraAnalogica);
  Serial.print("\n\rA tensÃ£o Ã©: ");
  Serial.print((leituraAnalogica/1023)*(float)aRef);
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
  Serial.print("Bateria nÃ£o estÃ¡ carregada!");
  return false;
}

void CarregarBateria(){
  bool carregou = false;
  AcionarRelesPreCarregamento();
  while(carregou == false)
  {
	  carregou = AcompanharCarregamento();
  }
}

void ImprimeNaTela(){
	Serial.print("\n\rBem vindo ao menu de teste!");
	Serial.print("\n\rSegue abaixo um menu de opçôes de teste:");
	Serial.print("\n\r(1) - Testar leitura no sensor de tensão da base:");
	Serial.print("\n\r(2) - Testar leitura no sensor de tensão da bateria:");
	Serial.print("\n\r(3) - Testar lógica do sensor da base:");
	Serial.print("\n\r(4) - Testar lógica do sensor da bateria:");
	Serial.print("\n\r(5) - Sair!");
}

void MenuTeste(){
  bool sair, haTensao = false;
  int caso;	
  while(sair != true){
    if (Serial.available() > 0){
		caso = Serial.read();
		
		switch (caso){
		case 1:
			haTensao = VerificaTensaoDaBase();
		break;
		case 2:
			haTensao = VerificaLeituraAnalogica(SinalReleApm,(float)MAXBATERIA);
		break;
		case 3:
			haTensao = VerificaTensaoDaBase();
			Serial.print("\n\rA lógica é:" + haTensao);
		break;
		case 4:
			haTensao = VerificaLeituraAnalogica(SinalReleApm,(float)MAXBATERIA);
			Serial.print("\n\rA lógica é:" + haTensao);
		break;
		case 5:
			sair = true;
		break;
		}
	}
  }
}

void loop() {
	ImprimeNaTela();
  MenuTeste();
}

