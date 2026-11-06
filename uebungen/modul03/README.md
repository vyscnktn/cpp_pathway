# Modul 03 – Schleifen: Dieselbe Arbeit N-mal

## Problem

Ein Neuron multipliziert seine Eingaben mit Gewichten und summiert:

```
Ausgabe = w1·x1 + w2·x2 + ... + wn·xn
```

Für 3 Eingaben kann man das von Hand schreiben. Für 784 Eingaben (ein 28×28-Pixel-Bild einer handgeschriebenen Ziffer, das klassische Einstiegsbeispiel) nicht.

Um die Speicherung der Daten geht es erst in Modul 05. Hier werden die Werte per Formel erzeugt:

```
w_i = 0.01 · i
x_i = 1.0
```

## Ziel

Wiederholung mit `for` und `while` ausdrücken, typische Schleifenfehler erkennen und verstehen, wo eine Variable "lebt" (Gültigkeitsbereich).

## Aufgaben

- [x] Die Summe für `n = 784` mit einer `for`-Schleife berechnen
- [x] Das Ergebnis vorher auf Papier berechnen und vergleichen
- [x] `0.01 * i` durch `i / 100.0` ersetzen und vergleichen
- [x] `i < n` durch `i <= n` ersetzen: Wie viele Durchläufe? Wie ändert sich die Summe?
- [x] Nach der Schleife `println("{}", i)` schreiben: Was sagt der Compiler?

## Konzepte

### `for`-Schleife

```cpp
int n = 784;
double sum = 0.0;
for (int i = 0; i < n; ++i)
{
    double w = 0.01 * i;
    double x = 1.0;
    sum += w * x;
}
```

Die drei Teile im Kopf der Schleife sind die ausgeschriebene Form von Pythons `range(n)`:

| Teil | Bedeutung | in Python |
|---|---|---|
| `int i = 0` | Zähler startet bei 0 | Start von `range` |
| `i < n` | vor jedem Durchlauf prüfen: weiter? | Ende von `range` (exklusiv) |
| `++i` | nach jedem Durchlauf um 1 erhöhen | Schrittweite von `range` |

Analogie: ein Zählknopf. Am Anfang auf 0 stellen, vor jeder Runde prüfen, ob die Grenze erreicht ist, nach jeder Runde einmal drücken. Python versteckt alle drei Schritte in `range`, C++ zeigt sie offen.

### `while`-Schleife

Aus Praktikum 1 (`main4.cpp`) bekannt: `while (true) { ... break; }`.

- `for`: wenn man weiß, **wie oft** wiederholt wird.
- `while`: wenn man erst **während** der Wiederholung erfährt, wann Schluss ist.

### Kontrolle auf Papier

```
0 + 1 + ... + 783 = 783 · 784 / 2 = 306936
0.01 · 306936     = 3069.36
```

Ausgabe: `3069.3599999999997`. Das ist dasselbe Problem wie `0.1 + 0.2` aus Modul 02: `0.01` ist binär nicht exakt darstellbar, und die Rundungsfehler aus 784 Additionen summieren sich. Auch `i / 100.0` statt `0.01 * i` kann in einzelnen Schritten im letzten Bit abweichen. Diesmal war die Ausgabe gleich, garantiert ist das aber nicht.

### Gültigkeitsbereich (Scope)

Eine Variable lebt bis zum Ende des `{ }`-Blocks, in dem sie definiert wurde.

- `i` wird im Kopf der `for`-Schleife definiert und verschwindet mit der Schleife. `println("{}", i)` nach der Schleife ergibt: `'i' was not declared in this scope`.
- `sum` ist im Block von `main` definiert und lebt bis zum Ende von `main`.

Wichtig: Das Gegenteil von "lokal" ist hier nicht "global". Auch `sum` ist lokal, nur in einem größeren Block.

In Python erzeugt `for` keinen eigenen Gültigkeitsbereich. Nach `for i in range(784):` ist `i` noch vorhanden und hat den Wert `783`. C++ verhindert, dass man den Zähler versehentlich nach der Schleife weiterverwendet.

## Vergleich mit Python

| Gelernt | C++ | Python |
|---|---|---|
| Zählschleife | `for (int i = 0; i < n; ++i)` | `for i in range(n):` |
| Bedingungsschleife | `while (bedingung)` | `while bedingung:` |
| Zähler nach der Schleife | existiert nicht | existiert weiter |
| Skalarprodukt | Schleife von Hand | `np.dot(w, x)` (intern dieselbe Schleife, in C geschrieben) |

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| `sum = 0` trotz Schleife | `for (int i; ...)` ohne Startwert | Zähler immer initialisieren: `int i = 0` |
| Ergebnis zu groß (785 statt 784 Durchläufe) | `i <= n` statt `i < n` | **Off-by-one**: Grenze genau prüfen |
| `i` nach der Schleife unbekannt | Gültigkeitsbereich | Variable lebt nur in ihrem Block |
| Wieder ohne `-Wall` kompiliert | Gewohnheit | Immer `-Wall` |

### Nicht initialisierter Zähler: undefiniertes Verhalten

`int i;` ohne Startwert enthält, was zufällig gerade im Speicher stand. Hier war der Wert offenbar größer als 784. Die Bedingung `i < n` war schon vor dem ersten Durchlauf falsch, die Schleife lief **kein einziges Mal**. Das ist **undefiniertes Verhalten**: Auf einem anderen Rechner oder mit anderer Kompilierung kann die Schleife auch halb laufen. Gefährlicher als ein klarer Absturz, weil das Ergebnis manchmal fast richtig aussieht.

GCC erkennt nicht initialisierte Variablen nicht immer, mit Optimierung (`-O2`) meist besser.

### Off-by-one

Mit `i <= n` gibt es einen zusätzlichen Durchlauf mit `i = 784`, die Summe steigt um `0.01 · 784 = 7.84` auf etwa `3077.2`. Hier rechnet das Programm nur leicht falsch. Mit einem `vector` (Modul 05) liest derselbe Fehler Speicher hinter dem Ende der Liste, dann wird er gefährlich.

## Merksätze

- Zähler immer mit Startwert anlegen.
- Bei Grenzen fragen: `<` oder `<=`? Wie oft soll die Schleife wirklich laufen?
- Variablen leben nur in ihrem Block.
- Ergebnisse mit Kommazahlen sind Näherungen.

## Verbindung zu micrograd

Jedes Neuron berechnet eine Summe über alle Eingaben, und beim Training werden alle Gewichte in einer Schleife aktualisiert:

```
w = w - lernrate * grad
```

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | Summenschleife |

**Hinweis:** `main.cpp` enthält noch die Experimente `i <= n` und `println("i = {}", i)` nach der Schleife. In diesem Zustand kompiliert die Datei nicht. Für die saubere Version `i < n` verwenden und die `println`-Zeile für `i` entfernen.

```bash
g++ -std=c++23 -Wall main.cpp -o main
./main
```
