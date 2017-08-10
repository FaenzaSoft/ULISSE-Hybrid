// CONSUMO ZERO ULISSE HYBRID
//
// QUESTO SOFTWARE CONSENTE DI CARICARE LE BATTERIE CON UN PANNELLO FOTOVOLTAICO. LA SERA VENGONO ACCESI I FARETTI. SE
// DURANTE LA NOTTE LE BATTERIE SCENDONO SOTTO I 23,5 VOLT INTERVIENE UN CARICABATTERIE SUPPLEMENTARE, DA RETE
//
// UTILIZZARE ULISSE 4X4 CON INGRESSO PER CREPUSCOLARE ESTERNO.
// IL CARICABATTERIE DEVE ESSERE REGOLATE A 24,5 VOLT STABILIZZATI E VA ALLACCIATO DIRETTAMENTE ALLE BATTERIE DA 24 VOLT TRAMITE UN 
// DIODO DA 6 AMPER CHE CONSENTE ALL'ENERGIA DI FLUIRE SOLO IN DIREZIONE DELLE BATTERIE (CADUTA DI TENSIONE 0,8 VOLT).
//
// IN OGNI CASO IL SOFTWARE STACCA I FARETTI SE LA TENSIONE SCENDE SOTTO I 23 VOLT.
//
//  regolato per FV 36/30 - batterie 24 - fondo scala 50 volt - resistenza di shunt 0,25 ohm 25 watt - display 2x16:
//
// ATTENZIONE: l'alimentatore aggiuntivo da 24,5 volt va allacciato direttamente alle batterie da 24 volt con diodo:
//
// PROGRAMMA PER CARICABATTERIE "ULISSE" con Mosfet IRFP150M da 100 volt 42 Amper oppure 3205 da 55 volt:
// per Nuovenergie Srl - Via Risorgimento, 26 - Faenza
// Autore: Marchi Luigi SoftPlus_Consumo_Zero, mail: luigi.marchi.faenza@gmail.com:
// AVVERTENZA: occorre utilizzare l'hardware specifico dedicato (vedi schema elettrico).
//
//
// AVVERTENZA: per evitare problemi di raffreddamento delle resistenze (4 x 1 ohm 5 watt cadauna) è stato impostato il limitatore a 7 amper:
//
//
// AVVERTENZA: nella centralina Consumo_Zero_ULISSE, con Mosfet, la "massa"(GND) delle batterie non coincide con la massa:
// della centralina e con quella dei pannelli:
//
//
// AVVERTENZA: l'uscita PWM del pin 10 è dedicata al microrelè che gestisce i faretti notturni. Stacca quando la:
// tensione cala del 5% rispetto alla tensione nominale (es: quando i 24 volt diventano 23 volt sotto carico):
// la tensione rimane staccata per almeno 1 ora (gestibile, vedi in fondo al listato).:
//
//
//
// ATTENZIONE, IMPORTANTE:
// Arduino funziona a 5 volt e non ha problemi di sorta, circa il rischio di scariche elettriche.                               //
// Circa l'alimentatore da rete è OBBLIGATORIO utilizzare un caricabatteria da telefonino (a norma) con uscita 15 volt DC.    //
// Per operare sulle parti hardware di contorno occorre avere competenze ed esperienze consolidate. Occorre essere esperti e    //
// consapevoli dei rischi circa gli interventi in apparecchiature con tensioni di rete (230 AC) e/o tensioni elevate di         //
// stringhe di pannelli fotovoltaici. E' pertanto necessario affidarsi a personale qualificato.                                 //
// Il produttore del software declina ogni responsabilità per danni elettrici.                                                  //
// Quanto ai danni derivante da malfunzionamento del software, il produttore del software declina ogni responsabilità circa:    //
// 1) danni derivanti da utilizzo di componentistica di contorno;                                                               //
// 2) danni derivanti da modifica del software originario;                                                                      //
// 3) danni derivanti da scarsa esperienza e capacità professionale del cliente.                                                //
//
//
//
//
// ATTENZIONE: non ci sono trimmer, le regolazioni si fanno via software, occorre selezionare il tipo di pannello ed:
// eventualmente anche il numero cioè uno solo da 60 o 72 celle, (tensione 30 oppure 36 volt) oppure due (solo dove previsto !!!!), in serie:
// che comportano una tensione in ingresso doppia (60 oppure 72 volt) e varia da marca a marca:
// Anche le batterie da caricare vanno specificate via software: possono essere, in serie:
// una (12 volt); due (24 volt); 4 (48 volt, dove previsto !!!):
//
// Occorre quiindi cambiare i parametri, in testa al listato.:
// Dati di default:
// 1) tensione_batterie = 24:
// 2) tensione_pannelli = 30/36:
// 3) limitatore = 7000 milliamper:
// 4) ritardo accensione = 4000 minuti x 100:
// 5) stacco faretti (24 volt) = 2300:
//
// Circa volt Fv e volt batterie si possono utilizzare diverse combinazioni:
//
// Opzione 1:
// int tensione_pannelli = 30;      // un solo pannello da 60 celle:
// int tensione_batterie = 12;      // una batteria da 12 volt:
//
// Opzione 2:
// int tensione_pannelli = 36;      // un solo pannello da 72 celle:
// int tensione_batterie = 12;      // una batteria da 12 volt:
//
// Opzione 3:
int tensione_pannelli = 30;    // un pannello da 60 celle:
int tensione_batterie = 24;    // due batterie da 12 volt in serie:
//
//
// Opzione 4:
// int tensione_pannelli = 36;    // un pannello da 72 celle (W_200):
// int tensione_batterie = 24;    // due batterie da 12 volt, in serie (W_200):
//
//
// Opzione 5:
// int tensione_pannelli = 24;    // da trasormatore di rete:
// int tensione_batterie = 12;    // una batteria da 12 volt:
//
//
// Opzione 6:
// int tensione_pannelli = 19;    // da trasormatore per computer portatili:
// int tensione_batterie = 12;    // una batteria da 12 volt:
//
//
//
// REGOLAZIONE FINDO SCALA:
//
// int fondo_scala = 900;     // per hardware fondo scala 100 volt (W_500 e W_1000):
int fondo_scala = 450;     // per hardware fondo scala 50 volt (W_200):
//
//
//
#include <Arduino.h> //inclusione della libreria // definizione dei pin di INPUT e OUTPUT:
// includere la libreria:
#include <LiquidCrystal.h>
// vedi anche: http://www.maffucci.it/2012/02/17/appunti-su-arduino-pilotare-un-display-lcd/#sthash.Z10I1f17.dpuf
// uso del display LCD 16x2 standard Hitachi HD44780
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//
//
int caricoPin = 9;             // carico analogico PWM pin 9 per Mosfet di potenza:
int caricoPinVentola = 6;      // carico analogico PWM pin 6 per Mosfet di regolazione ventola 12 volt:
int caricoPinRSS = 10;         // carico analogico PWM pin 10 per gestione faretti notturni, tramite microrelè:
int volt_crepPin = A2;         // ingresso analogico A2 sensore volt crepuscolare:
int volt_FVPin = A3;           // ingresso analogico A3 sensore volt FV:
int volt_1ohmPin = A4;         // ingresso analogico A4 sensore dopo le 3 resistenze da 1 ohm:
int volt_batteriePin = A5;     // ingresso analogico A5 sensore volt batterie:

