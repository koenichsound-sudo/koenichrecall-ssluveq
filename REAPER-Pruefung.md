# Kurzer Test in REAPER

Diese Schritte ergänzen die automatisierten Tests um die Prüfung im tatsächlichen Host:

1. VST3 in den Benutzer-VST3-Ordner kopieren, in nativem ARM64-REAPER neu scannen und auf einer Spur einfügen.
2. Alle zwölf Regler verstellen, beide Focus-Tasten einschalten und IN ausschalten. Projekt speichern.
3. Projekt schließen und wieder öffnen. Alle Werte, Schalter und die IN-LED müssen unverändert wiederhergestellt sein.
4. Plug-in-Fenster schließen und erneut öffnen. Werte bleiben bestehen.
5. Zwei Instanzen mit unterschiedlichen Werten verwenden: Die Zustände müssen unabhängig bleiben.
6. Für eine Parameter-Automationsspur Werte schreiben und abspielen. Regler und Anzeige müssen folgen.
7. Bei laufendem Audio IN, HPF, Gains, Trim und Focus ändern. Es darf keine Klang- oder Pegeländerung geben.

Der automatisierte Test prüft Zustandsspeicherung und Wiederherstellung über dieselben Processor-State-Funktionen, die der VST3-Host benutzt. Eine manuelle REAPER-Projektprüfung ist davon getrennt und im Buildbericht ausdrücklich ausgewiesen.
