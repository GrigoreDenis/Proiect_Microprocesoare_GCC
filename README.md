# Proiect_Microprocesoare_GCC

Echipa GCC : Grigore Denis, Casandroiu Paul, Chinda Andrei
    Nr. crt Senzor Tip
        1 Sound Analogic
        2 Ambient Analogic
        3 RGB LED Integrat

Obiectiv: Utilizarea led-ului RGB integrat pentru afis,area culorii asociate intervalului de valori
recept, ionate de la cei doi senzori.

Caracteristica individuala:
    • Un buton grafic pentru select, ia senzorului pentru care se afis,eaz ̆a valoarea pe combinat, ia RGB,
    e.g., senzor 1 sau senzor 2.
    • Grafic pentru valorile senzorilor de intrare.
    
    
    
    Preluarea datelor de la senzorii asignat, i temei de proiect, fie prin intermediul perifericului ADC in
cazul senzorilor analogici, fie prin intermediul perifericului GPIO ˆın cazul senzorilor digitali.

    • Transmiterea datelor obt, inute de senzori de la platforma de dezvoltare, pe portul serial c ̆atre PC,
cu ajutorul perifericului UART.
    • Realizarea unei interfet,e grafice cu ajutorul limbajului Python si a unui framework specific precum
PyQT care s ̆a prezinte un grafic (barplot) in timp real pentru fiecare senzor de input asociat temei
de proiect. Un grafic va afis,a valorile in 3 zone de culori, verde pentru valori mici, galben pentru
valori medii s, i ros,u pentru valori mari. Cele doua praguri, i.e., valori mici-medii, valori medii-mari,
vor fi stabilite dinamic interpoland in 3 zone spectrul de valori generate de senzorul respectiv.
    • Realizarea unei documentat, ii de proiect in format .docx care sa contina urmatoarele sectiuni:
    
– Scopul proiectului - descriere a contextului practic in care ar putea fi folosit.
– Configurare - enumerarea perifericelor care au fost utilizate s, i modul lor de configurare (la
nivel de registrii), mod de conectare al senzorilor si motivarea acestuia prin schematics.
– Reprezentare grafic ̆a a modului de conectare al senzorilor s, i componentelor electronice pe un
breadboard virtual folosind platforma web Tinkercad.
– Setup - insiruirea pasilor care trebuie urmariti pentru rularea proiectului insotiti de printscreen-
uri.
– Probleme ıntampinate - modul de rezolvare al problemelor ıntampinate si sugestii de ımbunatatire
a proiectului rezultat.
• Crearea unui repository pe platforma Github unde va fi salvat codul proiectului (headere si fisiere
sursa) impreuna cu un fisier README.md care sa descrie pasii de initializare si setup.
