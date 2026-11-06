# Modul 06 – Zeiger (Pointer): "Nichts" sagen können

> Status: **in Arbeit**

## Vorwissen

Aus einer früheren Übung bekannt:

```cpp
int x = 5;
int* p = &x;    // p speichert die Adresse von x
*p = 10;        // gehe zur Adresse und schreibe 10 hinein
// x ist jetzt 10
```

- `&x`: "Welche Adresse hat x?"
- `*p`: "Gehe zur Adresse in p und schau nach, was dort liegt."

Analogie Hotel: `x` ist der Gegenstand im Zimmer, `&x` die Zimmernummer, `p` ein Zettel mit dieser Nummer, `*p` bedeutet, zum Zimmer zu gehen und hineinzuschauen. Kopiert man den Zettel, gibt es zwei Zettel, aber immer noch nur ein Zimmer.

`*p = 10` hat `x` verändert, obwohl `x` gar nicht erwähnt wurde, weil `p` auf dasselbe Zimmer zeigt.

## Problem

Aufgabe: In einem Gewichtsvektor das **größte** Gewicht finden und dem Aufrufer erlauben, es zu ändern (zum Beispiel auf 0 setzen, Gewichtsbegrenzung).

### Erster Versuch: Referenz zurückgeben

```cpp
double& biggest(vector<double>& w)
{
    std::size_t k = 0;
    for (std::size_t i = 1; i < w.size(); i++)
    {
        if (w[i] > w[k])
        {
            k = i;
        }
    }
    return w[k];
}

biggest(w) = 0.0;   // größtes Gewicht auf 0 setzen
```

### Warum kann man einem Funktionsaufruf etwas zuweisen?

Das sieht zunächst seltsam aus. Der Unterschied liegt im `&` beim Rückgabetyp.

- **Eine normale Funktion gibt einen Wert zurück**, eine Kopie. `relu(x) = 0.0` ist sinnlos, denn die Zahl liegt nirgendwo, man würde nichts verändern.
- **Eine Funktion mit `double&` gibt das Fach selbst zurück.** `biggest(w)` liefert nicht die Zahl `0.8`, sondern das Fach in `w`, in dem `0.8` liegt. Einem Fach kann man etwas zuweisen:

```cpp
biggest(w) = 0.0;
// gelesen: "lege 0.0 in das Fach, das biggest gefunden hat"
// also:    w[2] = 0.0;
```

Analogie Bibliothek: Man fragt "Welches ist das dickste Buch?"
- **Wert zurückgeben:** Die Bibliothekarin schreibt den Titel auf einen Zettel. Kritzelt man auf den Zettel, ändert sich im Regal nichts.
- **Referenz zurückgeben:** Die Bibliothekarin führt einen zum Regal und zeigt auf das Buch. Man kann es herausnehmen und etwas anderes hinstellen.

Gegenprobe: Ohne `&` im Rückgabetyp (`double biggest(...)`) kompiliert `biggest(w) = 0.0;` nicht, weil auf der linken Seite einer Zuweisung ein Fach (lvalue) stehen muss.

Python kann kein Fach aus einer Liste zurückgeben, man gibt stattdessen die **Position** zurück:

```python
def biggest_index(w):
    k = 0
    for i in range(1, len(w)):
        if w[i] > w[k]:
            k = i
    return k

w[biggest_index(w)] = 0.0
```

Die C++-Version macht dasselbe, nur steckt der Teil `w[...]` in der Funktion.

### Das eigentliche Problem: leerer Vektor

```cpp
vector<double> empty = {};
biggest(empty) = 0.0;
```

```
Segmentation fault (core dumped)
```

Im leeren Vektor gibt es kein `w[0]`, die Funktion gibt trotzdem `w[0]` zurück, und es wird an diese Adresse geschrieben. Die Adresse gehört nicht dem Programm. Das Betriebssystem bemerkt das und beendet das Programm. **Segmentation fault** heißt: "Du hast Speicher berührt, der dir nicht gehört."

Mit `-O2` gibt es nicht einmal die freundliche `Assertion`-Meldung aus Modul 05, und die Meldung sagt weder wo noch warum. Python würde `IndexError` melden.

Der eigentliche Fehler liegt im Entwurf: Der Rückgabetyp `double&` verspricht "ich zeige immer auf ein Fach". Bei einem leeren Vektor lässt sich das Versprechen nicht halten. Eine Referenz kann nicht "nichts" bedeuten.

