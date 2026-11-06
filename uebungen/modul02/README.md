# Modul 02 – Entscheidungen: `if` und Vergleiche

## Problem

Die Programme aus Modul 01 führen immer dieselben Zeilen in derselben Reihenfolge aus. Oft muss ein Programm aber **abhängig von den Daten** anders reagieren.

Beispiel aus neuronalen Netzen, die Aktivierungsfunktion **ReLU**:

- Ist die Zahl positiv, wird sie unverändert durchgelassen.
- Ist sie negativ, wird sie zu 0.

```
relu(3.0)  = 3.0
relu(-2.0) = 0.0
```

Mit reiner Arithmetik (`+`, `-`, `*`, `/`) lässt sich "wenn negativ, dann 0" nicht ausdrücken. Es fehlt ein Mechanismus, der anhand eines Wertes entscheidet, welche Zeile ausgeführt wird.

## Ziel

Verzweigungen mit `if` / `else` schreiben, Vergleichsoperatoren richtig einsetzen und zwei typische Fallen erkennen.

## Aufgaben

- [x] ReLU als Funktion schreiben und mit `3.0`, `-2.0` und `0.0` testen
- [x] Falle 1: `if (n = 0)` statt `if (n == 0)`, Ausgabe vorhersagen, mit `-Wall` kompilieren, Warnung lesen
- [x] Falle 2: `0.1 + 0.2 == 0.3`, Ausgabe vorhersagen, dann mit Toleranz vergleichen

## Konzepte

### `if` / `else`

```cpp
if (x <= 0)
{
    return 0.0;
}
else
{
    return x;
}
```

- Die Bedingung steht in runden Klammern.
- Jeder Block hat seine eigenen geschweiften Klammern (vgl. Praktikum 1, `main3.cpp`).
- Die Einrückung dient nur der Lesbarkeit. Der Block wird durch `{ }` bestimmt, nicht durch Leerzeichen.
- `else if` entspricht Pythons `elif`.

### Vergleichsoperatoren und Logik

| Bedeutung | C++ | Python |
|---|---|---|
| gleich / ungleich | `==`, `!=` | `==`, `!=` |
| kleiner / größer | `<`, `>`, `<=`, `>=` | gleich |
| und / oder / nicht | `&&`, `\|\|`, `!` | `and`, `or`, `not` |

### Funktion mit Rückgabewert

```cpp
double relu(double x)
{
    if (x <= 0)
    {
        return 0.0;
    }
    return x;
}
```

`return` beendet die Funktion sofort. Deshalb braucht man nach dem ersten `return` kein `else` mehr.

Zuerst wurde ReLU mit einem Ausgabeparameter `void ReLU(double x, double& y)` geschrieben. Das funktioniert, aber für ein einzelnes Ergebnis ist `return` üblicher und lesbarer. Ausgabeparameter über `&` sind sinnvoll, wenn eine Funktion mehrere Ergebnisse liefern muss (siehe Modul 05).

### Falle 1: `=` statt `==`

```cpp
int n = 5;
if (n = 0)   // Zuweisung, kein Vergleich!
```

`n = 0` fragt nicht "ist n null?", sondern sagt "lege 0 in n". Danach wird der zugewiesene Wert (0) als Bedingung benutzt. 0 gilt in C++ als "falsch", also läuft immer der `else`-Zweig, und der alte Wert 5 ist überschrieben. Die Ausgabe widerspricht sich selbst:

```
n sifir degil, n = 0
```

Mit `-Wall` meldet der Compiler:

```
main.cpp:27:11: warning: suggest parentheses around assignment used as truth value [-Wparentheses]
   27 |     if (n = 0)
      |         ~~^~~
```

- `27:11`: Zeile 27, Spalte 11, das `^` zeigt auf das `=`.
- `warning`: Das Programm wird trotzdem gebaut.
- `assignment used as truth value`: Eine Zuweisung wird als Bedingung verwendet.
- `suggest parentheses`: "Wenn das Absicht ist, schreib `if ((n = 0))`." Der Compiler kennt die Absicht nicht, deshalb fragt er nur nach.

In Python ist `if n = 0:` ein `SyntaxError`. C++ erlaubt es und warnt nur.

### Falle 2: Kommazahlen mit `==` vergleichen

```cpp
double t = 0.1 + 0.2;
if (t == 0.3)   // false!
```

Der Computer speichert Zahlen binär. 0,1 ist im Binärsystem ein unendlicher Bruch (wie 1/3 = 0,333... im Dezimalsystem) und wird nach 64 Bit abgeschnitten. Kleine Fehler summieren sich:

```
0.1 + 0.2 = 0.30000000000000004441
0.3       = 0.29999999999999998890
```

Lösung: Kommazahlen auf "nah genug" prüfen:

```cpp
#include <cmath>

if (std::abs(t - 0.3) < 1e-9)
```

`std::abs` ist der Betrag, `1e-9` die erlaubte Toleranz. In Python: `math.isclose(t, 0.3)`.

## Vergleich mit Python

| Gelernt | C++ | Python |
|---|---|---|
| Verzweigung | `if (...) { } else { }` | `if ...:` / `else:` |
| weiterer Fall | `else if` | `elif` |
| Logik | `&&`, `\|\|`, `!` | `and`, `or`, `not` |
| Falle 1 | `if (n = 0)` kompiliert, nur Warnung | `SyntaxError` |
| Falle 2 | Kommazahlen nicht mit `==` vergleichen | gleiches Problem, `math.isclose` |

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| Falle 1 zuerst nicht ausgelöst | aus Gewohnheit `==` geschrieben | Zum Testen den Fehler bewusst einbauen |
| `0.1 + 0.2 == 0.3` für wahr gehalten | binäre Darstellung | Kommazahlen sind Näherungen |
| Ohne `-Wall` kompiliert | Warnungen waren aus | **Immer mit `-Wall` kompilieren und Warnungen lesen** |

## Merksätze

- `=` weist zu, `==` vergleicht.
- Kommazahlen nie mit `==` vergleichen, sondern mit Toleranz.
- Warnungen sind kein Rauschen. Ein Fehler, den der Compiler in 2 Sekunden findet, kann ohne Warnung Stunden kosten.

## ReLU am Nullpunkt

`relu(0)` ist bei `x < 0` und bei `x <= 0` gleich 0, beim Vorwärtsdurchlauf macht es keinen Unterschied. Die **Ableitung** ist bei 0 aber nicht definiert (links 0, rechts 1). Im Code muss man sich entscheiden. Karpathys micrograd verwendet `out.data > 0`, die Ableitung bei 0 ist also 0.

## Verbindung zu micrograd

ReLU ist eine der Operationen von `Value`. Sowohl der Vorwärtsdurchlauf (`max(0, x)`) als auch die Ableitung (1 für positive Werte, sonst 0) sind ein `if`. Der Toleranzvergleich wird später gebraucht, um den automatisch berechneten Gradienten mit dem numerischen Gradienten aus der Grenzwertdefinition zu vergleichen.

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | ReLU, Falle 1 (korrigiert) und Falle 2 mit Toleranz |

```bash
g++ -std=c++23 -Wall main.cpp -o main
./main
```