long volt_FV = 0;
long volt_1ohm = 0;
long volt_drain = 0;
long volt_batterie = 0;
long mAmper = 0;
unsigned long coefficiente = 0;
int ciclopwm = 0;
int ciclopwm_ventola = 0;
int ciclopwm_RSS = 0;
int crepuscolare = 0;
int aumento = 1;
int conta1 = 0;
int conta2 = 0;
int attivazione_crep = 500;       // *****************************valore di attivazione crepuscolare, modificabile (da 100 a 1024):
unsigned long conta_crepuscolo = 0;
unsigned long volt_crep = 0;
int ritardo = 10;               // definizione ritardo accensione faretti, 1200  secondi = 20 minuti / 2400 secondi = 40 minuti:
// int conta_crepuscolo = 0;             // definisce il ritardo del crepuscolare (2000 = 18 minuti circa):
unsigned long conta_stacco = 0;
long somma_FV = 0;
long somma_1ohm = 0;
long somma_drain = 0;
unsigned long tempo_vecchio = 0;
unsigned long durata = 0;
int ore = 0;
int minuti = 0;
int secondi = 0;
//
//
//
void setup()
{
  pinMode(caricoPin, OUTPUT);
  pinMode(caricoPinVentola, OUTPUT);
  pinMode(caricoPinRSS, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A2, INPUT);     // ingresso analogico A2 sensore volt crepuscolare:    
  pinMode(A3, INPUT);     // ingresso analogico A3 sensore volt FV:
  pinMode(A4, INPUT);     // ingresso analogico A4 sensore dopo le 4 resistenze da 1 ohm:
  pinMode(A5, INPUT);     // ingresso analogico A5 sensore volt batterie:
  //
}

