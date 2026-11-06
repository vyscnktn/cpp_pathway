# Modul 04 – Funktionen: Code nicht kopieren

## Problem

Eine Schicht hat 3 Neuronen. Alle rechnen dasselbe, nur mit anderen Gewichten:

```cpp
// Neuron 1
double sum1 = 0.0;
for (int i = 0; i < n; ++i) { sum1 += (0.01 * i) * 1.0; }
double out1 = relu(sum1);

// Neuron 2
double sum2 = 0.0;
for (int i = 0; i < n; ++i) { sum2 += (0.02 * i) * 1.0; }
double out2 = relu(sum2);

// Neuron 3 ...
```

Dieselben 4 Zeilen wurden dreimal kopiert. Findet man einen Fehler (zum Beispiel `<=` in der Schleife), muss man ihn an drei Stellen korrigieren. Bei 100 Neuronen an 100 Stellen.

## Ziel

Wiederholten Code in eine Funktion auslagern, das Veränderliche als Parameter übergeben, und verstehen, warum die Reihenfolge der Definitionen in C++ wichtig ist.

## Aufgaben

- [x] Die Neuronenberechnung als Funktion `neuron(scale, n)` schreiben und für 3 Neuronen aufrufen
- [x] Erklären, warum das dritte Neuron (negativer Skalierungsfaktor) 0 liefert
- [x] Die Funktionen unter `main` verschieben: Was sagt der Compiler?
- [x] Das Problem lösen, ohne die Funktionen zurückzuschieben (Deklaration)

## Konzepte

### Funktion definieren

```cpp
double neuron(double scale, int n)
{
    double s = 0.0;
    for (int i = 0; i < n; ++i)
    {
        s += (scale * i) * 1.0;
    }
    return relu(s);
}
```

Der Kopf `double neuron(double scale, int n)` entspricht Pythons `def neuron(scale, n):`, enthält aber drei zusätzliche Angaben:

| Teil | Bedeutung | in Python |
|---|---|---|
| `double` (ganz vorne) | Rückgabetyp | nicht angegeben |
| `double scale` | erster Parameter ist `double` | `scale` (ohne Typ) |
| `int n` | zweiter Parameter ist `int` | `n` (ohne Typ) |

Die Typen sind ein Vertrag, den der Compiler bei jedem Aufruf prüft. `neuron("hallo", 784)` würde Python erst zur Laufzeit bemerken, C++ lehnt es schon beim Kompilieren ab.

### Ein Fehler, eine Stelle

Beim ersten Lauf lieferte das Programm `3077.2` statt `3069.36`. Ursache: `i <= n` in der Funktion. Nach **einer** Korrektur waren alle drei Neuronen richtig. Genau das ist der Nutzen von Funktionen.

### Warum liefert Neuron 3 den Wert 0?

Bei negativem Skalierungsfaktor sind alle Gewichte negativ, die Summe ist negativ, und ReLU macht daraus 0.

### Reihenfolge: Deklaration vor Verwendung

Stehen die Funktionen unter `main`, meldet der Compiler:

```
main.cpp:9:30: error: 'neuron' was not declared in this scope
```

Wichtig ist die richtige Erklärung:

- **Eine Funktionsdefinition läuft nicht von selbst.** Sie beschreibt nur, wie die Funktion arbeitet. Eine Funktion läuft erst, wenn sie aufgerufen wird. Ein Programm beginnt immer bei `main`, die Reihenfolge der Definitionen ändert die Ausführungsreihenfolge nicht.
- **Der Fehler entsteht beim Kompilieren.** Das Programm ist noch gar nicht gelaufen. Der Compiler liest die Datei **einmal von oben nach unten**. In Zeile 9 kennt er `neuron` noch nicht, dass es weiter unten kommt, weiß er zu diesem Zeitpunkt nicht.

Analogie: In einem Buch steht auf Seite 9 "Ahmet öffnete die Tür", Ahmet wird aber erst auf Seite 30 vorgestellt. Der Leser fragt auf Seite 9: "Wer ist Ahmet?" Der C++-Compiler ist so ein Leser.

**Warum geht es in Python?** Python sucht Namen erst **beim Aufruf**. Wenn `main()` läuft, ist die Datei längst vollständig geladen.

### Lösung: Deklaration (Prototyp)

Man kündigt die Funktion an: nur die Kopfzeile, mit Semikolon, ohne Rumpf.

```cpp
double relu(double x);
double neuron(double scale, int n);

int main()
{
    println("Neuron 1 = {}", neuron(0.01, 784));
}

double relu(double x)
{
    // Definition weiter unten
}
```

| | Was | Wo | Analogie |
|---|---|---|---|
| **Deklaration** | `double neuron(double scale, int n);` | über `main` | Inhaltsverzeichnis |
| **Definition** | Funktion mit Rumpf | irgendwo | das Kapitel selbst |

Das ist genau der Aufbau aus Praktikum 1: `func1.h` enthielt nur Deklarationen. `#include "func1.h"` kopiert sie an den Anfang von `main5.cpp`, deshalb konnte `main5.cpp` `func1` aufrufen, obwohl die Definition in einer anderen Datei (`func1.cpp`) stand. Der Linker verbindet am Ende Aufruf und Definition.

## Vergleich mit Python

| Gelernt | C++ | Python |
|---|---|---|
| Funktion | `double f(double x) { ... }` | `def f(x): ...` |
| Rückgabe | `return x;` | `return x` |
| Typen | Pflicht, vom Compiler geprüft | optional (Type Hints), nicht erzwungen |
| Reihenfolge | Name muss vor Verwendung bekannt sein | Name wird beim Aufruf gesucht |
| Ankündigung | Deklaration / Header | nicht nötig |

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| `3077.2` statt `3069.36` | `i <= n` in der Funktion | Ein Fehler in einer Funktion trifft alle Aufrufe, eine Korrektur repariert alle |
| `'neuron' was not declared in this scope` | Funktion unter `main` ohne Deklaration | Compiler liest von oben nach unten |
| Erklärung "Funktionen nach main laufen nie" | Definition mit Ausführung verwechselt | Definieren ≠ ausführen; Programm startet immer bei `main` |

## Merksätze

- Wiederholter Code gehört in eine Funktion, das Veränderliche wird Parameter.
- Ein Name muss bekannt sein, bevor er verwendet wird.
- Deklaration = Ankündigung, Definition = Umsetzung.
- Header-Dateien sind Sammlungen von Deklarationen.

## Verbindung zu micrograd

`make(...)`, `operator+` und `operator*` sind Funktionen. Ein ganzes Netz besteht daraus, dieselbe Neuron-Funktion mit verschiedenen Gewichten immer wieder aufzurufen. Später wird die Bibliothek auf `.h`- und `.cpp`-Dateien aufgeteilt, genau mit dem Deklarations-/Definitionsprinzip.

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | `relu` und `neuron`, drei Neuronen |

```bash
g++ -std=c++23 -Wall main.cpp -o main
./main
```

Erwartete Ausgabe (letzte Stellen können wegen Rundung abweichen):

```
Neuron 1 = 3069.36
Neuron 2 = 6138.72
Neuron 3 = 0
```
