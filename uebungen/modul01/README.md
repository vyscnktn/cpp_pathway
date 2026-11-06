# Modul 01 – Datentypen: Warum ist `7 / 2` gleich `3`?

## Problem

Der Durchschnitt von zwei Noten (7 und 2) soll 4,5 ergeben. In C++ liefert der naheliegende Code aber `4`:

```cpp
int a = 7;
int b = 2;
println("Durchschnitt: {}", (a + b) / 2);   // 4, nicht 4.5
println("7 / 2 = {}", a / b);              // 3, nicht 3.5
```

Es gibt keinen Compilerfehler und keine Warnung. Das Programm läuft und rechnet trotzdem "falsch".

## Ziel

Verstehen, dass in C++ jeder Wert einen **Typ** hat und dass der Typ bestimmt, wie gerechnet wird.

## Aufgaben

- [x] Programm mit `int`-Division schreiben, Ergebnis vorher schätzen, dann ausführen (`main.cpp`)
- [x] Dasselbe in Python ausprobieren und `/` mit `//` vergleichen (`main.py`)
- [x] Die Falle `double ortalama = (a + b) / 2;` untersuchen: Warum ist das Ergebnis trotz `double` gleich `4`?
- [x] Drei Varianten nebeneinander vergleichen (`main1.cpp`)

## Konzepte

### Datentypen `int` und `double`

| Typ | Speichert | Größe | Beispiel |
|---|---|---|---|
| `int` | ganze Zahlen | meist 4 Byte | `7`, `-3` |
| `double` | Kommazahlen | 8 Byte | `3.5`, `0.01` |
| `float` | Kommazahlen (ungenauer) | 4 Byte | `3.5f` |

`double` hat etwa 15–16 signifikante Stellen, `float` nur etwa 7.

### Regel: Der Typ des Ergebnisses hängt von den Operanden ab

```
int    / int    → int      (7 / 2   = 3,   Nachkommastellen werden abgeschnitten)
double / int    → double   (7.0 / 2 = 3.5)
int    / double → double   (7 / 2.0 = 3.5)
```

Sind beide Seiten ganze Zahlen, ist das Ergebnis eine ganze Zahl. Der Rest wird nicht gerundet, sondern **weggeworfen**. Den Rest bekommt man separat mit `%`: `7 % 2 = 1`.

### Reihenfolge der Auswertung

```cpp
double ortalama = (a + b) / 2;
```

1. Zuerst wird die rechte Seite berechnet: `9 / 2` ist `int / int`, also `4`. Die `0,5` ist hier schon verloren.
2. Danach wird `4` in den `double`-Kasten gelegt und zu `4.0` umgewandelt.

Der `double`-Kasten kommt **zu spät**. Bei der Division gab es nur `int`-Werte.

Analogie: Eine Prüfung wird mit 4,5 bewertet, aber das Notensystem kennt nur ganze Zahlen, also steht 4 im Zeugnis. Wer das Zeugnis später an eine Schule mit Kommanoten bringt, bekommt dort 4,0 eingetragen. Die verlorene 0,5 kommt nicht zurück.

### Implizite Umwandlung

| Umwandlung | Informationsverlust | Verhalten |
|---|---|---|
| `int` → `double` (4 → 4.0) | nein | still, ohne Warnung |
| `double` → `int` (4.5 → 4) | ja | erlaubt, Warnung höchstens mit `-Wconversion` |

Mit geschweiften Klammern verbietet C++ verlustbehaftete Umwandlungen:

```cpp
int x = 3.5;   // kompiliert, x = 3
int y{3.5};    // FEHLER: narrowing conversion
```

### Lösungen: vor der Division nach `double` wechseln

```cpp
double o1 = (a + b) / 2.0;                    // Konstante als double
double o2 = static_cast<double>(a + b) / 2;   // vor der Division umwandeln
double o3 = static_cast<double>((a + b) / 2); // FALLE: Umwandlung erst nach der Division → 4
```

`static_cast<double>(x)` bedeutet "verwende diesen Wert als `double`". In Python entspricht das `float(x)`.

## Vergleich mit Python

| Gewünscht | Python | C++ |
|---|---|---|
| 3.5 | `7 / 2` | `7.0 / 2` (mindestens eine Seite `double`) |
| 3 | `7 // 2` | `7 / 2` (beide Seiten `int`) |
| Rest 1 | `7 % 2` | `7 % 2` |
| Umwandeln | `float(a)` | `static_cast<double>(a)` |

Detail: Bei negativen Zahlen unterscheiden sie sich. Python `-7 // 2` ergibt `-4` (rundet ab), C++ `-7 / 2` ergibt `-3` (schneidet Richtung 0 ab). Das C++-`/` ist also nicht genau Pythons `//`.

In Python hat eine **Variable** keinen Typ, nur der **Wert**. In C++ hat die Variable einen festen Typ, und Werte werden beim Hineinlegen angepasst. Genau diese stille Anpassung macht die Falle möglich.

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| `(a + b) / 2` ergibt `4` | `int / int` | Mindestens eine Seite muss `double` sein |
| `double` als Ziel half nicht | Division passiert vor der Zuweisung | Umwandlung muss **vor** der Division geschehen |
| `(a + b) / 2.0` als `/ 2` abgetippt | ein `.0` vergessen | `2` und `2.0` sind verschiedene Typen |
| `a + b` als `a / b` abgetippt | ein Zeichen vertauscht | Kleine Tippfehler ändern das Ergebnis, nicht die Kompilierbarkeit |
| `o3` für richtig gehalten | Programm lief ohne Fehler | Syntaktisch korrekt heißt nicht automatisch richtig |
| `println("...", x, '\n')` | `println` macht selbst einen Zeilenumbruch | Überflüssige Argumente werden ignoriert |

## Merksätze

- Jeder Wert in C++ hat einen Typ, und der Typ bestimmt die Rechenart.
- `int / int` schneidet ab, es rundet nicht.
- Der Wechsel zu `double` muss vor der Division passieren.
- `println` lässt bei `4.0` das `.0` weg. Mit `{:.1f}` sieht man es.

## Verbindung zu micrograd

`Value` speichert `double data` und `double grad`. Gewichte, Gradienten und Lernrate sind Werte wie 0,03. Als `int` würden alle Gradienten auf 0 abgeschnitten, und das Netz könnte nichts lernen.

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | erstes Experiment mit `int`-Division |
| `main.py` | Python-Vergleich `/` und `//` |
| `main1.cpp` | die drei Varianten `o1`, `o2`, `o3` |

Kompilieren:

```bash
g++ -std=c++23 -Wall main1.cpp -o main1
./main1
```