void loop()
{
  //
  // ************************************** step 1 verifica tensioni:
  //
  somma_FV = 0;
  somma_1ohm = 0;
  somma_drain = 0;
  //
  for (int cicli = 0; cicli < 100; cicli++)
  {
    volt_FV = analogRead(A3);
    somma_FV = (somma_FV + volt_FV);
    //
    volt_1ohm = analogRead(A4);
    somma_1ohm = (somma_1ohm + volt_1ohm);
    //
    volt_drain = analogRead(A5);
    somma_drain = (somma_drain + volt_drain);
    //
    delay(2);
  }
  //
  //
  // ***************************************** step 2 rettifica i dati delle tensioni:
  //
  volt_FV = (somma_FV / 100);  // perchè è stata fatta la somma di 100 cicli:
  volt_1ohm = (somma_1ohm / 100);  // perchè è stata fatta la somma di 100 cicli:
  volt_drain = (somma_drain / 100);  // perchè è stata fatta la somma di 100 cicli:
  volt_FV = (volt_FV * fondo_scala / 100);
  volt_1ohm = (volt_1ohm * fondo_scala / 100);
  volt_drain = (volt_drain * fondo_scala / 100);
  //
  //
  if (volt_FV > volt_1ohm)
  {
    // utilizzare la versione 40/2 per l'hardware con resistenze da 0,5 ohm e quella 80/2 per resistenze da 0,25 ohm:
    //    mAmper = (volt_FV - volt_1ohm) * (20 / 2);     //perchè ci sono 4 resistenze da 1 ohm serie/parallelo, cioè 1ohm di carico:
    //    mAmper = (volt_FV - volt_1ohm) * (40 / 2);     //perchè ci sono 8 resistenze da 1 ohm serie/parallelo, cioè 0,5 ohm di carico:
    mAmper = (volt_FV - volt_1ohm) * (80 / 2);     //perchè abbiamo una resistenza da 0,25 ohm:
  }
  else
  {
    mAmper = 0;
  }
  // modifica per l'uscita del Mosfet sul DRAIN/collettore:

  //
  //  if (crepuscolare == 1) volt_batterie = volt_batterie + 120;         // quando è notte la tensione va rettificata:
  //  else volt_batterie = (volt_1ohm - volt_drain);
  //   if (volt_drain < 0) volt_drain = 70;              // rettifica perchè con crepuscolare la tensione si inverte sul Mosfet:
  //   volt_batterie = (volt_1ohm - volt_drain);
  if (volt_drain < 50)  volt_batterie = volt_1ohm + 100;    // rettifica perchè con crepuscolare la tensione si inverte sul Mosfet:
  else volt_batterie = (volt_1ohm - volt_drain);
  //
  //
  ciclopwm = ciclopwm + aumento;
  //


  // step 4 controlla e modifica i cicli PWM:
  //
  //  if (ciclopwm > 200 || volt_batterie > 1370)
  aumento = 1;
  //
  //
  if (tensione_batterie == 12)
  {
    if (volt_batterie > 1400)
    {
      ciclopwm = ciclopwm - (aumento + 3);
    }
    if (volt_batterie < 1200)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
  }
  //
  //
  if (tensione_batterie == 24)
  {
    //    if (volt_batterie > 2900)
    if (volt_batterie > 2750)
    {
      ciclopwm = ciclopwm - (aumento + 3);
    }
    if (volt_batterie < 2400)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
  }
  //
  //
  //
  //
  //
  if (tensione_pannelli == 15)         // tensione da alimentatori strisce led da 12 volt dimmerabili fino a 15 volt:
  {
    if (volt_FV > 1700)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
    if (volt_FV < 1300)
    {
      ciclopwm = ciclopwm - (aumento + 2);
    }
  }
  //
  //
  if (tensione_pannelli == 19)         // tensione da alimentatori per computer portatili:
  {
    if (volt_FV > 2200)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
    if (volt_FV < 1800)
    {
      ciclopwm = ciclopwm - (aumento + 2);
    }
  }
  //
  //
  if (tensione_pannelli == 24)         // tensione da trasformatore:
  {
    if (volt_FV > 2800)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
    if (volt_FV < 2050)
    {
      ciclopwm = ciclopwm - (aumento + 2);
    }
  }
  //
  //
  // un pannello da 60 celle, si fa lavorare tra i 26 e i 34 volt:
  if (tensione_pannelli == 30)
  {
    if (volt_FV > 3400)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
    if (volt_FV < 2600)                       // mettere a 26/28, salvo casi particolari;
    {
      ciclopwm = ciclopwm - (aumento + 2);
    }
  }
  //
  //
  // un pannello da 72 celle, si fa lavorare tra i 31 e i 40 volt:
  if (tensione_pannelli == 36)
  {
    if (volt_FV > 3600)
    {
      ciclopwm = ciclopwm + (aumento + 1);
    }
    if (volt_FV < 2800)
    {
      ciclopwm = ciclopwm - (aumento + 2);
    }
  }
  //
  //
  //
  // step 5 attiva il limitatore generale di corrente:
  //   ********************************************************* DATO MODIFICABILE ***********************************************:
  // if (mAmper > 7000)   // limitatorte a 7 amper:            // ******** dato modificabile:
  if (mAmper > 3500)    // limitatore a 3,5 amper:
  {
    ciclopwm = ciclopwm - (aumento + 3);
  }
  //
  //
  //
  // ******************step 3 attivazione crepuscolare esterno, basato sulle differenze di luminosità:
  // con luminosità modesta attiva i faretti, viceversa li spegne, cioè stacca il microrelè:
  //
  volt_crep = analogRead(A2);       // esprime un numero compreso tra 0 e 1024, corrispondente a tensione 0 e tensione 5 volt:
  //
  //
  if (volt_crep > attivazione_crep)        // attivazione crepuscolare esterno:
  {
    conta_crepuscolo = conta_crepuscolo + 1;
    //    analogWrite (caricoPin, ciclopwm);
  }
  else
  {
    conta_crepuscolo = 0;
    analogWrite (caricoPinRSS, 1);
    crepuscolare = 0;
  }
  // ****************************************************************** DATO MODIFICABILE **********************************************:
  if (conta_crepuscolo > ritardo)                // dipende dal ritardo impostato in cima al programma, in secondi:
  {
    analogWrite (caricoPinRSS, 254);
    crepuscolare = 1;
    ciclopwm = 0;    
  }
  //
  //
  //    
  //
  if (ciclopwm > 254) ciclopwm = 254;
  //  if (ciclopwm < 1) ciclopwm = 1;
  if (ciclopwm < 0) ciclopwm = 0;
  //***************************************** SISTEMA DUPLEX PWM *****************************************************************:
  //
  int stop_PWM = (12500/ciclopwm);             // interrompe il ciclo per qualche millisecondo quando il dutycycle è debole:
  if (stop_PWM > 500) stop_PWM = 500;
  if (stop_PWM < 0) stop_PWM = 1;  
  analogWrite (caricoPin, 0);
  delay (stop_PWM);
  analogWrite (caricoPin, ciclopwm);
  //
  //*****************************************FINE SISTEMA DUPLEX PWM ************************************************************:
  //
  //
  conta1 = conta1 + 1;
  conta2 = conta2 + 1;
  //
  //
  // step 5 visualizza i dati di cicloPWM (in percentuale) e la carica, in milliamper:
  if (conta1 > 4)
  {
    lcd.begin(16, 2);
    // Visualizzo il messaggio sul display
    //
    if (crepuscolare == 0)                      // il crepuscolare non è attivo:
    {
      tempo_vecchio = millis() / 1000;       
    }
    if (crepuscolare == 1)                      // il crepuscolare è attivo e visualizzo il messaggio sul display:
    {
      durata = millis()/1000 - tempo_vecchio;      
      lcd.begin(16, 2);        
      if (analogRead(caricoPinRSS) < 100)       // significa che il relè non è ancora stato attivato:
      {
        lcd.print("hh-mm-sec Crepus");         
        lcd.setCursor(0, 1);
        lcd.print("  :  :      ON  "); 
      }
      else
      {
        lcd.print("hh-mm-sec   LED ");           // significa che il relè è stato acceso e alimenta i faretti LED:
        lcd.setCursor(0, 1);
        lcd.print("  :  :      ON  ");        
      }  
      if (durata >= 3600) 
      {
        ore = durata/3600;
        durata = durata - (ore * 3600);
      }
      if (durata < 3600) 
      {
        minuti = durata/60;
        durata = durata - (minuti * 60);
      }      
      if (durata < 60) secondi = durata;
      lcd.setCursor(0, 1);         
      lcd.print(ore);   
      lcd.setCursor(3, 1);         
      lcd.print(minuti);        
      lcd.setCursor(6, 1);         
      lcd.print(secondi);   
      lcd.setCursor(10, 1);         
    }
    else
    {
      lcd.print("%CicloPWM mAmper");

      lcd.setCursor(0, 1);
      lcd.print("               ");
      lcd.setCursor(1, 1);
      lcd.print(ciclopwm * 100 / 254);
      lcd.setCursor(10, 1);
      // mAmper = mAmper - 0; // regolazione fine sensibilità mAmper:
      lcd.print(mAmper);
    }
    conta1 = 0;
    //
    // attiva la ventola (dove prevista) oltre 1 amper di carica:
    //    if (mAmper > 1000) analogWrite (caricoPinVentola, 254);
    //    else analogWrite (caricoPinVentola, 0);
    //    delay (500);
  }
  //
  //
  // step 6 visualizza i dati delle tensioni in ingresso e sulle batterie:
  if (conta2 > 9)
  {
    lcd.begin(16, 2);
    // Visualizzo il messaggio sul display
    //  lcd.print("CicloPWM mAmper");
    lcd.print("VoltFV  VoltBatt");
    lcd.setCursor(0, 1);
    lcd.print("               ");
    //    lcd.setCursor(1, 1);
    //    lcd.print(volt_FV);
    //    lcd.setCursor(11, 1);
    //    lcd.print(volt_batterie);
    lcd.setCursor(2, 1);
    lcd.print(volt_FV);
    lcd.setCursor(3, 1);
    lcd.print(",");
    lcd.setCursor(1, 1);    
    lcd.print(volt_FV/100); 
    lcd.setCursor(12, 1);
    lcd.print(volt_batterie);    
    lcd.setCursor(13, 1);
    lcd.print(",");    
    lcd.setCursor(11, 1);  
    lcd.print(volt_batterie/100);      
    conta2 = 0;
    delay(200);
  }
  //
  //
  //
  // step 8 stoppa microrelè per non scaricare del tutto le batterie:
  // *************************************************************** DATO MODIFICABILE ***********************************************:
  if (volt_batterie < 2300)                                                  // ********* dato modificabile:
    //            if (volt_batterie < 1150)        // con batteria da 12 volt:
  {
    conta_stacco = conta_stacco + 1;

    if (conta_stacco > 50)               // stacca con ritardo per evitare lo sbalzo quando si accendono i faretti:
    {
      crepuscolare = 0;                      // stop faretti a 11,5 volt per batteria:
      analogWrite (caricoPinRSS, 1);
      lcd.begin(16, 2);
      lcd.print("Sospensione LED " );
      lcd.setCursor(0, 1);
      lcd.print("               " );
      lcd.setCursor(0, 1);
      lcd.print("Volt Bat:");
      lcd.setCursor(12, 1);
      lcd.print(volt_batterie);
      lcd.setCursor(13, 1);  
      lcd.print(",");  
      lcd.setCursor(11, 1); 
      lcd.print(volt_batterie/100);      
      delay (2400000);     // 40 minuti di stop per evitare accensioni e spegnimenti continui:
      conta_stacco = 0;
    }
  }
  //
  //
}
// FINE "ULISSE" Mosfet per Nuovenergie Srl:





















