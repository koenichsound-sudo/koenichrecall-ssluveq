# Koenich Recall – SSL UltraViolet EQ 1.0.1

JUCE/C++ VST3 für REAPER auf macOS Apple Silicon, ab macOS 12.
Ein Recall-Werkzeug: Audio wird bei jeder Reglerstellung unverändert durchgereicht, auch bei IN und Focus. Keine EQ-, Gain-, Trim- oder HPF-Bearbeitung; keine Latenz.

## Installation

Die Datei `Koenich Recall - SSL UltraViolet EQ.vst3` in `~/Library/Audio/Plug-Ins/VST3/` kopieren (Ordner bei Bedarf erstellen). In REAPER unter Einstellungen → Plug-ins → VST erneut nach Plug-ins suchen. Das Plug-in als FX einfügen. Die ARM64-Version benötigt die native Apple-Silicon-Version von REAPER.

Der Build ist lokal ad-hoc signiert, nicht Apple-notarisiert. Es ist kein Installer notwendig. Das erste Koenich-Recall-Plug-in bleibt durch eine eigene VST3-ID unabhängig.

## Bedienung und Recall

- Regler mit der Maus ziehen; für feine Schritte beim Ziehen die Strg-Taste halten (JUCE-Standard). Pfeiltasten nach Fokussierung oder Mausrad sind ebenfalls möglich.
- Der aktuelle Zahlenwert erscheint beim Überfahren oder Ziehen. Doppelklick setzt den jeweiligen Standardwert zurück.
- LF Frequency, HF Frequency und HPF rasten in vier Stufen ein.
- IN zeigt seinen Zustand mit einer violetten LED. Focus-Tasten zeigen den eingeschalteten Zustand eingedrückt/dunkler an. Keine Taste verändert andere Parameter.
- Alle 15 Parameter sind hostseitig automatisierbar und werden mit dem REAPER-Projekt gespeichert. Projekt speichern, schließen und später öffnen stellt diese Zustände wieder her.
- Die Oberfläche lässt sich am unteren rechten Rand proportional skalieren. Standardgröße ca. 520 × 921 Pixel.

## Parameter

| Parameter | Bereich / Stufen | Standard |
|---|---|---|
| LF Gain | −9 … +9 dB | 0 dB |
| LF Frequency | 30 / 50 / 60 / 90 Hz | 50 Hz |
| HF Gain | −9 … +9 dB | 0 dB |
| HF Frequency | 8 / 12 / 16 / 20 kHz | 12 kHz |
| LMF Gain | −9 … +9 dB | 0 dB |
| LMF Frequency | 0,1 … 1,1 kHz | 0,6 kHz |
| LMF Q | 0,5 … 3,0 | 1,75 |
| HMF Gain | −9 … +9 dB | 0 dB |
| HMF Frequency | 0,6 … 9 kHz | 3 kHz |
| HMF Q | 0,5 … 3,0 | 1,75 |
| HPF | OFF / 30 / 40 / 50 Hz | OFF |
| Trim | −12 … +12 dB | 0 dB |
| IN | aus / ein | ein |
| LMF Focus | aus / ein | aus |
| HMF Focus | aus / ein | aus |

Die neun kontinuierlichen Regler haben 300° Drehweg, −150° bis +150° relativ zu 12 Uhr. Q läuft entsprechend den schmalen/breiten Kurvensymbolen im Screenshot im Uhrzeigersinn von 3,0 nach 0,5. Die drei diskreten Regler stehen bei −45°, −15°, +15°, +45°, aus den Punkten im Screenshot abgeleitet. Die HMF-Frequenz ist stückweise auf die aufgedruckten Werte kalibriert; die unbeschriftete 12-Uhr-Mitte wird als 3 kHz interpretiert. Dies ist eine grafische Zuordnung, keine Messung der Hardware-Potentiometer.

Die bereitgestellte Originalgrafik ist als Frontplatte eingebettet; die zwölf Regler und drei Schalter werden darüber interaktiv gezeichnet. Das im Screenshot sichtbare LF-Label „70“ wird gemäß Auftrag durch „60“ ersetzt. Die Fußzeile kennzeichnet das Recall-Plug-in.

## Selbst bauen

Benötigt: Xcode Command Line Tools (`xcode-select --install`), CMake ≥ 3.22, Internet für den ersten JUCE-Download oder ein lokales JUCE 8.0.15.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build --config Release --parallel 4
ctest --test-dir build --output-on-failure
```

Offline mit vorhandenem JUCE:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64 -DJUCE_SOURCE_DIR=/absoluter/pfad/zu/JUCE
cmake --build build --config Release --parallel 4
```

Ergebnis: `build/KoenichUltraViolet_artefacts/Release/VST3/Koenich Recall - SSL UltraViolet EQ.vst3`.

GUI-Vorschau aus dem tatsächlichen JUCE-Editor rendern:

```sh
./build/UVEQTests_artefacts/Release/UVEQTests /absoluter/pfad/preview.png
```

## Quellstruktur

`Source/Parameters.h`: stabile Parameter-IDs, Bereiche, Positionen und Skalen. `PluginProcessor.*`: transparente Audioweiterleitung und APVTS-Zustand. `PluginEditor.*`: skalierbare Frontplatte und Host-Anbindung. `Tests/RecallTests.cpp`: Funktionstests. `Assets/frontpanel.png`: vom Nutzer bereitgestellte Referenz.

JUCE 8.0.15 wird versionsgebunden geladen und nicht im Quellarchiv mitgeliefert. Für eine Veröffentlichung gelten die jeweiligen JUCE-Lizenzbedingungen; die Referenzgrafik und Marken sind nicht als eigene Gestaltung beansprucht.
