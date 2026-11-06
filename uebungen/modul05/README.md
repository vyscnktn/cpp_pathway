# Modul 05 – Daten speichern (`std::vector`) und nicht kopieren (Referenzen)

Dieses Modul fasst zwei Themen zusammen, die direkt aufeinander aufbauen: Zuerst braucht man einen Ort für viele Werte (`vector`), und weil der groß ist, will man ihn nicht bei jedem Funktionsaufruf kopieren (Referenz).

---

## Teil A: `std::vector`

### Problem

Bisher wurden die Gewichte per Formel erzeugt: `0.01 * i`. In einem echten Netz entstehen Gewichte durch Training, jedes ist eine eigene Zahl:

```
w = 0.5, -1.2, 0.8, 0.03, ...   (784 Stück)
```

784 einzelne Variablen (`w0`, `w1`, ...) kann man nicht schreiben, und in einer Schleife gibt es keinen Weg, anhand von `i` die richtige Variable zu wählen. Gebraucht wird **eine Variable mit nummerierten Fächern**.

### Lösung

```cpp
#include <vector>
using std::vector;

vector<double> w = {0.5, -1.2, 0.8};
vector<double> x = {1.0, 2.0, 3.0};
```

| Python | C++ |
|---|---|
| `w = [0.5, -1.2, 0.8]` | `vector<double> w = {0.5, -1.2, 0.8};` |
| `len(w)` | `w.size()` |
| `w[0]` | `w[0]` |
| `w.append(2.0)` | `w.push_back(2.0);` |
| `for v in w:` | `for (double v : w)` |

Der wichtige Unterschied ist `<double>`: In eine Python-Liste darf man alles gemischt legen, in einem C++-`vector` haben alle Elemente **denselben Typ**. Dafür liegen sie im Speicher direkt nebeneinander, und Schleifen sind sehr schnell.

Seit C++23 kann man einen `vector` direkt ausgeben: `println("{}", w)` ergibt `[0.5, -1.2, 0.8]`.

### Das Neuron mit Vektoren

```cpp
double neuron(const vector<double>& w, const vector<double>& x)
{
    if (w.size() != x.size())
    {
        println("Hata: w {} elemanli, x {} elemanli", w.size(), x.size());
        return 0.0;
    }

    double s = 0.0;
    for (std::size_t i = 0; i < w.size(); ++i)
    {
        s += w[i] * x.at(i);
    }
    return relu(s);
}
```

Kontrolle für `w = {0.5, -1.2, 0.8}`, `x = {1.0, 2.0, 3.0}`:

```
relu(0.5·1 + (-1.2)·2 + 0.8·3) = relu(0.5) = 0.5
```

### Wo wird die Aktivierung angewendet?

Erste Schätzung war 2,9: ReLU wurde gedanklich auf **jeden einzelnen Term** angewendet: `0.5 + 0 + 2.4`. Richtig ist: ReLU wird **einmal auf die Gesamtsumme** angewendet, im Code durch `return relu(s);` nach der Schleife.

```
Eingabeterme          Neuronausgang           Schicht
w1·x1 ┐
w2·x2 ├─ summieren → z ──→ relu(z)            ← einmal pro Neuron
w3·x3 ┘

Neuron 1 → relu ┐
Neuron 2 → relu ├─→ Ausgabe der Schicht (Vektor)
Neuron 3 → relu ┘
```

| Ebene | Aktivierung? |
|---|---|
| einzelner Term `w_i · x_i` | nein |
| Ausgang eines Neurons | ja, einmal nach der Summe |
| Schicht | ja, auf den Ausgang jedes einzelnen Neurons |

Warum nicht pro Term? Dann würde jeder negative Term zu 0, und ein negatives Gewicht könnte die Summe nie **verringern**. Ein Neuron könnte nur "mehr feuern", nie "weniger feuern". Negative Gewichte sind hemmende Verbindungen, und Hemmung wirkt nur, wenn sie in die Summe eingeht.

Weitere Punkte:
- Die drei Eingaben können Rohdaten sein (Eingabeschicht) oder die Ausgänge von drei Neuronen der vorherigen Schicht. Dem Neuron ist das egal, es bekommt nur drei Zahlen. So entstehen tiefe Netze: dieselbe Funktion, Schicht für Schicht verkettet.
- Die **letzte Schicht** hat oft kein ReLU: bei Regression gar keine Aktivierung (die Ausgabe darf negativ sein), bei Klassifikation Softmax oder Sigmoid.
- Ohne Aktivierung wäre jede Schicht nur "multiplizieren und summieren". Zwei solche Schichten hintereinander sind mathematisch wieder eine einzige. Der Knick von ReLU bei 0 verhindert dieses Zusammenfallen.

### `int` vs. `size_t`

```
warning: comparison of integer expressions of different signedness:
'int' and 'std::vector<double>::size_type' {aka 'long unsigned int'} [-Wsign-compare]
```

