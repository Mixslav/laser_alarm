// Program za kućni alarm 2019. seminarski rad
// Mislav Stipić
// program radi pomoću arduino uno ploče
// (laser, senzor, zvučnik)

  /*
 *      MENI
 * ASCII kod za 1 = 49, tihi način alarma
                2 = 50, glasni način alarma
                3 = 51, reset alarma
                4 = 52, upali alarm
                5 = 53  ugasi alarm
*/


const int laserPin = 47;         // pin za laser (smeđa)
const int zvucnikPin = 46;       // pin za zvucnik (plava)
const int ldrPin = A15;          // pin za ldr (žuta)

int potvrda = 0;                 // potvrda dali se alarm aktivirao ili konstantno provjerava dali je netko prešao preko lasera ili je ugašen (0 - u radu, 1 - ugašen, 2 - alarm se aktivirao)
int var = 49;                    // pomoćna varijabla za unos i razvrstavanje unosa (početna vrijednost je 1 = 49 tj tihi način alarma)
int var1 = 49;                   // pomoćna varijabla za prvi dio menija (početna vrijednost je 1 = 49 tj. tihi način alarma)
int var2 = 52;                   // pomoćna varijabla za drugi dio menija (početna vrijednost je 4 = 52 tj. upali allarm)
       
float napon; // varijabla za mjerenje napona na fotootporniku



// setup funkcija, nužna za rad programa, kod unutar ove funkcije se izvršava samo jedan put
void setup() {
  
  Serial.begin(9600);               // frekvencija komunikacije arduina i računala (9600 bita po sek.)

  pinMode(zvucnikPin, OUTPUT);      // postavi pin za zvučnik kao izlaz
  pinMode(laserPin, OUTPUT);        // postavi pin za laser kao izlaz


  /*
 *      MENI
 * ASCII kod za 1 = 49, tihi način alarma
                2 = 50, glasni  
                3 = 51, reset
                4 = 52, upali
                5 = 53  ugasi
*/

  meni();                           // printaj meni samo jedan put

    digitalWrite(laserPin,HIGH);    // upali laser
    delay(100);
    
}



//////////////////////////// početak glavne petlje /////////////////////////////
void loop() {



  while(potvrda == 0){   // dok alarm "sluša" (potvrda je 0) čekaj unos i provjeri dali je netko aktivirao alarm
  
    unos();                // funkcija koja gleda unos korisnika dok alarm "sluša"
    provjeriAlarm();       // provjeri dali se alarm aktivirao

  }

  unos();               // funkcija koja gleda unos korisnika (nalazi se izvan while petlje te idalje prati što će korisnik unijeti)


  if(potvrda == 2){   // provjeri dali se alarm aktivirao
  
    alarm_se_aktivirao();  // funkcija za aktivaciju alarma (alarm se aktivira u tihom ili glasnom načinu rada)
}

  if(var1 == 51){          // reset alarma, 3 = 51, reset ako smo unijeli 3
    
    reset_alarma();        // funkcija za resetiranje alarma
}


upali_ugasi_alarm();       // provjeri ulaz i vidi dali je alarm upaljen ili ugašen, ako je ugašen alarm ne radi ništa


}
////////////////////////////// kraj glavne petlje //////////////////////////////////////






////////////////////  FUNKCIJE  /////////////////////////////////////////////////////////



void reset_alarma(){        // funkcija za resetiranje alarma

    meni();                 //ponovo printaj meni
    Serial.println("Alarm je resetiran (laser je upaljen, mod alarma je postavljen na [ tihi ] način");     // printaj tekst unutar navodnika pa idi u novi red
    Serial.println();                                       // novi red
    Serial.println();                                       // novi red

    potvrda = 0;   // potvrda je 0 za vraćanje u while petlju u glavnoj petlji programa tj. ponovno gledaj dali je netko prešao preko lasera
    var1 = 49;     // 1 = 49 za vraćanje na tihi način alarma
    var2 = 52;     // 4 = 52 za ponovno paljenje alarma
    
}




