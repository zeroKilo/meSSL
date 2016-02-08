// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem MESSL_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// MESSL_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef MESSL_EXPORTS
#define MESSL_API __declspec(dllexport)
#else
#define MESSL_API __declspec(dllimport)
#endif

// Diese Klasse wird aus meSSL.dll exportiert.
class MESSL_API CmeSSL {
public:
	CmeSSL(void);
	// TODO: Hier die Methoden hinzufügen.
};

extern MESSL_API int nmeSSL;

MESSL_API int fnmeSSL(void);