Auch die Python-Idee mit dem Index hilft hier nicht direkt: Für "nicht gefunden" würde man `-1` zurückgeben, aber ein Index ist `size_t` und kann nicht negativ sein (Modul 05).

## Lösung: Zeiger und `nullptr`

Ein Zeiger speichert eine Adresse, und es gibt eine besondere Adresse: **`nullptr`**, "nirgendwo".

```cpp
double* biggest(vector<double>& w)
{
    if (w.empty())
    {
        return nullptr;          // "nicht gefunden"
    }

    std::size_t k = 0;
    for (std::size_t i = 1; i < w.size(); ++i)
    {
        if (w[i] > w[k])
        {
            k = i;
        }
    }
    return &w[k];                // Adresse des Fachs
}
```

Beim Verwenden zuerst prüfen:

```cpp
double* p = biggest(w);
if (p != nullptr)
{
    *p = 0.0;
}
else
{
    println("vektor bos");
}
```

Python-Entsprechung:

```python
k = biggest_index(w)   # gibt None zurück, wenn w leer ist
if k is not None:
    w[k] = 0.0
```

`nullptr` ist das C++-Gegenstück zu `None`.

## Referenz oder Zeiger?

| | Referenz `&` | Zeiger `*` |
|---|---|---|
| Kann "nichts" bedeuten? | nein | ja, `nullptr` |
| Kann später auf etwas anderes zeigen? | nein | ja |
| Zugriff | wie eine normale Variable | mit `*p` oder `p->` |
| Gefahr | gering | Absturz, wenn man `nullptr` nicht prüft |

Regel: **Standardmäßig Referenzen verwenden.** Zeiger, wenn etwas "vielleicht nicht vorhanden" ist oder später auf etwas anderes zeigen soll.

`p->data` ist die Kurzform von `(*p).data`.

## Aufgaben

- [x] `biggest` mit Referenz-Rückgabe schreiben und `biggest(w) = 0.0` ausprobieren
- [x] Mit leerem Vektor testen: `Segmentation fault`
- [ ] Gegenprobe: `&` im Rückgabetyp entfernen und die Compilermeldung lesen
- [ ] Auf die Zeiger-Version umstellen, mit `w` und mit leerem Vektor testen, beides darf nicht abstürzen
- [ ] Falle: die `nullptr`-Prüfung weglassen und `*biggest(empty) = 0.0;` schreiben. Was passiert?

## Stolperfallen und meine Fehler

| Was passiert ist | Ursache | Merksatz |
|---|---|---|
| `'vector' was not declared in this scope` | `#include <vector>` fehlte | Was man benutzt, bindet man selbst ein |
| Mehrere Folgefehler | erster Fehler hat den Rest ausgelöst | Immer beim ersten Fehler anfangen |
| `'w' was not declared in this scope` | `w` in `main` nicht angelegt | Variablen vor der Verwendung definieren |
| `println(biggest(w) = 0.0)` | `println` braucht als erstes Argument einen Formattext | Zuweisung und Ausgabe trennen |
| `Segmentation fault` | `double&` bei leerem Vektor | Eine Referenz kann nicht "nichts" sein |

Hinweis zur Compilermeldung: `<print>` erwähnt `vector` intern nur namentlich, liefert aber nicht die vollständige Definition. Darum reicht `<print>` allein nicht. Nie darauf verlassen, dass eine andere Header-Datei zufällig etwas mitbringt.

## Merksätze

- Zeiger = Adresse. `&x` holt die Adresse, `*p` geht zur Adresse.
- `nullptr` bedeutet "nirgendwo", entspricht Pythons `None`.
- Ein Zeiger kann "nichts" sagen, dafür muss man ihn vor dem Benutzen prüfen.
- Standard: Referenz. Zeiger nur, wenn "nicht vorhanden" möglich ist.

## Verbindung zu micrograd

- `Value* o = out.get();` in `operator+` ist ein roher Zeiger, der keinen Referenzzähler erhöht. Damit wird der Kreis vermieden, in dem ein Knoten sich über seine eigene `backward`-Funktion selbst festhält.
- `a->data` in micrograd ist `(*a).data`.
- `shared_ptr` (später) ist ein Zeiger mit Zähler, der seinen Speicher selbst freigibt.

## Dateien

| Datei | Inhalt |
|---|---|
| `main.cpp` | `biggest` mit Referenz-Rückgabe, Test mit leerem Vektor (stürzt absichtlich ab) |

```bash
g++ -std=c++23 -Wall -O2 main.cpp -o main
./main
```
