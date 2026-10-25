# g++ Notizen (PG1, Praktikum 1)

## Der Weg vom Text zum Programm

```
main.cpp  --(Compiler, -c)-->  main.o  --(Linker)-->  main
Source Code                    Object File            Executable
(Text)                         (Maschinencode)        (ausführbar)
```

## Aufbau eines Aufrufs

```
g++  [Optionen]  Eingabedatei  -o Ausgabedatei
```

## Optionen

| Option | Bedeutung |
|---|---|
| `g++` | C++-Compiler der GCC (GNU Compiler Collection) |
| `-std=c++23` | Sprachstandard C++23 (nötig für `<print>` / `println`) |
| `-c` | Nur compilieren, **nicht** linken -> Ergebnis ist eine `.o`-Datei |
| `-o name` | Name der entstehenden Datei (nie gleich der Quelldatei!) |
| `-Wall` | "Warnings all": schaltet viele Warnungen ein |

## Die wichtigsten Befehle

```bash
# Compilieren (Ergebnis: main.o)
g++ -std=c++23 -c main.cpp -o main.o

# Mit Warnungen
g++ -std=c++23 -c -Wall main.cpp -o main.o

# Linken (Ergebnis: Executable main)
g++ main.o -o main

# Starten
./main
```

Mehrere Dateien: jede `.cpp` einzeln mit `-c` compilieren, dann alle `.o` zusammen linken:

```bash
g++ main5.o func1.o func2.o -o main5
```

## Mehrere Dateien: Header (.h) und Source (.cpp)

Eine Funktion kann in einer **anderen Datei** stehen als der Code, der sie aufruft. Jede `.cpp` wird einzeln zu einer `.o` compiliert, der Linker verbindet alle am Ende:

```
main5.cpp  --compilen-->  main5.o  ─┐
func1.cpp  --compilen-->  func1.o  ─┼--linken-->  main5 (Programm)
func2.cpp  --compilen-->  func2.o  ─┘
```

| Begriff | Wo | Bedeutung |
|---|---|---|
| **Deklaration** | in `func1.h` | "Es gibt eine Funktion mit diesem Namen, diesen Parametern, diesem Rückgabetyp." |
| **Definition** | in `func1.cpp` | Der eigentliche Code der Funktion |

- Analogie: Deklaration = Eintrag im **Inhaltsverzeichnis**, Definition = das **Kapitel** selbst
- `#include "func1.h"` in `main5.cpp` sagt dem Compiler, wie die Funktion aufgerufen werden darf
- Der Compiler lässt an der Aufrufstelle eine Lücke, der **Linker** füllt sie mit dem Code aus `func1.o`
- Fehlt eine `.o`: `ld: cannot find func1.o` (Linker findet sie nicht -> nicht compiliert)
- Nur `func1.cpp` geändert -> nur `func1.o` neu erzeugen und neu linken (genau das automatisiert `make`)
- Nur `.cpp`-Dateien werden compiliert, `.h`-Dateien nie direkt
- Compiler übersetzt, Linker verbindet: zwei getrennte Aufgaben

## Fehlermeldungen lesen

```
main2.cpp:10:17: error: expected ';' before 'println'
```

- `main2.cpp:10:17` = Datei, Zeile 10, Spalte 17
- `error:` = Art der Meldung (error stoppt, warning nicht)
- Rest = was der Compiler erwartet hat
- Immer die **erste** Meldung zuerst lesen (spätere sind oft Folgefehler)
- Der echte Fehler steht oft in der Zeile **davor** (z. B. fehlendes `;`)

## Meine Fehler und was ich daraus lerne

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| `unrecognized command-line option '-c++23'` | Option doppelt / falsch getippt | Richtig ist `-std=c++23`. Pfeil-hoch holt den letzten Befehl zurück |
| Programm lief, aber nichts ausgegeben | Nur `-c` gemacht, nicht gelinkt | `.o` ist nicht ausführbar. Erst linken, dann `./main` |
| `input file is the same as output file` | `-o main4.cpp` statt `-o main4.o` | Hinter `-o` steht der **neue** Name. Nie die Quelldatei |
| Altes Verhalten nach Änderung | Neu compiliert nicht geklappt, alte `.o` gelinkt | Nach jeder Änderung: neu compilen **und** neu linken |
| `syntax error near unexpected token 'newline'` | `<...>` aus einem Muster mitgetippt | `<` ist in der Shell eine Umleitung. Platzhalter ohne Klammern schreiben |
| `expected ';'` | Semikolon fehlt | Zeile des Fehlers **und** die davor prüfen |
| `else` im `if`-Block | `}` nach dem `if`-Block fehlte | `if { } else { }`: jeder Block hat eigene Klammern |
| Summe war falsch / fehlte | `int sum;` ohne Startwert, `println` ohne `{}` und Argument | Variablen immer initialisieren: `int sum = 0;` |
| `Permission denied` bei `pg1-public/.../main5.cpp` | Am Zeilenanfang fehlte `cp` | Das erste Wort einer Zeile ist der **Befehl**. Muster: `cp Quelle(n) Ziel` |
| `ld: cannot find func1.o` | `func1.cpp` und `func2.cpp` nicht compiliert | Jede `.cpp` einzeln mit `-c` compilieren, erst dann linken |
| `-o main5.o` beim Linken | Ausgabename vom Compilieren übernommen | Compilieren -> `.o`, Linken -> Programmname ohne Endung |

## Pfade im Terminal

- `pwd` zeigt, wo ich gerade bin
- `ls` zeigt den Inhalt
- `..` = einen Ordner hoch, `~` = Home-Verzeichnis (`/home/can`)
- Pfade gelten immer von dem Ordner aus, in dem ich stehe
- Tab-Taste vervollständigt Namen und verhindert Tippfehler
- `cp` kopiert, `mv` verschiebt
