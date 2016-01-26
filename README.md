## Indeling

    -Code
	->De meest recente versie van de volledige code komt hierin te staan
	->De huidige versie is het hoogste versie nummer
    -Documenten
	->Bevat projectverslagen
    -Modellen
	->Bevat alle modellen voor dit project
    -RaspberryPi
	->Bevat bestanden die naar de RaspberryPi geupload moeten worden/of gedownload moeten worden 
    -Teamcontract
	->Bevat het teamcontract
    -Urenverantwoording
	->Bevat het urenverantwoordingsdocument
    -Vergaderingen
	->Bevat alle vergader agenda's en de bijbehorende notule
    -Visual Studio
	->Bevat het visual studio project, hierin staat niet gegarandeerd de nieuwste code

## Code maken

In de map code staat een sub-map met een versie nummer, selecteer de juiste versie. In deze folder staat de makefile 

De volgende commando's zijn beschikbaar

Compileer alle code en link deze code : make 
Compileer alle code en link deze en voer het het programma uit: make run
Alle tussen bestanden verwijderen: make clean




## Documentatie generen

In de map code staat een sub-map met een versie nummer, selecteer de juiste versie. In deze folder staat de Doxyfile en de makefile.

De volgende commando's zijn beschikbaar

Maak alle documentatie: make doc



## Alle gemaakte bestande verwijderen

Om alle bestanden die generereerd zijn door de makefile en/of de Doxyfile kan het volgende commando uitgevoerd worden

Verwijder alle .o .d files en de /Doc folder en programma: make clean   