Erste Vermutung war, `w.size()` gebe `double` zurück. Falsch: Die **Elemente** sind `double`, die **Anzahl** der Elemente ist immer eine ganze Zahl. Ein Vektor mit 2,5 Elementen gibt es nicht. Die Warnung betrifft das **Vorzeichen**:

| Typ | kann negativ sein | Beispiel |
|---|---|---|
| `int` | ja (signed) | −5, 0, 784 |
| `size_t` | nein (unsigned) | 0, 784, 18 Trillionen |

Beim Vergleich wird `int` in unsigned umgewandelt. Aus `-1` wird dabei eine riesige Zahl, und `-1 < w.size()` ist plötzlich falsch.

Lösung: Zählertyp anpassen, `for (std::size_t i = 0; i < w.size(); ++i)`. **Nicht** `double i`: Ein Index ist eine Position und immer ganzzahlig.

Ein Vektor hat also zwei unabhängige Typen: den **Elementtyp** (`double`) und den **Positionstyp** (`size_t`).

### Zugriff außerhalb der Grenzen

Ein Tippfehler hat das Thema von selbst ausgelöst:

```cpp
vector<double> w = {0.5, -1,2, 0.8};   // Komma statt Punkt!
```

Im Deutschen und Türkischen ist das Komma das Dezimaltrennzeichen. In C++ trennt das Komma in `{ }` **Elemente**. `w` hatte dadurch 4 Elemente (`0.5, -1, 2, 0.8`), `x` nur 3. Im letzten Durchlauf wurde `x[3]` gelesen:

```
Assertion '__n < this->size()' failed.
Aborted (core dumped)
```

Bei langen Template-Fehlermeldungen: **das Ende lesen**. `__n` ist der gewünschte Index (3), `this->size()` die Größe (3). Das entspricht Pythons `IndexError`.

Drei Wege, sich zu schützen:

| Methode | Prüfung | Geschwindigkeit | Wann |
|---|---|---|---|
| `x[i]` | keine (in manchen Einstellungen doch) | am schnellsten | wenn die Grenzen sicher stimmen |
| `x.at(i)` | bei jedem Zugriff, auch mit `-O2` | etwas langsamer | wenn man unsicher ist, beim Lernen |
| `if` vor der Schleife | einmal am Anfang | fast kostenlos | um zwei Vektoren auf gleiche Länge zu prüfen |

`x.at(i)` mit `-O2`:

```
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 2) >= this->size() (which is 2)
```

Das ist eine **Ausnahme** (Exception), Pythons `IndexError` entspricht ihr am ehesten, und die Meldung ist gut lesbar.

In der Praxis: einmal am Eingang prüfen, danach in der Schleife schnell mit `[]`.

**Designfrage zur eigenen Prüfung:** Die Funktion gibt im Fehlerfall `0.0` zurück. 0 ist aber auch eine ganz normale ReLU-Ausgabe. Der aufrufende Code kann "das Neuron hat wirklich 0 berechnet" nicht von "die Eingabe war falsch" unterscheiden. Python würde mit `raise ValueError(...)` abbrechen. In C++ geht das mit `throw`, das kommt später. Grundsatz: **Im Fehlerfall keinen Wert zurückgeben, der wie ein gültiges Ergebnis aussieht.**

Python-Falle: `zip(self.w, x)` (in micrograd) bricht bei unterschiedlichen Längen **still** ab und verwendet nur die kürzere Länge. Seit Python 3.10 gibt es `zip(w, x, strict=True)`.

---

## Teil B: Referenzen `&`

### Problem

```cpp
double neuron(vector<double> w, vector<double> x)
```

Bei jedem Aufruf werden beide Vektoren **komplett kopiert**:

- 1 `double` = 8 Byte, 784 Elemente ≈ 6 KB pro Vektor
- 2 Vektoren pro Aufruf ≈ 12,5 KB
- 1000 Neuronen × 10.000 Trainingsschritte = 10 Millionen Aufrufe
- 10 Millionen × 12,5 KB ≈ **125 GB** Kopierarbeit

Zusätzlich wird für jede Kopie Speicher angefordert und wieder freigegeben. Die Funktion **liest** die Daten aber nur.

Analogie: Ein Freund will eine Seite in deinem Buch lesen. Statt ihm das Buch zu geben, kopierst du alle 784 Seiten, gibst ihm die Kopie, er liest eine Seite und wirft die Kopie weg, und das 10 Millionen Mal.

### Lösung: Referenz

```cpp
double neuron(const vector<double>& w, const vector<double>& x)
```

- `&`: keine Kopie, sondern ein zweiter Name für das Original. Man gibt das Buch selbst weiter.
- `const`: das Versprechen "diese Funktion ändert die Daten nicht". Man leiht das Buch, darf aber nichts hineinschreiben. Der Compiler prüft das Versprechen:

```cpp
w[0] = 99;   // in einer Funktion mit const vector<double>& w
```
```
error: assignment of read-only location '(& w)->std::vector<double>::operator[](0)'
```

