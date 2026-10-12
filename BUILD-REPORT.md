# Build- und Testbericht — 12. Oktober 2026

Erfolgreicher Release-Build auf macOS 26.7.1 mit Apple Clang 17.0.0, JUCE 8.0.15 und CMake 4.4. Ziel: macOS ≥ 12.0, ausschließlich ARM64.

## Bestätigt

- VST3-Bundle gebaut; Binärprüfung: `Mach-O 64-bit bundle arm64`.
- VST3-Factory während JUCE-Modulmetadaten-Erzeugung erfolgreich geladen.
- Eigene Plug-in-ID: `Kuv1`; Bundle-ID: `sound.koenich.recall.ultraviolet`.
- Ad-hoc-Signatur nach der Metadatenerzeugung erneuert; `codesign --verify --deep --strict` erfolgreich.
- CTest: 1/1 Testprogramm erfolgreich, alle darin enthaltenen Assertions bestanden.
- Exakt 15 Processor-Parameter; Minima und Maxima aller zwölf Regler geprüft.
- Alle vier Stufen und angezeigten Texte der drei diskreten Parameter geprüft.
- HMF-Skalenabbildung und inverse Abbildung an 1001 Positionen geprüft.
- IN und beide Focus-Zustände ändern keinen Reglerwert.
- Zustand gespeichert, in einer neuen Processor-Instanz wiederhergestellt und für alle 15 Werte verglichen.
- Ungültiger Zustandsblock verworfen, Werte unverändert.
- Bitidentisches Audio vor/nach Processing: Float und Double, IN aus und ein, 1/2/8/64 Kanäle, Blockgrößen 0/1/64/1024.
- Passende Kanalbelegungen akzeptiert, ungleiche Ein-/Ausgangsbelegung abgelehnt.
- Alle zwölf GUI-Regler und drei Tasten an Parameter angebunden; GUI → Parameter und Parameter → GUI geprüft.
- Vorschau direkt aus dem JUCE-Editor gerendert und visuell geprüft.

## Noch nicht als Host-Test durchgeführt

Keine manuelle Installation oder REAPER-Sitzung verändert. Das Speichern/Schließen/Öffnen eines tatsächlichen REAPER-Projekts und Host-Automation wurden nicht manuell getestet. Dafür liegt `REAPER-Pruefung.md` bei. Die automatisierten Tests prüfen die zugrunde liegenden Processor- und GUI-Funktionen, ersetzen aber keinen vollständigen Host-Kompatibilitätstest.

Keine Apple-Notarisierung; lokale ad-hoc-Signatur.

## Update 1.0.1

Fehlerhaft dargestelltes Unicode-Trennzeichen in der Fußzeile durch einen ASCII-Bindestrich ersetzt. Parameter-IDs und Zustandsformat bleiben kompatibel. Nutzer bestätigt einwandfreie Funktion der Version 1.0.0 in REAPER.