void upali_ugasi_alarm(){     // funkcija za paljenje ili gašenje alarma na temelju našeg unosa (tj lasera i alarma)

  if(var2 == 52){    // unosimo broj 4 = 52 za upaliti alarm
  
    digitalWrite(laserPin,HIGH);  // alarm je upaljen
  }

  if(var2 == 53){    // unosimo broj 5 = 53 za ugasiti alarm
  
     digitalWrite(laserPin,LOW);  // isključi alarm
  }
  
}



void alarm_se_aktivirao(){      // funkcija za aktivaciju alarma (alarm se aktivira u tihom ili glasnom načinu rada)
  
  if(var1 == 49){  // provjeravamo unos broja 1 za tihi način alarma
    Serial.println("Upozorenje, alarm se aktivirao!!!");    // printaj tekst unutar navodnika pa idi u novi red
    Serial.println();                                       // novi red
    Serial.println();                                       // novi red
    
    }

   if(var1 == 50){  // provjeravamo unos broja 2 = 50 za glasni način alarma  
    Serial.println("Upozorenje, alarm se aktivirao!!!");    // printaj tekst unutar navodnika pa idi u novi red
    Serial.println();                                       // novi red
    Serial.println();                                       // novi red
    zvucnik();                                              // sirena upaljena
    
    }
}




void unos(){      // funkcija za unos odabira iz menija


var = Serial.read();   //unosimo brojeve sa serijskog monitora

/////////////////////////////////// razvrstajemo unos /////////////////
/*
 * 
 * ASCII kod za 1 = 49, Za tihi način alarma
                2 = 50, Za glasni način alarma
                3 = 51, Za reset alarma
                4 = 52, Za upaliti alarm
                5 = 53  Za ugasiti alarm
*/

if(var <= 51 && var >=49){    // ako je uneseni broj između 1 i 3 izvrši:
var1 = var;                   // pridjeli uneseni broj pomoćnoj varijabli var1 za prvi dio menija

}

if(var >= 52 && var <= 53)    // ako je uneseni broj između 4 i 5 izvrši:
var2 = var;                   // pridjeli uneseni broj pomoćnoj varijabli var2 za drugi dio menija


/*
if(var > 53 || var < 49){                                         // ako nismo unijeli broj između 1 i 5 izvrši
  Serial.println("Unijeli ste krivu naredbu, ponovite unos:");    // printanje da je unesen krivi unos i prijeđi u novi red
  Serial.println();                                               // novi red
  }
  */
}



void provjeriAlarm(){   // funkcija koja provjerava dali je netko aktivirao alarm
  
  napon = analogRead(ldrPin) * (5.0 / 1023.0);      // čitaj napon sa LDR otpornika
  def_Aktiv();                                        // izvrši funkciju "def_Aktiv()"

}



void def_Aktiv(){     // funkcija za aktivnost alarma tj. funkcija za praćenje dali je alarm aktiviran ili gašenje alarma
  
  if(napon < 3.7)   // ovi brojevi su osjetljivost alarma (0.0 - 5.0)
    potvrda = 2;      //aktiviraj alarm na fotootporniku ako je napon pao ispod 3.7 tj ako je netko prešao preko lasera
  
  if(var2 == 53)      // odabir: 5 = 53  ugasi alarm
    potvrda = 1;      // ako nema lasera onda je alarm isključen
 
}



void zvucnik(){     // funkcija za zvučni alarm tj zvučnik

  tone(zvucnikPin, 1000);     // upali zvučnik, postavi frekvenciju tona na 1000 Hz
  delay(500);                 // razmak između uključenog tona i ugašenog tona (500 ms)
  noTone(zvucnikPin);         // isključi ton
  delay(500);                 // razmak između uključenog tona i ugašenog tona (500 ms)
  
}

  

void meni(){      // meni za serijski monitor
  
 Serial.print("Unesite brojeve po meniju za odabir: ");          // printaj tekst unutar navodnika pa idi u novi red
 Serial.println();  // novi red
 Serial.println();  // novi red

 Serial.println("Broj 1.) Za tihi način alarma");  // printaj tekst unutar navodnika pa idi u novi red
 Serial.println("Broj 2.) Za glasni način alarma");
 Serial.println("Broj 3.) Za reset alarma");
 Serial.println("Broj 4.) Za ponovno paljenje alarma");
 Serial.println("Broj 5.) Za ugasiti alarm");
 
 Serial.println();  // novi red
 Serial.println();  // novi red

}