`read-only location`: Man hat `const` versprochen und versucht trotzdem zu schreiben. Der Fehler kommt **beim Kompilieren**, das Programm läuft gar nicht erst.

### Drei Möglichkeiten der Parameterübergabe

| Parameter | Was passiert | Wann |
|---|---|---|
| `vector<double> w` | Kopie | wenn die Funktion eine eigene Kopie braucht |
| `vector<double>& w` | Original, änderbar | wenn die Funktion die Daten des Aufrufers ändern soll |
| `const vector<double>& w` | Original, nur lesen | **Standard für große Daten, die nur gelesen werden** |

Kleine Typen (`int`, `double`) weiterhin als Kopie übergeben: `double relu(double x)`. 8 Byte zu kopieren ist nicht teurer als ein Zugriff über eine Referenz.

Kurzregel:
> Ändert die Funktion die Daten: `&`
> Liest sie nur: `const &`
> Kleiner Wert: Kopie

### Experiment: `clear_all`

```cpp
void clear_all(vector<double> v)        // ohne &
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = 0.0;
    }
}
```

Nach `clear_all(w)` war `w` unverändert. Erste Vermutung: "Es fehlt ein `return`." Falsch. Eine `void`-Funktion gibt nichts zurück. Die eigentliche Ursache:

1. `v` ist eine **Kopie** von `w`.
2. Die Schleife setzt alle Elemente der Kopie auf 0.
3. Die Funktion endet, `v` verlässt den Gültigkeitsbereich und wird gelöscht.
4. `w` in `main` wurde nie berührt.

Die Idee mit `return` ist trotzdem ein gültiger, anderer Weg:

```cpp
vector<double> clear_all(vector<double> v) { ...; return v; }
w = clear_all(w);
```

Dabei wird aber eine Kopie erzeugt und anschließend das Original überschrieben. Direkter ist `&`:

```cpp
void clear_all(vector<double>& v)       // mit & → Original
```

Ergebnis: `w = [0, 0, 0], x = [0, 0]`.

`const vector<double>& v` funktioniert hier **nicht**: Die einzige Aufgabe der Funktion ist zu schreiben, `const` verbietet genau das.

### Vergleich mit Python: genau umgekehrt

In Python wird eine Liste an eine Funktion **nie kopiert**:

```python
def kaputt(w):
    w[0] = 99

w = [0.5, -1.2, 0.8]
kaputt(w)
print(w)   # [99, -1.2, 0.8]
```

Jeder Listenparameter in Python verhält sich wie `vector<double>&`. Ein `const` gibt es nicht. In C++ ist die Kopie der Standard, und `&` muss man ausdrücklich schreiben.

---

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| Warnung `-Wsign-compare` | `int i` mit `w.size()` verglichen | Zähler als `std::size_t` |
| Vermutung "`size()` liefert `double`" | Elementtyp und Positionstyp verwechselt | Anzahl ist immer ganzzahlig |
| `Assertion '__n < this->size()' failed` | `-1,2` statt `-1.2` | Dezimaltrennzeichen ist der Punkt |
| ReLU-Ergebnis 2,9 erwartet | ReLU auf jeden Term angewendet | Aktivierung einmal auf die Summe |
| `clear_all` hat nichts gelöscht | Übergabe als Kopie | Daten ändern: `&` |
| Vermutung "fehlendes `return`" | `void`-Funktion falsch verstanden | Kopie wurde verändert, nicht das Original |
| `-o2` statt `-O2` | Klein-/Großschreibung | `-O2` = Optimierung, `-o` = Ausgabedateiname |
| `const vector<double> x` ohne `&` | `&` vergessen | `const` ohne `&` ist eine unveränderliche **Kopie** |

## Merksätze

- `vector<T>`: viele Werte gleichen Typs, nebeneinander im Speicher.
- Index ist `size_t`, niemals `double`.
- Fehlermeldungen von Templates: das Ende lesen.
- Große Daten nur lesen: `const &`. Ändern: `&`. Kleine Werte: Kopie.
- Im Fehlerfall keinen gültig aussehenden Wert zurückgeben.

## Verbindung zu micrograd

- `vector<Ptr> prev;` in `Value` ist genau so ein Vektor: die Liste der Knoten, aus denen ein Wert entstanden ist.
- `operator+(const Ptr& a, const Ptr& b)`: `const &`, um das Ticket nicht zu kopieren und den Zähler nicht unnötig zu erhöhen.
- Ein Neuron ist `relu(Summe(w_i · x_i))`, eine Schicht ist ein Vektor von Neuronen.

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | `relu`, `clear_all` mit `&`, `neuron` mit `const &`, Größenprüfung und `at()` |

```bash
g++ -std=c++23 -Wall -O2 main.cpp -o main
./main
```

Aktueller Stand der Datei: `x` hat absichtlich nur 2 Elemente, um die Größenprüfung zu zeigen. Ausgabe:

```
Hata: w 3 elemanli, x 2 elemanli
Neuron = 0
After clear_all: w = [0, 0, 0], x = [0, 0]
```
